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
			//û�ҵ����õ�free list,׼���������free list
			void *r = refill(ROUND_UP(n));
			return r;
		}
		//����free list
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



	//���·����ڴ�ռ�
	void *alloc::reallocate(void *ptr, size_t old_sz, size_t new_sz) 
	{
		deallocate(ptr, old_sz);
		ptr = allocate(new_sz);

		return ptr;
	}

	//����һ����СΪn�Ķ��󣬲�����ʱ���Ϊ�ʵ���free list���ӽڵ�
	//����n�Ѿ��ʵ��ϵ���8�ı���
	void* alloc::refill(size_t n)
	{
		int nobjs = 20;
		char * chunk = chunk_alloc(n, nobjs);
		obj * volatile * my_free_list;
		obj * result;
		obj * current_obj, *next_obj;
		int i;
		//���ֻ���һ�����飬�������ͷ�����������ã�free list���½ڵ�
		if (1 == nobjs) return (chunk);
		//����׼������free list �������½ڵ�
		my_free_list = free_list + FREELIST_INDEX(n);

		//������chunk�ռ��ڽ���free list
		result = (obj *)chunk;
		*my_free_list = next_obj = (obj *)(chunk + n);
		//���½�free list�ĸ��ڵ㴮������
		for (i = 1;; i++) 
		{	//��1��ʼ����Ϊ��0�������ظ��Ͷˡ�
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
	//����size�Ѿ��ʵ��ϵ���8�ı���
	//ע�����nobjs��pass by reference
	char* alloc::chunk_alloc(size_t size, int & nobjs)
	{
		char* result;
		size_t total_bytes = size * nobjs;//�ܹ�����Ŀռ��С
		size_t bytes_left = end_free - start_free;//�ڴ��ʣ��ռ�

		if (bytes_left >= total_bytes) 
		{
			result = start_free;
			start_free += total_bytes;
			return(result);
		}
		else if (bytes_left >= size)
		{
			//�ڴ��ʣ��ռ䲻����ȫ���������������㹻��Ӧһ�����������ϵ�����
			nobjs = bytes_left / size;
			total_bytes = size*nobjs;
			result = start_free;
			start_free += total_bytes;
			return (result);
		}
		else {
			//�ڴ��ʣ��ռ���һ������Ĵ�С���޷��ṩ
			//ÿ������������һЩ�����ڴ�
			size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);
			if (bytes_left > 0)
			{//������ͷ
				obj* volatile * my_free_list = free_list + FREELIST_INDEX(bytes_left);
				//����free list,���ڴ���еĲ���ռ����
				((obj *)start_free)->free_list_link = *my_free_list;
				*my_free_list = (obj*)start_free;
			}

			//����heap�ռ䣬���������ڴ��
			start_free = (char*)malloc(bytes_to_get);
			if (0 == start_free)
			{
				//heap�ռ䲻�㣬malloc()ʧ��
				int i;
				obj * volatile * my_free_list, *p;
				//���ż�����������ӵ�еĶ������ⲻ������˺������ǲ����㳢������
				//��С�����飬��Ϊ���ڶ���̻��������׵�������
				//������Ѱ�ʵ���free list
				//��ν�ʵ���ָ������δ�����飬�����鹻��֮free list
				for (i = size; i <= __MAX_BYTES; i += __ALIGN)
				{
					my_free_list = free_list + FREELIST_INDEX(i);
					p = *my_free_list;
					if (0 != p) {//free list����δ������,�Ż��ڴ��
						*my_free_list = p->free_list_link;
						start_free = (char*)p;
						end_free = start_free + i;
						return (chunk_alloc(size, nobjs));
					}
				}
				end_free = 0;//ɽ��ˮ��������û�п����ڴ���
				//�׳��쳣��䣬�˴���
			}
			//heap�ռ���㣬malloc()�ɹ�
			heap_size += bytes_to_get;//heap_size��¼��ǰ�ڴ������
			end_free = start_free + bytes_to_get;
			return chunk_alloc(size, nobjs);
		}
	}
}