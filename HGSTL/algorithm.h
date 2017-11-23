
#ifndef _ALGORITHM_H_
#define _ALGORITHM_H_

#include <cstring>
#include <utility>

#include "iterator.h"
#include "type_traits.h"
namespace HGSTL {
	//***** fill O(N)******
	template<class ForwardIterator, class T>
	void fill(ForwardIterator first, ForwardIterator last, const T& value) {
		for (; first != last; ++first)
			*first = value;
	}

	inline void fill(char *first, char *last, const char& value) {
		memset(first, static_cast<unsigned char>(value), last - first);
	}

	inline void fill(wchar_t *first, wchar_t *last, const wchar_t& value) {
		memset(first, static_cast<unsigned char>(value), (last - first) * sizeof(wchar_t));
	}

	//***** fill_n O(N)******
	template<class OutputIterator, class Size, class T>
	OutputIterator fill_n(OutputIterator first, Size n, const T& value) {
		for (; n > 0; --n, ++first)
			*first = value;
		return first;
	}

	template<class Size>
	char *fill_n(char *first, Size n, const char& value) {
		memset(first, static_cast<unsigned char>(value), n);
		return first + n;
	}

	template<class Size>
	wchar_t *fill_n(wchar_t *first, Size n, const wchar_t& value) {
		memset(first, static_cast<unsigned char>(value), n * sizeof(wchar_t));
		return first + n;
	}

	//********** [distance] ******************************

	//********* [Algorithm Complexity: O(N)] ****************

	template<class InputIterator>
	typename iterator_traits<InputIterator>::difference_type

		_distance(InputIterator first, InputIterator last, input_iterator_tag) {
		typename iterator_traits<InputIterator>::difference_type dist = 0;
		while (first++ != last) {
			++dist;
		}

		return dist;

	}

	template<class RandomIterator>
	typename iterator_traits<RandomIterator>::difference_type

		_distance(RandomIterator first, RandomIterator last, random_access_iterator_tag) {

		auto dist = last - first;

		return dist;

	}

	template<class Iterator>
	typename iterator_traits<Iterator>::difference_type

		distance(Iterator first, Iterator last) {
		typedef typename iterator_traits<Iterator>::iterator_category iterator_category;
		return _distance(first, last, iterator_category());

	}

	//********** [push_heap] ******************************
	template <class RandomAccessIterator, class Distance, class T>
	inline void __push_heap(RandomAccessIterator first, Distance holeIndex, Distance topIndex, T value) {
		Distance parent = (holeIndex - 1) / 2;
		while (holeIndex > topIndex && *(first + parent) < value) {
			*(first + holeIndex) = *(first + parent);//���ڵ��ֵ�½�
			holeIndex = parent;
			parent = (holeIndex - 1) / 2;  //����һ�����ڵ�λ��
		}
		*(first + holeIndex) = value; //
	}

	template <class RandomAccessIterator, class Distance, class T>
	inline void __push_heap_aux(RandomAccessIterator first, RandomAccessIterator last,
		Distance *, T*) {
		HGSTL::__push_heap(first, Distance((last - first) - 1), Distance(0), T(*(last - 1)));
	}

	template <class RandomAccessIterator>
	inline void push_heap(RandomAccessIterator first, RandomAccessIterator last) {
		//��Ԫ���Ѿ�λ�ڵײ���������β��
		HGSTL::__push_heap_aux(first, last, difference_type(first), value_type(first));
	}

