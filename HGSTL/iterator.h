#ifndef _ITERATOR_H_
#define	_ITERATOR_H_
#include<cstddef>
namespace HGSTL {
	struct input_iterator_tag {};
	struct output_iterator_tag{};
	struct forward_iterator_tag:public input_iterator_tag{};//������Է��ʳ��˸���˽�г�Ա���ڵ����г�Ա
	struct bidirectional_iterator_tag:public forward_iterator_tag{};
	struct random_access_iterator_tag:public bidirectional_iterator_tag{};
	//������ǰ��ֻ��
	template<class T,class Distance>
	struct input_iterator 
	{
		typedef input_iterator_tag  iterator_category;
		typedef T					value_type;
		typedef Distance			difference_type;
		typedef T*					pointer;
		typedef T&					reference;
	};
	//������ǰ��Ψд
	template<class T,class Distance>
	struct output_iterator 
	{
		typedef output_iterator_tag	iterator_category;
		typedef void				value_type;
		typedef void				difference_type;
		typedef void				pointer;
		typedef void				reference;
	};
	//������ǰ����д
	template<class T,class Distance>
	struct forward_iterator 
	{
		typedef forward_iterator_tag	iterator_category;
		typedef T						value_type;
		typedef	Distance				difference_type;
		typedef T*						pointer;
		typedef T&						reference;
	};
	//����ǰ�󣬶�д
	template<class T,class Distance>
	struct bidirectional_iterator 
	{
		typedef bidirectional_iterator_tag	iterator_category;
		typedef T							value_type;
		typedef Distance					difference_type;
		typedef T*							pointer;
		typedef T&							reference;
	};
	//�ಽǰ�󣬶�д
	template<class T,class Distance>
	struct random_access_iterator 
	{
		typedef random_access_iterator_tag iterator_category;
		typedef T							value_type;
		typedef Distance					difference_type;
		typedef T*							pointer;
		typedef T&							reference;
	};

	template<class Category,class T,class Distance=ptrdiff_t,
	class Pointer=T*,class Reference=T&>
	struct iterator 
	{
		typedef Category	iterator_category;
		typedef T			value_type;
		typedef Distance	difference_type;
		typedef Pointer		pointer;
		typedef	Reference	reference;
	};
	
	template<class Iterator>
	struct iterator_traits 
	{
		typedef typename Iterator::iterator_category	iterator_category;
		typedef typename Iterator::value_type			value_type;
		typedef typename Iterator::difference_type		difference_type;
		typedef	typename Iterator::pointer				pointer;
		typedef typename Iterator::reference			reference;
	};//һ��Ҫ����typename����ΪIterator��һ��template���������������������ֻ�֮ǰ��
	//��������Iteratorһ����Ϥ��

	//TΪԭ��ָ��
	template<class T>
	struct iterator_traits<T*> 
	{
		typedef random_access_iterator_tag	iterator_category;
		typedef T							value_type;
		typedef ptrdiff_t					difference_type;
		typedef T*							pointer;
		typedef T&							reference;
	};
	//ƫ�ػ��桪�����������Ǹ�pointer-to-constʱ
	//��ȡ�������ͱ�Ӧ����T����const T
	template<class T>
	struct iterator_traits<const T*>
	{
		typedef random_access_iterator_tag	iterator_category;
		typedef T							value_type;
		typedef ptrdiff_t					difference_type;
		typedef T*							pointer;
		typedef T&							reference;
	};
	//����iterator���ͣ�
	template<class Iterator>
	inline typename iterator_traits<Iterator>::iterator_category
		iterator_category(const Iterator& It) 
	{
		typedef typename iterator_traits<Iterator>::iterator_category category;
		return category();
	}

	//��ȡvalue_type
	template<class Iterator>
	inline typename iterator_traits<Iterator>::value_type*
		value_type(const Iterator& It)
	{
		return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
	}
	//��ȡdistance_type
	template<class Iterator>
	inline typename iterator_traits<Iterator>::difference_type*
		distance_type(const Iterator& It)
	{
		return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
	}
	//distance
	template<class InputIterator>
	inline typename iterator_traits<InputIterator>::difference_type
		distance(InputIterator first, InputIterator last)
	{
		typedef typename iterator_traits<InputIterator>::iterator_category category;
		return __distance(first, last, category());
	}
	template<class InputIterator>
	inline typename iterator_traits<InputIterator>::difference_type
		__distance(InputIterator first, InputIterator last, input_iterator_tag)
	{
		iterator_traits<InputIterator>::difference_type n = 0;
		while (first != last)
		{
			++first;
			++n;
		}
		return n;
	}

	template<class RandomAccessIterator>
	inline typename iterator_traits<RandomAccessIterator>::difference_type
		__distance(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag)
	{
		return last - first;
	}
}
#endif // !_ITERATOR_H_

