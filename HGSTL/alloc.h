#ifndef _ALLOC_H_
#define _ALLOC_H_
#include<cstdlib>
namespace HGSTL {
	class alloc {
	private:
		enum {__ALIGN = 8};//С��������ϵ��߽�
		enum {__MAX_BYTES = 128};//С�����������
		enum {__NFRELISTS = __MAX_BYTES/__ALIGN};//free-lists����

	private:
		//ROUND_UP()��bytes�ϵ���8�ı���
		static size_t ROUND_UP(size_t bytes) 
		{
			return ((bytes+__ALIGN - 1) & ~(__ALIGN - 1));
		}
	private:
		union obj 
		{				//free-lists�Ľڵ㹹��
			union obj * free_list_link;
			char client_data[1];
		};
	private:
		//16��free-lists
		static obj * volatile free_list[__NFRELISTS];
		//���º������������С������ʹ�õ�n��free-list.n��0����
		static size_t FREELIST_INDEX(size_t bytes) 
		{
			return ((bytes + __ALIGN - 1) / __ALIGN - 1);
		}
		//����һ����СΪn�Ķ��󣬲����ܼ����СΪn���������鵽free list
		static void *refill(size_t bytes);

		//����һ���ռ䣬������nobjs����СΪ"size"������
		//�������nobjs�������������㣬nobjs���ܻή��
		static char *chunk_alloc(size_t size, int &nobjs);

		//Chunk allocation state
		static char *start_free;	//�ڴ����ʼλ�ã�ֻ��chunk_alloc()�б仯
		static char *end_free;	//�ڴ�ؽ���λ�ã�ֻ��chunk_alloc()�б仯
		static size_t heap_size;//��¼��ǰ�ڴ��������С

	public:
		static void * allocate(size_t n);
		static void deallocate(void *p, size_t n);
		static void * reallocate(void *p, size_t old_sz, size_t new_sz);
	};



}
#endif // !_ALLOC_H_
