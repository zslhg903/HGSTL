#ifndef _LIST_IMPL_H_
#define _LIST_IMPL_H_
namespace HGSTL {
	//清除所有节点（整个链表）
	template<class T,class Alloc>
	void list<T, Alloc>::clear()
	{
		link_type cur = (link_type)node->next;//node->next指向头节点
		while (cur != node)
		{
			link_type tmp = cur;
			cur = (link_type)cur->next;
			destroy_node(tmp);
		}
		//恢复node原始状态
		node->next = node;
		node->prev = node;
	}

	//将数值为value的所有元素移除
	template<class T,class Alloc>
	void list<T, Alloc>::remove(const T& value)
	{
		iterator first = begin();
		iterator last = end();
		while (first != last) 
		{
			iterator next = first;
			++next;
			if (*first == value) erase(first);
			first = next;
		}
	}

	//移除数值相同的连续元素。注意，只有“连续而相同的元素”，才会被移除剩一个
	template<class T,class Alloc>
	void list<T,Alloc>::unique()
	{
		iterator first = begin();
		iterator last = end();
		if (first == last) return;
		iterator next = first;
		while (++next != last) 
		{
			if (*first == *next)
				erase(next);
			else
				first = next;
			next = first;
		}
	}
	//merge()将x合并到*this身上。两个lists的内容都必须先经过递增排序
	template<class T,class Alloc>
	void list<T, Alloc>::merge(list<T, Alloc>& x) 
	{
		iterator first1 = begin();
		iterator last1 = end();
		iterator first2 = x.begin();
		iterator last2 = x.end();

		//注意：前提是，两个lists都已经过递增排序
		while (first1 != last1&&first2 != last2)
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
	//reverse()将*this的内容逆向重置
	template<class T,class Alloc>
	void list<T, Alloc>::reverse()
	{
		//以下判断，如果是空链表，或仅有一个元素，就不进行任何操作
		//使用size()==0||size()==1来判断，虽然也可以，但是比较慢
		if (node->next == node || link_type(node->next)->next == node) return;
		iterator first = begin();
		++first;
		while (first != end()) {
			iterator old = first;
			++first;
			transfer(begin(), old, first);
		}
	}
	//list不能使用STL算法sort(),必须使用自己的sort() member function
	//因为STL算法sort()只接受RamdonAccessIterator
	//本函数采用quick sort
	template<class T,class Alloc>
	void list<T, Alloc>::sort()
	{
		//if (node->next == node || link_type(node->next)->next == node) return;

		////一些新的lists，作为中介数据存放区
		//list<T, Alloc> carry;
		//list<T, Alloc> counter[64];
		//int fill = 0;
		//while (!empty())
		//{
		//	carry.splice(carry.begin(), *this, begin());
		//	int i = 0;
		//	while (i < fill && !counter[i].empty())
		//	{
		//		counter[i].merge(carry);
		//		carry.swap(counter[i++]);
		//	}
		//	carry.swap(counter[i]);
		//	if (i == fill) ++fill;
		//}
		//for (int i = 1; i < fill; i++)
		//	counter[i].merge(counter[i - 1]);
		//swap(counter[fill - 1]);

		//这里使用的是插入排序
		if (size() == 0 || size() == 1)
			return;
		list<T, Alloc> tmp;
		iterator q = begin();
		while (!empty()) {
			iterator p = tmp.begin();
			while (p != tmp.end() && *p < *q)
				p++;
			tmp.splice(p,*this, q);
			q = begin();
		}
		//将tmp赋给本list
		swap(tmp);
	}

}
#endif // !_LIST_IMPL_H_
