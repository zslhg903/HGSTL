#ifndef _LIST_H_
#define _LIST_H_
#include "listIterator.h"
#include "algorithm.h"
#include "construct.h"
#include "allocator.h"
#include <iostream>
namespace HGSTL {
	template<class T,class Alloc=alloc>
	class list {
	protected:
		typedef __list_node<T> list_node;//节点类型
		typedef simple_alloc<list_node, Alloc> list_node_allocator;
	public:
		typedef T value_type;
		typedef value_type* pointer;
		typedef value_type& reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef const reference const_reference;

		typedef list_node* link_type;//节点指针
		typedef __list_iterator<T, T&, T*>   iterator;
	protected:
		link_type node;//只要一个指针，便可表示整个环状双向链表
	public:
		list<T>& operator= (const list& l) {
			if (this != &l) {
				list(l).swap(*this);
			}
			return *this;
		}
		iterator begin()const { return (link_type)node->next; }
		iterator end()const { return node; }
		bool empty()const { return node->next == node; }
		size_type size() const
		{
			size_type result = 0;
			result=distance(begin(), end());
			return result;
		}
		reference front() { return *begin(); }//取头元素
		reference back() { return *(--end()); }//取尾元素

		list() { empty_initialize(); }//产生一个空链表

		list(const list& l):list()
		{
			for (iterator it = l.begin(); it != l.end(); it++)
				push_back(it.node->data);
		}
		~list()
		{
			clear();
			erase(end());
		}

		void push_front(const T& x) { insert(begin(), x);}
		void push_back(const T& x) { insert(end(), x); }

		void pop_front() { erase(begin()); }
		void pop_back()
		{
			iterator tmp = end();
			erase(--tmp);
		}

		
		void swap(list& x) 
		{
			link_type tmp = x.node;
			x.node = this->node;
			this->node = tmp;
		}

		//在position前插入节点，data值为x，并返回新节点的迭代器
		iterator insert(iterator position, const T& x)
		{
			link_type tmp = create_node(x);
			tmp->next = position.node;
			tmp->prev = position.node->prev;
			(link_type(position.node->prev))->next = tmp;
			position.node->prev = tmp;
			return tmp;
		}
		//擦除当前迭代器所指节点，并返回指向后一个节点的的迭代器
		iterator erase(iterator position) 
		{
			link_type next_node = link_type(position.node->next);
			link_type prev_node = link_type(position.node->prev);
			prev_node->next = next_node;
			next_node->prev = prev_node;
			destroy_node(position.node);
			return iterator(next_node);
		}

	protected:
		//配置一个节点并返回
		link_type get_node() { return list_node_allocator::allocate(); }
		//释放一个节点
		void put_node(link_type p) { list_node_allocator::deallocate(p); }

		//产生（配置并构造）一个节点，带有元素值
		link_type create_node(const T& x)
		{
			link_type p = get_node();
			construct(&p->data, x);
			return p;
		}

		//销毁（析构并释放）一个节点
		void destroy_node(link_type p)
		{
			destroy(&p->data);
			put_node(p);
		}
		void empty_initialize() 
		{
			node = get_node();//配置一个节点空间，令node指向它
			node->next = node;//令node头尾都指向自己，不设元素值
			node->prev = node;
		}

		//将[first,last)内的所有元素移动到position之前
		void transfer(iterator position, iterator first, iterator last) 
		{
			if (position != last) 
			{
				(*(link_type((*last.node).prev))).next = position.node;
				(*(link_type((*first.node).prev))).next = last.node;
				(*(link_type((*position.node).prev))).next = first.node;
				link_type tmp = link_type((*position.node).prev);
				(*position.node).prev = (*last.node).prev;
				(*last.node).prev = (*first.node).prev;
				(*first.node).prev = tmp;
			}
		}

	public:
		//将x接合于position所指位置之前，x必须不同于*this
		void splice(iterator position,list& x)
		{
			if (!x.empty())
				transfer(position, x.begin(), x.end());
		}

		//将i所指元素接合于position所指位置之前。position和i可指向同一个list
		void splice(iterator position, list&, iterator i) 
		{
			iterator j = i;
			++j;
			if (position == i || position == j) return;
			transfer(position, i, j);
		}

		//将[first,last)内的所有元素接合于position所指位置之前
		void splice(iterator position, list&, iterator first, iterator last)
		{
			if (first != last)
				transfer(position, first, last);
		}

	public:
		void clear();
		void remove(const T& value);
		void unique();
		void merge(list<T,Alloc>& x);
		void reverse();
		void sort();



	};
}
#include "list.impl.h"
#endif // _LIST_H_
