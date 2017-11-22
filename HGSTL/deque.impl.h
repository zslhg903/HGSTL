#ifndef _DEQUE_IMPL_H_
#define _DEQUE_IMPL_H_
#include"uninitialized.h"
#include"deque.h"
namespace HGSTL {
	template<class T,class Alloc,size_t BufSize>
	void deque<T, Alloc, BufSize>::fill_initialize(size_type n, const reference value)
	{
		create_map_and_nodes(n);
		map_pointer cur;
		for (cur = start.node; cur < finish.node; ++cur)
			uninitialized_fill(*cur, *cur + buffer_size(), value);
		uninitialized_fill(finish.first, finish.cur, value);
	}

	template<class T, class Alloc, size_t BufSize>
	void deque<T, Alloc, BufSize>::create_map_and_nodes(size_type num_elements)
	{
		size_type num_nodes = num_elements / buffer_size() + 1;

		map_size = max(initial_map_size(), num_nodes + 2);
		map = map_allocator::allocate(map_size);

		map_pointer nstart = map + (map_size - num_nodes) / 2;
		map_pointer nfinish = nstart + num_nodes - 1;

		map_pointer cur;
		for (cur = nstart; cur <= nfinish; ++cur)
			*cur = allocate_node();

		start.set_node(nstart);
		finish.set_node(nfinish);
		start.cur = start.first;
		finish.cur = finish.first + num_elements%buffer_size();
	}
	template<class T, class Alloc, size_t BufSize>
	void deque<T, Alloc, BufSize>::push_back_aux(const reference t)
	{
		value_type t_copy = t;
		reserve_map_at_back();
		*(finish.node + 1) = allocate_node();

		construct(finish.cur, t_copy);
		finish.set_node(finish.node + 1);
		finish.cur = finish.first;
	}
	template<class T, class Alloc, size_t BufSize>
	void deque<T, Alloc, BufSize>::push_front_aux(const reference t)
	{
		value_type t_copy = t;
		reserve_map_at_front();
		*(start.node - 1) = allocate_node();
		
		start.set_node(start.node - 1);
		start.cur = start.last - 1;
		construct(start.cur, t_copy);
	}
	template<class T, class Alloc, size_t BufSize>
	void deque<T, Alloc, BufSize>::reallocate_map(size_type nodes_to_add, bool add_at_front)
	{
		size_type old_num_nodes = finish.node - start.node + 1;
		size_type new_num_nodes = old_num_nodes + nodes_to_add;

		map_pointer new_nstart;
		if (map_size > 2 * new_num_nodes)
		{
			new_nstart = map + (map_size - new_num_nodes) / 2 + (add_at_front ? nodes_to_add : 0);
			if (new_nstart < start.node)
				copy(start.node, finish.node + 1, new_nstart);
			else
				copy_backward(start.node, finish.node + 1, new_nstart + old_num_nodes);
		}
		else
		{
			size_type new_map_size = map_size + max(map_size, nodes_to_add) + 2;

			map_pointer new_map = map_allocator::allocate(new_map_size);
			new_nstart new_map + (new_map_size - new_num_nodes) / 2 + (add_at_front ? nodes_to_add : 0);

			copy(start.node, finish.node + 1, new_nstart);
			map_allocator::deallocate(map, map_size);

			map = new_map;
			map_size = new_map_size();

			start.set_node(new_nstart);
			finish.set_node(new_nstart + old_num_nodes - 1);
		}
	}
	template<class T, class Alloc, size_t BufSize>
	void deque<T, Alloc, BufSize>::pop_back_aux()
	{
		deallocate_node(finish.first);
		finish.set_node(finish.node - 1);
		finish.cur = finish.last - 1;
		destroy(finish.cur);
	}
	template<class T, class Alloc, size_t BufSize>
	void deque<T, Alloc, BufSize>::pop_front_aux()
	{
		destroy(start.cur);

		deallocate_node(start.first);
		start.set_node(start.node + 1);
		start.cur = start.first;
	}
	template<class T, class Alloc, size_t BufSize>
	void deque<T, Alloc, BufSize>::clear()
	{
		for (map_pointer node = start.node + 1; node < finish.node; ++node)
		{
			destroy(*node, *node + buffer_size());
			data_allocator::deallocate(*node, buffer_size());
		}

		if (start.node != finish.node)
		{
			destroy(start.cur, start.last);
			destroy(finish.first, finish.cur);
			data_allocator::deallocate(finish.first, buffer_size());
		}
		else
		{
			destroy(start.cur, finish.cur);
		}
		finish = start;
	}
	template<class T, class Alloc, size_t BufSize>
	typename deque<T, Alloc, BufSize>::iterator deque<T, Alloc, BufSize>::erase(iterator first, iterator last)
	{
		if (first == start&&last == finish)
		{
			clear();
			return finish;
		}
		else
		{
			difference_type n = last - first;
			difference_type elems_before = first - start;
			if (elems_before < (size() - n / 2))
			{
				copy_backward(start, first, last);
				iterator new_start = start + n;
				destroy(start, new_start);
				for (map_pointer cur = start.node; cur < new_start.node; ++cur)
					data_allocator::deallocate(*cur, buffer_size());
				start = new_start;
			}
			else
			{
				copy(last, finish, first);
				iterator new_finish = finish - n;
				destroy(new_finish, finish);
				for (map_pointer cur = new_finish.node + 1; cur <= finish.node; ++cur)
					data_allocator::deallocate(*cur, buffer_size());
				finish = new_finish;
			}
			return start + elems_before;
		}
	}

	template<class T,class Alloc,size_t BufSize>
	typename deque<T,Alloc,BufSize>::iterator 
		deque<T, Alloc, BufSize>::insert_aux(iterator pos, const reference x)
	{
		difference_type index = pos - start;
		value_type x_copy = x;
		if (index < size() / 2)
		{
			push_front(front());
			iterator front1 = start;
			++front1;
			iterator front2 = front1;
			++front2;
			pos = start + index;
			iterator pos1 = pos;
			++pos1;
			copy(front2, pos1, front1);
		}
		else
		{
			push_back(back());
			iterator back1 = finish;
			--back1;
			iterator back2 = back1;
			--back2;
			pos = start + index;
			copy_backward(pos, back2, back1);
		}
		*pos = x_copy;
		return pos;
	}
}

#endif // !_DEQUE_IMPL_H_

