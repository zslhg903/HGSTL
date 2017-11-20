#ifndef _UNINITIALIZED_H_
#define	_UNINITIALIZED_H_
#include "algorithm.h"
#include "construct.h"
namespace HGSTL {
	//POD版本的未初始化的copy
	template<class InputIterator,class ForwardIterator>
	ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last,
		ForwardIterator result, __true_type) 
	{
		memcpy(result, first, (last - first) * sizeof(*first));
		return result + (last - first);
	}
	//非POD版本的未初始化copy
	template<class InputIterator,class ForwardIterator>
	ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last,
		ForwardIterator result, __false_type) 
	{
		ForwardIterator cur = result;
		for (; first != last; ++first, ++cur)
			construct(&*cur, *first);
		return cur;
	}
	template<class InputIterator,class ForwardIterator>
	ForwardIterator uninitialized_copy(InputIterator first, InputIterator last,
		ForwardIterator result)
	{
		return __uninitialized_copy(first, last, result, value_type(result));
	}
	template<class InputIterator, class ForwardIterator,class T>
	ForwardIterator __uninitialized_copy(InputIterator first, InputIterator last,
		ForwardIterator result,T*) 
	{
		typedef typename __type_traits<T>::is_POD_type is_POD;
		return __uninitialized_copy_aux(first, last, result, is_POD());
	}




	//未初始化的copy，以某一特定值初始化
	template<class ForwardIterator,class T>
	void __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last,
		const T& value, __true_type)
	{
		fill(first, last, value);
	}
	template<class ForwardIterator,class T>
	void __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last,
		const T& value, __false_type) 
	{
		for (; first != last; first++)
		{
			construct(&*first, value);
		}
	}
	template<class ForwardIterator,class T>
	void uninitialized_fill(ForwardIterator first, ForwardIterator last,
		const T& value) 
	{
		return __uninitialized_fill(first, last, value,value_type(first));
	}
	template<class ForwardIterator, class T,class T1>
	void __uninitialized_fill(ForwardIterator first, ForwardIterator last,
		const T& value,T1*)
	{
		typedef typename __type_traits<T>::is_POD_type is_POD;
		return __uninitialized_fill_aux(first, last, value, is_POD());
	}




	template<class ForwardIterator,class Size,class T>
	ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first, Size n,
		const T& x, __true_type) 
	{
		return fill_n(first, n, x);
	}
	template<class ForwardIterator, class Size, class T>
	ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first, Size n,
		const T& x, __false_type) 
	{
		ForwardIterator cur = first;
		for (; n > 0; n--, cur++)
			construct(&*cur, x);
		return cur;
	}
	template <class ForwardIterator, class Size, class T>
	inline ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n,
		const T& x) 
	{
		return __uninitialized_fill_n(first, n, x, value_type(first));
	}
	template <class ForwardIterator, class Size, class T,class T1>
	inline ForwardIterator __uninitialized_fill_n(ForwardIterator first, Size n,
		const T& x,T1*)
	{
		typedef typename __type_traits<T1>::is_POD_type is_POD;
		return __uninitialized_fill_n_aux(first, n, x, is_POD());
	}

}
#endif // !_UNINITIALIZED_H_
