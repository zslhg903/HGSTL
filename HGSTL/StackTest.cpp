#include"StackTest.h"
namespace HGSTL {
	namespace StackTest {
		void testCase1()
		{
			tsStackList<int> ts1;
			stdStackList<int> std1;
			//test push()
			for (int i = 0; i < 5; i++)
			{
				ts1.push(i);
				std1.push(i);
			}
			//test pop() and top()
			for (int i = 0; i < 5; i++,ts1.pop(),std1.pop())
			{
				assert(ts1.top() == std1.top());
				//std::cout << "ts1.front=" << ts1.top() << " and std1.front=" << std1.top() << std::endl;
			}
			


		}
		void testCase2()
		{
			
			tsStackDeque<int> ts1;
			stdStackDeque<int> std1;
			for (int i = 0; i < 5; i++)
			{
				ts1.push(i);
				std1.push(i);
			}
			//test pop() and top()
			for (int i = 0; i < 5; i++, ts1.pop(), std1.pop())
			{
				assert(ts1.top() == std1.top());
			}
		}

		void testAllCases()
		{
			testCase1();
			testCase2();
			std::cout << "StackTest is OK" << std::endl;
		}
	}
}