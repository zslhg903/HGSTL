#ifndef _LIST_H_
#define _LIST_H_
#include"listIterator.h"
#include "algorithm.h"
#include "construct.h"
#include "allocator.h"
#include<iostream>
namespace HGSTL {
	template<class T,class Alloc=alloc>
	class list {
	protected:
		typedef __list_node<T> list_node;
		typedef simple_alloc<list_node, Alloc> list_node_allocator;
	public:
		typedef T value_type;
		typedef value_type* pointer;
		typedef value_type& reference;

		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef list_node* link_type;//�ڵ�ָ��
		typedef __list_iterator<T, T&, T*>   iterator;
	protected:
		link_type node;//ֻҪһ��ָ�룬��ɱ�ʾ������״˫������
	public:
		
		iterator begin() { return (link_type)((*node).next); }
		iterator end() { return node; }
		bool empty()const { return node->next == node; }
		size_type size()const {
			size_type result = 0;
			result = distance(begin(), end());
			return result;
		}
		reference front() { return *begin(); }
		reference back() { return *(--end()); }

		list() { empty_initialize(); }//����һ��������
		
		void push_front(const T& x) { insert(begin(), x);}
		void push_back(const T& x) { insert(end(), x); }

		void pop_front() { erase(begin()); }
		void pop_back(){
			iterator tmp = end();
			erase(--tmp);
		}

		

		iterator insert(iterator position, const T& x) {
			link_type tmp = create_node(x);
			tmp->next = position.node;
			tmp->prev = position.node->prev;
			(link_type(position.node->prev))->next = tmp;
			position.node->prev = tmp;
			return tmp;
		}
		iterator erase(iterator position) {
			link_type next_node = link_type(position.node->next);
			link_type prev_node = link_type(position.node->prev);
			prev_node->next = next_node;
			next_node->prev = prev_node;
			destroy_node(position.node);
			return iterator(next_node);
		}

	protected:
		//����һ���ڵ㲢����
		link_type get_node() { return list_node_allocator::allocate(); }
		//�ͷ�һ���ڵ�
		void put_node(link_type p) { list_node_allocator::deaalocate(p); }

		//���������ò����죩һ���ڵ㣬����Ԫ��ֵ
		link_type create_node(const T& x) {
			link_type p = get_node();
			construct(&p->data, x);
			return p;
		}
		//���٣��������ͷţ�һ���ڵ�
		void destroy_node(link_type p) {
			destroy(&p->data);
			put_node(p);
		}
		void empty_initialize() {
			node = get_node();//����һ���ڵ�ռ䣬��nodeָ����
			node->next = node;//��nodeͷβ��ָ���Լ�������Ԫ��ֵ
			node->prev = node;
		}

		//��[first,last)�ڵ�����Ԫ���ƶ���position֮ǰ
		void transfer(iterator position, iterator first, iterator last) {
			if (position != last) {
				(*(link_type((*last.node).prev))).next = position.node;
				(*(link_type((*first.node).prev))).next = last.node;
				(*(link_type((*position.node).prev))).next = first.node;
				link_type tmp = link_type((*position.node).prev);
				(*position.node).prev = (*last.node).prev;
				(*last.node).prev = (*first.node).prev;
				(*first.node).prev = tmp;
			}
		}
		void splice(iterator position,list& x){
			if (!x.empty())
				transfer(position, x.begin(), x.end());
		}
		void splice(iterator position, list&, iterator i) {
			iterator j = i;
			++j;
			if (position == i || position == j) return;
			transfer(position, i, j);
		}

		void splice(iterator position, list&, iterator first, iterator last) {
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
