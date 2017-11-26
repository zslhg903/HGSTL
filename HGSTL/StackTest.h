#ifndef _STACKTEST_H_
#define _STACKTEST_H_
#include<list>
#include<stack>
#include<deque>
#include<cassert>
#include"deque.h"
#include"stack.h"
#include"list.h"
#include"TestUtil.h"

namespace HGSTL {
	namespace StackTest {
		template<class T>
		using tsStackList = HGSTL::stack<T,HGSTL::list<T>>;
		template<class T>
		using stdStackList = std::stack<T, std::list<T>>;

		template<class T>
		using tsStackDeque = HGSTL::stack<T, HGSTL::deque<T>>;
		template<class T>
		using stdStackDeque = std::stack<T, std::deque<T>>;

		void testCase1();
		void testCase2();
		void testAllCases();
	}
}
#endif // !_STACKTEST_H_