	//********** [pop_heap] ******************************
	//�����ֵ�ŵ�ĩβ
	template <class RandomAccessIterator, class T, class Distance>
	void __adjust_heap(RandomAccessIterator first, Distance holeIndex,
		Distance len, T value) {
		Distance topIndex = holeIndex;
		Distance secondChild = 2 * holeIndex + 2;
		while (secondChild < len) {
			//�Ƚ϶������Һ��ӣ�Ȼ����secondChild ����ϴ�ڵ�
			if (*(first + secondChild) < *(first + (secondChild - 1)))
				secondChild--;
			//�ϴ�ĺ�����Ϊ��ֵ�����������ϴ��ӽڵ㴦
			*(first + holeIndex) = *(first + secondChild);
			holeIndex = secondChild;
			secondChild = 2 * (secondChild + 1);
		}
		if (secondChild == len) {//û���ҽڵ�ֻ����ڵ�
			*(first + holeIndex) = *(first + (secondChild - 1));
			holeIndex = secondChild - 1;
		}
		HGSTL::__push_heap(first, holeIndex, topIndex, value);
	}

	template <class RandomAccessIterator, class T, class Distance>
	inline void __pop_heap(RandomAccessIterator first, RandomAccessIterator last,
		RandomAccessIterator result, T value, Distance*) {
		*result = *first;
		HGSTL::__adjust_heap(first, Distance(0), Distance(last - first), value);//value ��ʱ����
	}

	template<class RandomAccessIterator, class T>
	inline void __pop_heap_aux(RandomAccessIterator first, RandomAccessIterator last, T*) {
		HGSTL::__pop_heap(first, last - 1, last - 1, T(*(last - 1)), difference_type(first));
	}

	template<class RandomAccessIterator>
	inline void pop_heap(RandomAccessIterator first, RandomAccessIterator last) {
		HGSTL::__pop_heap_aux(first, last, value_type(first));
	}

	//********** [pop_heap] ******************************
	template<class RandomIterator, class T, class Distance>
	void __make_heap(RandomIterator first,
		RandomIterator last, T*, Distance*) {
		if (last - first<2) return;
		Distance len = last - first;
		Distance parent = (len - 2) / 2;
		while (true) {//���������������и��ڵ�
			HGSTL::__adjust_heap(first, parent, len, T(*(first + parent)));
			if (parent == 0) return;
			parent--;
		}
	}

	template<class RandomIterator>
	inline void make_heap(RandomIterator first, RandomIterator last) {
		HGSTL::__make_heap(first, last, value_type(first), difference_type(first));
	}


	//********** [advance] ******************************
	//********* [Algorithm Complexity: O(N)] ****************
	template<class InputIterator, class Distance>
	void _advance(InputIterator& it, Distance n, input_iterator_tag) {
		assert(n >= 0);
		while (n--) {
			++it;
		}
	}
	template<class BidirectionIterator, class Distance>
	void _advance(BidirectionIterator& it, Distance n, bidirectional_iterator_tag) {
		if (n < 0) {
			while (n++) {
				--it;
			}
		}
		else {
			while (n--) {
				++it;
			}
		}
	}
	template<class RandomIterator, class Distance>
	void _advance(RandomIterator& it, Distance n, random_access_iterator_tag) {
		if (n < 0) {
			it -= (-n);
		}
		else {
			it += n;
		}
	}

	template <class InputIterator, class Distance>
	void advance(InputIterator& it, Distance n) {
		typedef typename iterator_traits<InputIterator>::iterator_category iterator_category;
		_advance(it, n, iterator_category());
	}

	//********** [copy] ******************************

	//********* [Algorithm Complexity: O(N)] ****************

	template<class InputIterator, class OutputIterator>//p321
	OutputIterator __copy(InputIterator first, InputIterator last, OutputIterator result, __true_type) {

		auto dist = distance(first, last);
		memcpy(result, first, sizeof(*first) * dist);
		advance(result, dist);
		return result;

	}

	template<class InputIterator, class OutputIterator>
	OutputIterator __copy(InputIterator first, InputIterator last, OutputIterator result, __false_type) {

		while (first != last) {
			*result = *first;
			++result;
			++first;
		}

		return result;

	}

