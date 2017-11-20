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

		link_type node;//迭代器内部当然要有一个普通指针，指向list的节点

		//constructor
		__list_iterator(link_type x):node(x){}
		__list_iterator(){}
		__list_iterator(const iterator& x):node(x.node){}

		bool operator==(const self& x)const { return node == x.node; }
		bool operator!=(const self& x)const { return !(node == x.node); }
		//以下对迭代器取值(dereference),取的是节点的数据值
		reference operator*()const { return (*node).data; }

		//以下是迭代器的成员存取（member access）运算子的标准做法
		pointer operator->()const { return &(operator*()); }

		//对迭代器累加1，就是前进一个节点
		self& operator++() {
			node = (link_type)((*node).next);
			return *this;
		}
		self operator++(int) {
			self tmp = *this;
			++*this;
			return tmp;
		}
		//对迭代器递减1，就是后退一个节点
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
