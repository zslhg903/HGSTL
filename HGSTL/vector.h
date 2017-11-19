#ifndef _VECTOR_H_
#define _VECTOR_H_
#include<algorithm>
#include<type_traits>
#include<iostream>
#include<initializer_list>

#include"allocator.h"
#include"algorithm.h"
#include"uninitialized.h"
namespace HGSTL {
	template<class T,class Alloc=alloc>
	class vector {
	public:
		typedef T			value_type;
		typedef value_type*	pointer;
		typedef value_type*	iterator;//vector的迭代器就是普通指针
		typedef value_type& reference;
		typedef size_t		size_type;
		typedef ptrdiff_t	difference_type;
	protected:
		typedef simple_alloc<value_type, Alloc> data_allocator;
		iterator start;
		iterator finish;
		iterator end_of_storage;

		void insert_aux(iterator position, const T& x)
		{
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
				catch (...) {
					//出现异常，回滚
					destroy(new_start, new_finish);
					data_allocator::deallocate(new_start, len);
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
		void insert(iterator position, size_type n, const T& x)
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
						uninitialized_copy(finish - n, finish, finish);
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
		void deallocate() {
			if (start)
				data_allocator::deallocate(start, end_of_storage - start);
		}
		void fill_initialize(size_type n, const T& value) {
			start = allocate_and_fill(n, value);
			finish = start + n;
			end_of_storage = finish;
		}
	public:
		iterator begin() const { return start; }
		iterator end() const{ return finish; }
		size_type size() const { return size_type(finish - start); }
		size_type capacity() const { return size_type(end_of_storage - start); }
		bool empty() const {return begin() == end();}
		reference operator[](size_type n) { return *(begin() + n); }
		bool operator==(const vector& other) const {
			if (size() != other.size()) return false;
			auto first1 = begin(), last1 = end();
			auto first2 = other.begin(), last2 = other.end();
			for (; first1 != last1&&first2 != last2; first1++, first2++) {
				if (*first1 != *first2)
					return false;
			}
			return true;
		}
		bool operator!=(const vector& other) const {
			return !(*this == other);
		}



		vector():start(0),finish(0),end_of_storage(0){}
		vector(size_type n, const T& value) { fill_initialize(n, value); }
		vector(int n, const T& value) { fill_initialize(n, value); }
		vector(long n, const T& value) { fill_initialize(n, value); }
		explicit vector(size_type n) { fill_initialize(n, T()); }

		~vector() {
			destroy(start, finish);
			deallocate();
		}

		void swap(vector<T, Alloc> &x)
		{
			using std::swap;
			swap(start, x.start);
			swap(finish, x.finish);
			swap(end_of_storage, x.end_of_storage);
		}
		reference front() { return *begin(); }
		reference back() { return *(end() - 1); }
		void push_back(const T &x) {
			if (finish != end_of_storage) {
				construct(finish, x);
				++finish;
			}
			else
				insert_aux(end(), x);//已无备用空间，需要重新分配
		}

		void pop_back() {
			--finish;
			destroy(finish);
		}

		iterator erase(iterator position) {
			if (position + 1 != end())
				copy(position + 1, finish, position);//后续元素往前移动
			--finish;
			destroy(finish);
			return position;
		}
		//清除[first,last)中的所有元素
		iterator erase(iterator first, iterator last) {
			iterator i = copy(last, finish, first);
			destroy(i, finish);
			finish = finish - (last - first);
			return first;
		}

		void resize(size_type new_size, const T& x) {
			if (new_size < size())
				erase(begin() + new_size, end());//将后续元素移除
			else
				insert(end(), new_size - size(), x);
		}
		void resize(size_type new_size) { resize(new_size, T()); }
		void clear() { erase(begin(), end()); }

	protected:
		//配置空间并填满内容
		iterator allocate_and_fill(size_type n, const T& x) {
			iterator result = data_allocator::allocate(n);
			HGSTL::uninitialized_fill_n(result, n, x);
			return result;
		}
		
		// 分配空间并copy一个区间的元素到新空间处  
		template <typename ForwardIterator>
		iterator allocate_and_copy(size_type n, ForwardIterator first, ForwardIterator last)
		{
			iterator result = data_allocator.allocate(n);
			uninitialized_copy(first, last, result);
			return result;
		}




	};


}
#endif // !_VECTOR_H_

