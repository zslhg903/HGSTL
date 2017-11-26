#ifndef _VECTOR_TEST_H_
#define _VECTOR_TEST_H_
#include "vector.h"
#include "TestUtil.h"
#include<vector>
#include <array>
#include <cassert>
#include<iostream>
#include <iterator>
#include <string>
namespace HGSTL {
	namespace VectorTest {
		template<class T>
		using stdVec = std::vector<T>;

		template<class T>
		using tsVec = HGSTL::vector<T>;
		void testCase1();
		void testCase2();
		void testCase3();
		void testCase4();
		void testCase5();
		void testAllCases();
	}
}

#endif
