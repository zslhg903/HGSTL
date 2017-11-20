#ifndef _ALLOC_H_
#define _ALLOC_H_
#include<cstdlib>
namespace HGSTL {
	class alloc {
	private:
		enum {__ALIGN = 8};//小型区块的上调边界
		enum {__MAX_BYTES = 128};//小型区块的上限
		enum {__NFRELISTS = __MAX_BYTES/__ALIGN};//free-lists个数

	private:
		//ROUND_UP()将bytes上调至8的倍数
		static size_t ROUND_UP(size_t bytes) 
		{
			return ((bytes+__ALIGN - 1) & ~(__ALIGN - 1));
		}
	private:
		union obj 
		{				//free-lists的节点构造
			union obj * free_list_link;
			char client_data[1];
		};
	private:
		//16个free-lists
		static obj * volatile free_list[__NFRELISTS];
		//以下函数根据区块大小，决定使用第n号free-list.n从0起算
		static size_t FREELIST_INDEX(size_t bytes) 
		{
			return ((bytes + __ALIGN - 1) / __ALIGN - 1);
		}
		//返回一个大小为n的对象，并可能加入大小为n的其它区块到free list
		static void *refill(size_t bytes);

		//配置一大块空间，可容纳nobjs个大小为"size"的区块
		//如果配置nobjs个区块有所不便，nobjs可能会降低
		static char *chunk_alloc(size_t size, int &nobjs);

		//Chunk allocation state
		static char *start_free;	//内存池起始位置，只在chunk_alloc()中变化
		static char *end_free;	//内存池结束位置，只在chunk_alloc()中变化
		static size_t heap_size;//记录当前内存池容量大小

	public:
		static void * allocate(size_t n);
		static void deallocate(void *p, size_t n);
		static void * reallocate(void *p, size_t old_sz, size_t new_sz);
	};



}
#endif // !_ALLOC_H_
