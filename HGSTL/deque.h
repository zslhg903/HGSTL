#ifndef _DEQUE_H_
#define _DEQUE_H_
#include"dequeIterator.h"
#include"allocator.h"
#include"construct.h"
namespace HGSTL {
	enum { _min_map_num = 8 };
	template<class T,class Alloc=alloc,size_t BufSize=0>
	class deque {
	public:
		typedef T value_type;
		typedef value_type* pointer;
		typedef value_type& reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef const reference const_reference;

		typedef __deque_iterator<T, T&, T*, BufSize> iterator;

	protected:
		typedef pointer* map_pointer;

		iterator start;//表示第一个节点
		iterator finish;//表示最后一个节点

		map_pointer map;//指向map,map是块连续空间
		size_type map_size;//map内有多少指针
	protected:
		typedef simple_alloc<value_type, Alloc> data_allocator;
		typedef simple_alloc<pointer, Alloc>	map_allocator;
	public:
		deque() :start(), finish(), map_size(0), map(0)
		{
			create_map_and_nodes(0);
		}
		deque(int n, const value_type& value) :start(), finish(), map(0), map_size(0)
		{
			fill_initialize(n, value);
		}
		~deque()
		{
			destroy(start, finish);
			destroy_map_and_nodes();
		}
	public:
		iterator begin() { return start; }
		iterator end() { return finish; }
		reference operator[](size_type n)
		{
			return start[difference_type(n)];
		}

		reference front() { return *start; }
		reference back()
		{
			iterator tmp = finish;
			--tmp;	//调用__deque_iterator<>::operator--
			return *tmp;//调用__deque_iterator<>::operator*
		}
		size_type size() const { return finish - start; }

		size_type max_size() const { return size_type(-1); }
		bool empty() const { return finish == start; }
		void clear();
		

		void push_back(const value_type& t)
		{
			if (finish.cur != finish.last - 1)//最后缓冲区尚有两个（含）以上的元素备用空间
			{
				construct(finish.cur, t);//直接在备用空间上构造元素
				++finish.cur;	//调整最后缓冲区的使用状态
			}
			else
				push_back_aux(t);
		}
		void push_front(const value_type& t)
		{
			if (start.cur != start.first)
			{
				construct(start.cur - 1, t);
				--start.cur;
			}
			else
				push_front_aux(t);
		}

		void pop_back()
		{
			if (finish.cur != finish.first)
			{
				--finish.cur;
				destroy(finish.cur);
			}
			else
				pop_back_aux();
		} 

		void pop_front()
		{
			if (start.cur != start.last - 1)
			{
				destroy(start.cur);
				++start.cur;
			}
			else
				pop_front_aux();
		}
		iterator erase(iterator pos)
		{
			iterator next = pos;
			++next;
			difference_type index = pos - start;
			if (index < (size() >> 1))
			{
				copy_backward(start, pos, next);
				pop_front();
			}
			else
			{
				copy(next, finish, pos);
				pop_back();
			}
			return start + index;
		}
		iterator erase(iterator first, iterator last);

		iterator insert(iterator position, const value_type& x)
		{
			if (position.cur == start.cur)
			{
				push_front(x);
				return start;
			}
			else if (position.cur == finish.cur)
			{
				push_back(x);
				iterator tmp = finish;
				--tmp;
				return tmp;
			}
			else
			{
				return insert_aux(position, x);
			}
		}

	protected:
		void fill_initialize(size_type, const value_type&);
		void create_map_and_nodes(size_type);
		void push_back_aux(const value_type&);
		void push_front_aux(const value_type&);
		void reallocate_map(size_type nodes_to_add, bool add_at_front);
		void pop_back_aux(); //finish.cur == finish.first 释放该缓冲区
		void pop_front_aux();
		void destroy_map_and_nodes();
		
		
		iterator insert_aux(iterator pos, const value_type& x);

		static size_t buffer_size() { return __deque_buf_size(BufSize, sizeof(T)); }
		static size_t __deque_buf_size(size_t n, size_t sz) 
		{
			return n != 0 ? n : (sz < 512 ? size_t(512 / sz) : size_t(1));
		}
		int initial_map_size() 
		{
			return _min_map_num;
		}
		pointer allocate_node() 
		{
			return data_allocator::allocate(buffer_size());
		}
		void deallocate_node(pointer n)
		{
			data_allocator::deallocate(n, buffer_size());
		}
		void reserve_map_at_back(size_type nodes_to_add = 1)
		{
			if (nodes_to_add > map_size - (finish.node + 1 - map))
				reallocate_map(nodes_to_add, false);
		}
		void reserve_map_at_front(size_type nodes_to_add = 1)
		{
			if (nodes_to_add > start.node - map)
				reallocate_map(nodes_to_add, true);
		}
		
	};
}

#include"deque.impl.h"
#endif // !_DEQUE_H_

