#ifndef _LISTITERATOR_H_
#define _LISTITERATOR_H_
#include "iterator.h"
#include "listNode.h"
namespace HGSTL {
	template<class T,class Ref,class Ptr>
	struct __list_iterator {
		typedef __list_iterator<T, T&, T*> iterator;
		typedef __list_iterator<T, Ref, Ptr> self;

		typedef bidirectional_iterator_tag iterator_catagory;
		typedef T value_type;
		typedef Ptr pointer;
		typedef Ref reference;
		typedef __list_node<T>* link_type;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;

		link_type node;//�������ڲ���ȻҪ��һ����ָͨ�룬ָ��list�Ľڵ�

		//constructor
		__list_iterator(link_type x):node(x){}
		__list_iterator(){}
		__list_iterator(const iterator& x):node(x.node){}

		bool operator==(const self& x)const { return node == x.node; }
		bool operator!=(const self& x)const { return !(node == x.node); }
		//���¶Ե�����ȡֵ(dereference),ȡ���ǽڵ������ֵ
		reference operator*()const { return (*node).data; }

		//�����ǵ������ĳ�Ա��ȡ��member access�������ӵı�׼����
		pointer operator->()const { return &(operator*()); }

		//�Ե������ۼ�1������ǰ��һ���ڵ�
		self& operator++() {
			node = (link_type)((*node).next);
			return *this;
		}
		self operator++(int) {
			self tmp = *this;
			++*this;
			return tmp;
		}
		//�Ե������ݼ�1�����Ǻ���һ���ڵ�
		self& operator--() {
			node = (link_type)((*node).prev);
			return *this;
		}
		self operator--(int) {
			self tmp = *this;
			--*this;
			return tmp;
		}
	};
}

#endif // !_LISTITERATOR_H_
