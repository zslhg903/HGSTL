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
		typedef value_type*	iterator;//vector�ĵ�����������ָͨ��
		typedef value_type& reference;
		typedef size_t		size_type;
		typedef ptrdiff_t	difference_type;
	protected:
		typedef simple_alloc<value_type, Alloc> data_allocator;
		iterator start;
		iterator finish;
		iterator end_of_storage;

		void insert_aux(iterator position, const T& x);
		
		void insert(iterator position, size_type n, const T& x);
	
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
				insert_aux(end(), x);//���ޱ��ÿռ䣬��Ҫ���·���
		}

		void pop_back() {
			--finish;
			destroy(finish);
		}

		iterator erase(iterator position) {
			if (position + 1 != end())
				copy(position + 1, finish, position);//����Ԫ����ǰ�ƶ�
			--finish;
			destroy(finish);
			return position;
		}
		//���[first,last)�е�����Ԫ��
		iterator erase(iterator first, iterator last) {
			iterator i = copy(last, finish, first);
			destroy(i, finish);
			finish = finish - (last - first);
			return first;
		}

		void resize(size_type new_size, const T& x) {
			if (new_size < size())
				erase(begin() + new_size, end());//������Ԫ���Ƴ�
			else
				insert(end(), new_size - size(), x);
		}
		void resize(size_type new_size) { resize(new_size, T()); }
		void clear() { erase(begin(), end()); }

	protected:
		//���ÿռ䲢��������
		iterator allocate_and_fill(size_type n, const T& x) {
			iterator result = data_allocator::allocate(n);
			HGSTL::uninitialized_fill_n(result, n, x);
			return result;
		}
		
		// ����ռ䲢copyһ�������Ԫ�ص��¿ռ䴦  
		template <typename ForwardIterator>
		iterator allocate_and_copy(size_type n, ForwardIterator first, ForwardIterator last)
		{
			iterator result = data_allocator.allocate(n);
			uninitialized_copy(first, last, result);
			return result;
		}




	};


}
#include "vector.impl.h"
#endif // !_VECTOR_H_

