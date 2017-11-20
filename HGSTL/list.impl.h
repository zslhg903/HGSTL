#ifndef _LIST_IMPL_H_
#define _LIST_IMPL_H_
#include "list.h"
namespace HGSTL {
	template<class T,class Alloc>
	void list<T, Alloc>::clear()
	{
		link_type cur = (link_type)node->next;
		while (cur != node) {
			link_type tmp = cur;
			cur = (link_type)cur->next;
			destroy_node(tmp);
		}
		//恢复node原始状态
		node->next = node;
		node->prev = node;
	}

	template<class T,class Alloc>
	void list<T, Alloc>::remove(const T& value)
	{
		iterator first = begin();
		iterator last = end();
		while (first != last) {
			iterator next = first;
			++next;
			if (*first == value) erase(first);
			first = next;
		}
	}
	template<class T,class Alloc>
	void list<T,Alloc>::unique()
	{
		iterator first = begin();
		iterator last = end();
		if (first == last) return;
		iterator next = first;
		while (++next != last) {
			if (*first == *next)
				erase(next);
			else
				first = next;
			next = first;
		}
	}
	template<class T,class Alloc>
	void list<T, Alloc>::merge(list<T, Alloc>& x) 
	{
		iterator first1 = begin();
		iterator last1 = end();
		iterator first2 = x.begin();
		iterator last2 = x.end();

		//注意：前提是，两个lists都已经过递增排序
		while (first != last1&&first2 != last2)
		{
			if (*first2 < *first1)
			{
				iterator next = first2;
				transfer(first1, first2, ++next);
				first2 = next;
			}
			else
				++first1;
			if (first2 != last2) transfer(last1, first2, last2);
		}
	}

	template<class T,class Alloc>
	void list<T, Alloc>::reverse()
	{
		if (node->next == node || link_type(node->next)->next == node) return;
		iterator first = begin();
		++first;
		while (first != end()) {
			iterator old = first;
			++first;
			transfer(begin(), old, first);
		}
	}

	template<class T,class Alloc>
	void list<T, Alloc>::sort()
	{
		if (node->next == node || link_type(node->next)->next == node) return;
		list<T, Alloc> carry;
		list<T, Alloc> counter[64];
		int fill = 0;
		while (!empty())
		{
			carry.splice(carry.begin(), *this, begin());
			int i = 0;
			while (i < fill && !counter[i].empty())
			{
				counter[i].merge(carry);
				carry.swap(counter[i++]);
			}
			carry.swap(counter[i]);
			if (i == fill) ++fill;
		}
		for (int i = 1; i < fill; i++)
			counter[i].merge(counter[i - 1]);
	}

}
#endif // !_LIST_IMPL_H_
