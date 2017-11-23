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

		T* cur;	//此迭代器所指之缓冲区中的现行（current）元素（cur指向下一个元素）
		T* first;	//此迭代器所指之缓冲区的头
		T* last;	//此迭代器所指之缓冲区的尾（含备用空间）
		map_pointer node;	//指向管控中心

		//如果n不为0，传回n，表示buffer size由用户自定义
		//如果n为0，表示buffer size使用默认值，那么 
		//	如果sz（元素大小，sizeof(value_type)）小于512，传回512/sz
		//	如果sz不小于512，传回1
		static size_t __deque_buf_size(size_t n, size_t sz)
		{
			return n != 0 ? n : (sz < 512 ? size_t(512 / sz) : size_t(1));
		}

		//修正缓冲区
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
			--*this;//调用--i
			return tmp;
		}
		//实现随机存取。迭代器可以直接跳跃n个距离
		self& operator+=(difference_type n)
		{
			difference_type offset = n + (cur - first);
			if (offset >= 0 && offset < difference_type(buffer_size()))
				//目标位置在同一缓冲区内
				cur += n;
			else
			{
				//标的位置不在同一缓冲区
				difference_type node_offset =
					offset > 0 ? offset / difference_type(buffer_size())
					: -difference_type((-offset - 1) / buffer_size()) - 1;
				//切换至正确的节点（亦即缓冲区）
				set_node(node + node_offset);
				//切换至正确的元素
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
		//以下实现随机存取，迭代器可以直接跳跃n个距离
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

