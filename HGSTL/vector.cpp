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
			//以上配置原则：如果原大小为0，则配置1（个元素大小）
			//如果原大小不为0，则配置原大小的两倍
			//前半段用来放置原数据，后半段准备用来放置新数据


			iterator new_start = data_allocator::allocate(len);
			iterator new_finish = new_start;
			try {
				//将原vector的内容拷贝到新vector
				new_finish = uninitialized_copy(start, position, new_start);
				construct(new_finish, x);  //为新元素设定初值x
				new_finish++;//调整水位
				//将安插点的原内容也拷贝过来
				new_finish = uninitialized_copy(position, finish, new_finish);
			}
			catch(...){
				//出现异常，回滚
				destroy(new_start, new_finish);
				data_allocator::deallocate(new_start, new_size);
				throw;
			}

			//析构并释放原vector
			destroy(begin(), end());
			deallocate();

			//调整迭代器，指向新vector
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
				//备用空间大于等于“新增元素个数”
				T x_copy = x;
				//以下计算插入点之后的现有元素个数
				const size_type elems_after = finish - position;
				iterator old_finish = finish;
				if (elems_after > n) {
					//“插入点之后的现有元素个数”大于“新增元素个数”
					uninitialized_fill_n(finish - n, finish, finish);
					finish += n;
					copy_backward(position, old_finish - n, old_finish);
					fill(position, position + n, x_copy);//从插入点开始填入新值
				}
				else {
					//“插入点之后的现有元素个数”小于等于“新增元素个数”
					uninitialized_fill_n(finish, n - elems_after, x_copy);
					finish += n - elems_after;
					uninitialized_copy(position, old_finish, finish);
					finish += elems_after;
					fill(position, old_finish, x_copy);
				}
			
			}
			else {
				//备用空间小于“新增元素个数”
				//首先决定新长度：旧长度的两倍，或旧长度+新增元素个数
				const size_type old_size = size();
				const size_type len = old_size + max(old_size, n);
				//以下配置新的vector空间
				iterator new_start = data_allocator::allocate(len);
				iterator new_finish = new_start;
				try {
					//以下首先将旧vector的插入点之前的元素复制到新空间
					new_finish = uninitialized_copy(start, position, new_start);
					//以下再将新增元素（初值皆为n）填入新空间
					new_finish = uninitialized_fill_n(new_finish, n, x);
					//以下再将旧vector的插入点之后的元素复制到新空间
					new_finish = uninitialized_copy(position, finish, new_finish);
				}
				catch (...) {
					destroy(new_start, new_finish);
					data_allocator::deallocate(new_start, len);
					throw;
				}
				//以下清除并释放旧的vector
				destroy(start, finish);
				deallocate();
				//以下调整水位标记
				start = new_start;
				finish = new_finish;
				end_of_storage = new_start + n;
			}
		}
	}
}