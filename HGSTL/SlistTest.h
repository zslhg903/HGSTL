#ifndef _SLIST_TEST_H_
#define _SLIST_TEST_H_
#include"slist.h"
#include<iostream>
#include<cassert>
#include"algorithm.h"
namespace HGSTL {
	namespace SlistTest {
		void testCase1()
		{
			slist<int> islist;
			islist.push_front(9);
			islist.push_front(1);
			islist.push_front(2);
			islist.push_front(3);
			islist.push_front(4);
			assert(islist.size() == 5);

			
		}
		void testAllCases()
		{
			testCase1();
			std::cout << "SlistTest is OK" << std::endl;
		}
	}
}
#endif // !_SLIST_TEST_H_

