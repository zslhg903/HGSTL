#ifndef _CONSTRUCT_H_
#define _CONSTRUCT_H_
#include<new>
#include "type_traits.h"
namespace HGSTL {
	template<class T1,class T2>
	inline void construct(T1 *p, const T2& value) {
		new (p) T1(value);
	}
	template<class T>
	inline void destroy(T* pointer) {
		pointer->~T();
	}

	template<class ForwardIterator>
	inline void destroy(ForwardIterator first, ForwardIterator last) {
		__destroy(first, last, value_type(first));
	}

	template<class ForwardIterator,class T>
	inline void __destroy(ForwardIterator first, ForwardIterator last, T*)
	{
		typedef typename __type_traits<T>::has_trivial_destructor trivial_destructor;
		__destroy_aux(first, last, trivial_destructor());
	}
	//���Ԫ�ص���ֵ�ͱ���non-trivial destructor
	template<class ForwardIterator>
	inline void
		__destroy_aux(ForwardIterator first, ForwardIterator last, __false_type) {
		for (; first < last; ++first)
			destroy(&*first);
	}
	//���Ԫ�ص���ֵ�ͱ���trivial destructor
	template<class ForwardIterator>
	inline void __destroy_aux(ForwardIterator, ForwardIterator, __true_type) {}

	//���char* wchar_t*���ػ��汾
	inline void destroy(char*,char*){}
	inline void destroy(wchar_t*,wchar_t*){}
}
#endif // !_CONSTRUCT_H_

