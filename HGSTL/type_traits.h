#ifndef _TYPE_TRAITS_H_
#define _TYPE_TRAITS_H_
namespace HGSTL {
	struct __true_type {};
	struct __false_type{};

	template<class type>
	struct __type_traits {
		typedef __true_type 
	};
}

#endif // !_TYPE_TRAITS_H_

