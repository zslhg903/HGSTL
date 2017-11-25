#ifndef _LISTTEST_H_
#define _LISTTEST_H_
#include<cassert>
#include"TestUtil.h"
#include"list.h"
#include<list>
namespace HGSTL {
	namespace ListTest {
		template<class T>
		using tsList = HGSTL::list<T>;
		template<class T>
		using stdList = std::list<T>;

		void testCase1();
		void testCase2();
		void testCase3();
		void testAllCases();
	}
}
#endif // !_LISTTEST_H_

