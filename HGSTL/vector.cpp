#include "vector.h"
namespace HGSTL {
	template<class T,class Alloc>
	void vector<T, Alloc>::insert_aux(iterator position, const T& x) {
		if (finish != end_of_storage) {
			construct(finish, *(finish - 1));
			++finish;
			T x_copy = x;
			copy_backward(position, finish - 2, finish - 1);
			*position = x_copy;
		}
		else {
			const size_type old_size = size();
			const size_type len = old_size != 0 ? 2 * old_size : 1;
			//��������ԭ�����ԭ��СΪ0��������1����Ԫ�ش�С��
			//���ԭ��С��Ϊ0��������ԭ��С������
			//ǰ�����������ԭ���ݣ�����׼����������������


			iterator new_start = data_allocator::allocate(len);
			iterator new_finish = new_start;
			try {
				//��ԭvector�����ݿ�������vector
				new_finish = uninitialized_copy(start, position, new_start);
				construct(new_finish, x);  //Ϊ��Ԫ���趨��ֵx
				new_finish++;//����ˮλ
				//��������ԭ����Ҳ��������
				new_finish = uninitialized_copy(position, finish, new_finish);
			}
			catch(...){
				//�����쳣���ع�
				destroy(new_start, new_finish);
				data_allocator::deallocate(new_start, new_size);
				throw;
			}

			//�������ͷ�ԭvector
			destroy(begin(), end());
			deallocate();

			//������������ָ����vector
			start = new_start;
			finish = new_finish;
			end_of_storage = new_start + len;
		}
	}

	template<class T, class Alloc>
	void vector<T, Alloc>::insert(iterator position, size_type n, const T &x)
	{
		if (n != 0) {
			if (size_type(end_of_storage - finish) >= n) {
				//���ÿռ���ڵ��ڡ�����Ԫ�ظ�����
				T x_copy = x;
				//���¼�������֮�������Ԫ�ظ���
				const size_type elems_after = finish - position;
				iterator old_finish = finish;
				if (elems_after > n) {
					//�������֮�������Ԫ�ظ��������ڡ�����Ԫ�ظ�����
					uninitialized_fill_n(finish - n, finish, finish);
					finish += n;
					copy_backward(position, old_finish - n, old_finish);
					fill(position, position + n, x_copy);//�Ӳ���㿪ʼ������ֵ
				}
				else {
					//�������֮�������Ԫ�ظ�����С�ڵ��ڡ�����Ԫ�ظ�����
					uninitialized_fill_n(finish, n - elems_after, x_copy);
					finish += n - elems_after;
					uninitialized_copy(position, old_finish, finish);
					finish += elems_after;
					fill(position, old_finish, x_copy);
				}
			
			}
			else {
				//���ÿռ�С�ڡ�����Ԫ�ظ�����
				//���Ⱦ����³��ȣ��ɳ��ȵ���������ɳ���+����Ԫ�ظ���
				const size_type old_size = size();
				const size_type len = old_size + max(old_size, n);
				//���������µ�vector�ռ�
				iterator new_start = data_allocator::allocate(len);
				iterator new_finish = new_start;
				try {
					//�������Ƚ���vector�Ĳ����֮ǰ��Ԫ�ظ��Ƶ��¿ռ�
					new_finish = uninitialized_copy(start, position, new_start);
					//�����ٽ�����Ԫ�أ���ֵ��Ϊn�������¿ռ�
					new_finish = uninitialized_fill_n(new_finish, n, x);
					//�����ٽ���vector�Ĳ����֮���Ԫ�ظ��Ƶ��¿ռ�
					new_finish = uninitialized_copy(position, finish, new_finish);
				}
				catch (...) {
					destroy(new_start, new_finish);
					data_allocator::deallocate(new_start, len);
					throw;
				}
				//����������ͷžɵ�vector
				destroy(start, finish);
				deallocate();
				//���µ���ˮλ���
				start = new_start;
				finish = new_finish;
				end_of_storage = new_start + n;
			}
		}
	}
}