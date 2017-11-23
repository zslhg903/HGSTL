#include <iostream>
#include"VectorTest.h"
#include"ListTest.h"
#include"DequeTest.h"
int main() {
	HGSTL::VectorTest::testAllCases();
	HGSTL::ListTest::testAllCases();
	HGSTL::DequeTest::testAllCases();
	system("pause");
}