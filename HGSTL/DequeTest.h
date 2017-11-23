#ifndef _DequeTest_H_
#define _DequeTest_H_
#include<deque>
#include"deque.h"
#include<cassert>
#include"TestUtil.h"
namespace HGSTL {
	namespace DequeTest {
		template<class T>
		using stdDeque = std::deque<T>;
		template<class T>
		using tsDeque = HGSTL::deque<T>;
		void testCase1();

		void testAllCases();
	}
}
#endif // !_DequeTest_H_
