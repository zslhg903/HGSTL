#include"PriorityQueueTest.h"
namespace HGSTL {
	namespace PriorityQueueTest {
		void testCase1()
		{
			int ia[9] = { 0,1,2,3,4,8,9,3,5 };
			priority_queue<int> ipq1(ia, ia + 9);
			std::priority_queue<int> ipq2(ia, ia + 9);
			assert(ipq1.size() == ipq2.size());

			while (!ipq1.empty())
			{
				assert(ipq1.top() == ipq2.top());
				ipq1.pop();
				ipq2.pop();
			}
		}
		void testAllCases()
		{
			testCase1();
			std::cout << "PriorityQueueTest is OK" << std::endl;
		}
	}
}