#ifndef _SLIST_IMPL_H_
#define _SLIST_IMPL_H_
#include"allocator.h"
#include"construct.h"
namespace HGSTL {
	template<class T,class Alloc=HGSTL::alloc>
	class slist
	{
	public:
		typedef T value_type;
		typedef value_type* pointer;
		typedef const value_type* const_pointer;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;

		typedef __slist_iterator<T, T&, T*> iterator;
		typedef __slist_iterator<T,const T&,const T*> const_iterator;
	private:
		typedef __slist_node<T> list_node;
		typedef __slist_node_base list_node_base;
		typedef __slist_iterator_base iterator_base;
		typedef simple_alloc<list_node, Alloc> list_node_allocator;

		static list_node* create_node(const value_type& x)
		{
			list_node* node = list_node_allocator::allocate();
			construct(&node->data, x);
			node->next = 0;
			return node;
		}

		static void destroy_node(list_node* node)
		{
			destroy(&node->data);
			list_node_allocator::deallocate(node);
		}
	private:
		list_node_base head;
	public:
		slist() { head.next = 0; }
		~slist() { /*clear();*/ }
	public:
		iterator begin() { return iterator((list_node*)head.next); }
		iterator end() { return iterator(0); }
		size_type size()const { return __slist_size(head.next); }
		bool empty()const { return head.next == 0; }

		void swap(slist& L)
		{
			list_node_base* tmp = head.next;
			head.next = L.head.next;
			L.head.next = tmp;
		}
		reference front() { return ((list_node*)head.next)->data; }
		void push_front(const value_type& x)
		{
			__slist_make_link(&head, create_node(x));
		}
		void pop_front()
		{
			list_node* node = (list_node*)head.next;
			head.next = node->next;
			destroy_node(node);
		}
		/*void clear()
		{
			list_node* cur = (list_node*)head;
			while (cur != NULL)
			{
				list_node* next = (list_node*)cur->next;
				destroy_node(cur);
				cur = next;
			}
			head = NULL;
		}*/
	};
}
#endif // !_SLIST_IMPL_H_


