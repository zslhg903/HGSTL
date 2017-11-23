#ifndef _DEQUEITERATOR_H_
#define _DEQUEITERATOR_H_
#include "iterator.h"
namespace HGSTL {
	template<class T,class Ref,class Ptr,size_t BufSiz>
	struct __deque_iterator {
		typedef __deque_iterator<T, T&, T*, BufSiz> iterator;
		typedef __deque_iterator<T, const T&, const T*, BufSiz> const_iterator;
		static size_t buffer_size() { return __deque_buf_size(BufSiz, sizeof(T)); }

		typedef random_access_iterator_tag iterator_category;
		typedef T value_type;
		typedef Ptr pointer;
		typedef Ref reference;
		typedef ptrdiff_t difference_type;

		typedef size_t size_type;
		typedef T** map_pointer;

		typedef __deque_iterator self;

		T* cur;	//�˵�������ָ֮�������е����У�current��Ԫ�أ�curָ����һ��Ԫ�أ�
		T* first;	//�˵�������ָ֮��������ͷ
		T* last;	//�˵�������ָ֮��������β�������ÿռ䣩
		map_pointer node;	//ָ��ܿ�����

		//���n��Ϊ0������n����ʾbuffer size���û��Զ���
		//���nΪ0����ʾbuffer sizeʹ��Ĭ��ֵ����ô 
		//	���sz��Ԫ�ش�С��sizeof(value_type)��С��512������512/sz
		//	���sz��С��512������1
		static size_t __deque_buf_size(size_t n, size_t sz)
		{
			return n != 0 ? n : (sz < 512 ? size_t(512 / sz) : size_t(1));
		}

		//����������
		void set_node(map_pointer new_node)
		{
			node = new_node;
			first = *new_node;
			last = first + difference_type(buffer_size());
		}

		reference operator*() const { return *cur; }
		pointer operator->() const { return &(operator*()); }
		difference_type operator-(const self& x) const
		{
			return difference_type(buffer_size())*(node - x.node - 1) +
				(cur - first) + (x.last - x.cur);
		}
		//++i
		self& operator++()
		{
			++cur;
			if (cur == last)
			{
				set_node(node + 1);
				cur = first;
			}
			return *this;
		}
		//i++
		self operator++(int)
		{
			self tmp = *this;
			++*this;
			return tmp;
		}
		//--i
		self& operator--()
		{
			if (cur == first)
			{
				set_node(node - 1);
				cur = last;
			}
			--cur;
			return *this;
		}
		//i--
		self operator--(int)
		{
			self tmp = *this;
			--*this;//����--i
			return tmp;
		}
		//ʵ�������ȡ������������ֱ����Ծn������
		self& operator+=(difference_type n)
		{
			difference_type offset = n + (cur - first);
			if (offset >= 0 && offset < difference_type(buffer_size()))
				//Ŀ��λ����ͬһ��������
				cur += n;
			else
			{
				//���λ�ò���ͬһ������
				difference_type node_offset =
					offset > 0 ? offset / difference_type(buffer_size())
					: -difference_type((-offset - 1) / buffer_size()) - 1;
				//�л�����ȷ�Ľڵ㣨�༴��������
				set_node(node + node_offset);
				//�л�����ȷ��Ԫ��
				cur = first + (offset - node_offset*difference_type(buffer_size()));
			}
			return *this;
		}

		self operator+(difference_type n) const
		{
			self tmp = *this;
			return tmp += n;
		}

		self& operator-=(difference_type n) { return *this += -n; }

		self operator-(difference_type n) const
		{
			self tmp = *this;
			return tmp -= n;
		}
		//����ʵ�������ȡ������������ֱ����Ծn������
		reference operator[](difference_type n) const { return *(*this + n); }

		bool operator==(const self& x) const { return cur == x.cur; }
		bool operator!=(const self& x) const { return !(*this == x); }
		bool operator<(const self& x) const 
		{
			return (node == x.node) ? (cur < x.cur) : (node < x.node);
		}

	};
}
#endif // !_DEQUEITERATOR_H_