	template<class InputIterator, class OutputIterator, class T>
	OutputIterator _copy(InputIterator first, InputIterator last, OutputIterator result, T*) {
		typedef typename __type_traits<T>::is_POD_type is_pod;
		return __copy(first, last, result, is_pod());

	}
	//��ȫ�����汾
	template <class InputIterator, class OutputIterator>
	inline OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result) {

		//return _copy(first, last, result, value_type(first));
		return __copy_dispath<InputIterator, OutputIterator>()(first, last, result);
	}

	//����汾��1����������ʽ
	inline char* copy(const char* first, const char* last, char* result)
	{
		memmove(result, first, last - first);
		return result + (last - first);
	}
	//����汾��2����������ʽ
	inline wchar_t* copy(const wchar_t* first, const wchar_t* last, wchar_t* result)
	{
		memmove(result, first, sizeof(wchar_t)*(last - first));
		return result + (last - first);
	}
	//
	template<class InputIterator,class OutputIterator>
	struct __copy_dispath
	{
		OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result)
		{
			return __copy(first, last, result, iterator_category(first));
		}
	};
	//ƫ�ػ��汾��1����������������T*ָ����ʽ
	template<class T>
	struct __copy_dispatch<T*, T*>
	{
		T* operator()(T* first, T* last, T* result)
		{
			typedef typename __type_traits<T>::has_trivial_assignment_operator t;
			return __copy_t(first, last, result, t());
		}
	};
	//ƫ�ػ��汾��2������һ������Ϊconst T*ָ����ʽ���ڶ�����ΪT*ָ����ʽ
	template<class T>
	struct __copy_dispath<const T*, T*>
	{
		T* operator()(const T* frist, const T* last, T* result)
		{
			typedef typename __type_traits<T>::has_trivial_assignment_operator t;
			return __copy_t(first, last, result, t());
		}
	};

	template<class InputIterator,class OutputIterator>
	inline OutputIterator __copy(InputIterator first, InputIterator last, OutputIterator result, input_iterator_tag)
	{
		for (; first != last; ++result, ++first)
			*result = *first;
		return result;
	}

	template<class RandomAccessIterator,class OutputIterator>
	inline OutputIterator __copy(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, random_access_iterator_tag)
	{
		return __copy_d(first, last, result, distance_type(first));
	}

	template<class RandomAccessIterator,class OutputIterator,class Distance>
	inline OutputIterator __copy_d(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, Distance *)
	{
		for (Distance n = last - first; n > 0; --n, ++result, ++first)
			*result = *first;
		return result;
	}

	template<class T>
	inline T* __copy_t(const T* first, const T* last, T* result, __true_type)
	{
		memmove(result, first, sizeof(T)*(last - first));
		return result + (last - first);
	}

	template<class T>
	inline T* __copy_t(const T* first, const T* last, T* result, __false_type)
	{
		return __copy_d(first, last, result, (ptrdiff_t*)0);
	}

	//////////////
	/*template <class InputIterator, class OutputIterator>
	OutputIterator copy_backward(InputIterator first, InputIterator last, OutputIterator result) {

		OutputIterator new_result = result - (last - first);
		return _copy(first, last, new_result, value_type(first));

	}

	template<>
	inline char *copy(char *first, char *last, char *result) {

		auto dist = last - first;
		memcpy(result, first, sizeof(*first) * dist);
		return result + dist;

	}

	template<>
	inline wchar_t *copy(wchar_t *first, wchar_t *last, wchar_t *result) {

		auto dist = last - first;
		memcpy(result, first, sizeof(*first) * dist);
		return result + dist;

	}*/
	template <class T1, class T2>
	T1 max(T1 a, T2 b) {
		return a > b ? a : b;
	}

	template <class T1, class T2>
	T1 min(T1 a, T2 b) {
		return a < b ? a : b;
	}

	//********** [find] *************************
	//********* [Algorithm Complexity: O(N)] ****************
	template <class InputIterator, class T>
	InputIterator find(InputIterator first, InputIterator last, const T& val) {
		for (; first != last; ++first) {
			if (*first == val)
				break;
		}
		return first;
	}
}

#endif