#include"QueueTest.h"
namespace HGSTL {
	namespace QueueTest {
		void testCase1()
		{
			tsQueueList<int> ts1;
			stdQueueList<int> std1;
			for (int i = 0; i < 5; i++)
			{
				ts1.push(i);
				std1.push(i);
			}
			//test push()
			assert(ts1.size() == std1.size());
			//test front() and back()
			assert(ts1.front() == std1.front());
			assert(ts1.back() == std1.back());
			for (int i = 0; i < 5; i++, ts1.pop(), std1.pop())
			{\
				assert(ts1.front() == std1.front());
			}
			


		}
		void testCase2()
		{

			tsQueueDeque<int> ts1;
			stdQueueDeque<int> std1;
			for (int i = 0; i < 5; i++)
			{
				ts1.push(i);
				std1.push(i);
			}
			//test push()
			assert(ts1.size() == std1.size());
			//test front() and back()
			assert(ts1.front() == std1.front());
			assert(ts1.back() == std1.back());
			for (int i = 0; i < 5; i++, ts1.pop(), std1.pop())
			{
				assert(ts1.front() == std1.front());
			}
		}

		void testAllCases()
		{
			testCase1();
			testCase2();
			std::cout << "QueueTest is OK" << std::endl;
		}
	}
}