#include "alloc.h"

namespace HGSTL {
	char *alloc::start_free = 0;
	char *alloc::end_free = 0;
	size_t alloc::heap_size = 0;
	alloc::obj * volatile alloc::free_list[alloc::__NFRELISTS] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	};

	void *alloc::allocate(size_t n)
	{
		obj * volatile * my_free_list;
		obj * result;
		if (n > (size_t)__MAX_BYTES)
		{
			return malloc(n);
		}

		my_free_list = free_list + FREELIST_INDEX(n);
		result = *my_free_list;
		if (result == 0) 
		{
			//没找到可用的free list,准备重新填充free list
			void *r = refill(ROUND_UP(n));
			return r;
		}
		//调整free list
		*my_free_list = result->free_list_link;
		return (result);
	}

	void alloc::deallocate(void *p, size_t n)
	{
		obj *q = (obj *)p;
		obj * volatile * my_free_list;

		if (n > (size_t)__MAX_BYTES)
		{
			free(p);
			return;
		}
		my_free_list = free_list + FREELIST_INDEX(n);
		q->free_list_link = *my_free_list;
		*my_free_list = q;
	}



	//重新分配内存空间
	void *alloc::reallocate(void *ptr, size_t old_sz, size_t new_sz) 
	{
		deallocate(ptr, old_sz);
		ptr = allocate(new_sz);

		return ptr;
	}

	//返回一个大小为n的对象，并且有时候会为适当的free list增加节点
	//假设n已经适当上调至8的倍数
	void* alloc::refill(size_t n)
	{
		int nobjs = 20;
		char * chunk = chunk_alloc(n, nobjs);
		obj * volatile * my_free_list;
		obj * result;
		obj * current_obj, *next_obj;
		int i;
		//如果只获得一个区块，这个区块就分配给调用者用，free list无新节点
		if (1 == nobjs) return (chunk);
		//否则准备调整free list ，纳入新节点
		my_free_list = free_list + FREELIST_INDEX(n);

		//以下在chunk空间内建立free list
		result = (obj *)chunk;
		*my_free_list = next_obj = (obj *)(chunk + n);
		//以下将free list的各节点串接起来
		for (i = 1;; i++) 
		{	//从1开始，因为第0个将返回给客端。
			current_obj = next_obj;
			next_obj = (obj*)((char*)next_obj + n);
			if (nobjs - 1 == i) {
				current_obj->free_list_link = 0;
				break;
			}
			else {
				current_obj->free_list_link = next_obj;
			}
		}
		return (result);
	}
	//假设size已经适当上调至8的倍数
	//注意参数nobjs是pass by reference
	char* alloc::chunk_alloc(size_t size, int & nobjs)
	{
		char* result;
		size_t total_bytes = size * nobjs;//总共申请的空间大小
		size_t bytes_left = end_free - start_free;//内存池剩余空间

		if (bytes_left >= total_bytes) 
		{
			result = start_free;
			start_free += total_bytes;
			return(result);
		}
		else if (bytes_left >= size)
		{
			//内存池剩余空间不能完全满足需求量，但足够供应一个（含）以上的区块
			nobjs = bytes_left / size;
			total_bytes = size*nobjs;
			result = start_free;
			start_free += total_bytes;
			return (result);
		}
		else {
			//内存池剩余空间连一个区块的大小都无法提供
			//每次申请两倍多一些的新内存
			size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);
			if (bytes_left > 0)
			{//还有零头
				obj* volatile * my_free_list = free_list + FREELIST_INDEX(bytes_left);
				//调整free list,将内存池中的残余空间编入
				((obj *)start_free)->free_list_link = *my_free_list;
				*my_free_list = (obj*)start_free;
			}

			//配置heap空间，用来补充内存池
			start_free = (char*)malloc(bytes_to_get);
			if (0 == start_free)
			{
				//heap空间不足，malloc()失败
				int i;
				obj * volatile * my_free_list, *p;
				//试着检视我们手上拥有的东西，这不会造成伤害。我们不打算尝试配置
				//较小的区块，因为那在多进程机器上容易导致灾难
				//以下搜寻适当的free list
				//所谓适当是指“尚有未用区块，且区块够大”之free list
				for (i = size; i <= __MAX_BYTES; i += __ALIGN)
				{
					my_free_list = free_list + FREELIST_INDEX(i);
					p = *my_free_list;
					if (0 != p) {//free list尚有未用区块,放回内存池
						*my_free_list = p->free_list_link;
						start_free = (char*)p;
						end_free = start_free + i;
						return (chunk_alloc(size, nobjs));
					}
				}
				end_free = 0;//山穷水尽，到处没有可用内存了
				//抛出异常语句，此处略
			}
			//heap空间充足，malloc()成功
			heap_size += bytes_to_get;//heap_size记录当前内存池容量
			end_free = start_free + bytes_to_get;
			return chunk_alloc(size, nobjs);
		}
	}
}