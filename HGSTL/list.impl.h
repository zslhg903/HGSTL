#ifndef _LIST_IMPL_H_
#define _LIST_IMPL_H_
namespace HGSTL {
	//������нڵ㣨��������
	template<class T,class Alloc>
	void list<T, Alloc>::clear()
	{
		link_type cur = (link_type)node->next;//node->nextָ��ͷ�ڵ�
		while (cur != node)
		{
			link_type tmp = cur;
			cur = (link_type)cur->next;
			destroy_node(tmp);
		}
		//�ָ�nodeԭʼ״̬
		node->next = node;
		node->prev = node;
	}

	//����ֵΪvalue������Ԫ���Ƴ�
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

	//�Ƴ���ֵ��ͬ������Ԫ�ء�ע�⣬ֻ�С���������ͬ��Ԫ�ء����Żᱻ�Ƴ�ʣһ��
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
	//merge()��x�ϲ���*this���ϡ�����lists�����ݶ������Ⱦ�����������
	template<class T,class Alloc>
	void list<T, Alloc>::merge(list<T, Alloc>& x) 
	{
		iterator first1 = begin();
		iterator last1 = end();
		iterator first2 = x.begin();
		iterator last2 = x.end();

		//ע�⣺ǰ���ǣ�����lists���Ѿ�����������
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
	//reverse()��*this��������������
	template<class T,class Alloc>
	void list<T, Alloc>::reverse()
	{
		//�����жϣ�����ǿ����������һ��Ԫ�أ��Ͳ������κβ���
		//ʹ��size()==0||size()==1���жϣ���ȻҲ���ԣ����ǱȽ���
		if (node->next == node || link_type(node->next)->next == node) return;
		iterator first = begin();
		++first;
		while (first != end()) {
			iterator old = first;
			++first;
			transfer(begin(), old, first);
		}
	}
	//list����ʹ��STL�㷨sort(),����ʹ���Լ���sort() member function
	//��ΪSTL�㷨sort()ֻ����RamdonAccessIterator
	//����������quick sort
	template<class T,class Alloc>
	void list<T, Alloc>::sort()
	{
		//if (node->next == node || link_type(node->next)->next == node) return;

		////һЩ�µ�lists����Ϊ�н����ݴ����
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

		//����ʹ�õ��ǲ�������
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
		//��tmp������list
		swap(tmp);
	}

}
#endif // !_LIST_IMPL_H_
