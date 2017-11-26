#include <iostream>
#include"VectorTest.h"
#include"ListTest.h"
#include"DequeTest.h"
#include"StackTest.h"
#include"QueueTest.h"
int main() {
	HGSTL::VectorTest::testAllCases();
	HGSTL::ListTest::testAllCases();
	HGSTL::DequeTest::testAllCases();
	HGSTL::StackTest::testAllCases();
	HGSTL::QueueTest::testAllCases();
	system("pause");
}