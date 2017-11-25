#include"DequeTest.h"
namespace HGSTL {
	namespace DequeTest {
		void testCase1()
		{
			tsDeque<int> tsD1(20, 9);
			stdDeque<int> stdD1(20, 9);

			for (int i = 0; i < tsD1.size(); i++)
				tsD1[i] = i;
			for (int i = 0; i < stdD1.size(); i++)
				stdD1[i] = i;

			assert(HGSTL::Test::container_equal(tsD1, stdD1));
			//test push_back();
			for (int i = 0; i < 3; i++)
				tsD1.push_back(i);
			for (int i = 0; i < 3; i++)
				stdD1.push_back(i);
			assert(HGSTL::Test::container_equal(tsD1, stdD1));

			//test push_front();
			tsD1.push_front(99);
			stdD1.push_front(99);
			assert(tsD1[0] == stdD1[0]);

			tsD1.push_front(97);
			stdD1.push_front(97);
			assert(tsD1[0] == stdD1[0]);

			tsD1.push_front(96);
			stdD1.push_front(96);
			assert(tsD1[0] == stdD1[0]);

			//test pop_front()
			tsD1.pop_front();
			stdD1.pop_front();
			assert(tsD1[0] == stdD1[0]);

			tsD1.pop_front();
			stdD1.pop_front();
			assert(tsD1[0] == stdD1[0]);

			tsD1.pop_front();
			stdD1.pop_front();
			assert(tsD1[0] == stdD1[0]);

			//test pop_back()
			tsD1.pop_back();
			stdD1.pop_back();
			assert(tsD1[tsD1.size() - 1] == stdD1[stdD1.size() - 1]);

			tsD1.pop_back();
			stdD1.pop_back();
			assert(tsD1[tsD1.size() - 1] == stdD1[stdD1.size() - 1]);

			tsD1.pop_back();
			stdD1.pop_back();
			assert(tsD1[tsD1.size() - 1] == stdD1[stdD1.size() - 1]);

			//test clear();
			tsD1.clear();
			stdD1.clear();
			assert(tsD1.size() == stdD1.size());

			
		}
		void testCase2()
		{
			tsDeque<int> tsD1(10,0);
			stdDeque<int> stdD1(10,0);
			
			for (int i = 1; i <= 10; i++)
			{
				tsD1[i - 1] = i;
				stdD1[i - 1] = i;
			}
			tsDeque<int>::iterator it1 = find(tsD1.begin(), tsD1.end(), 1);
			stdDeque<int>::iterator it2 = std::find(stdD1.begin(), stdD1.end(), 1);
			//test erase();
			tsD1.erase(it1);
			stdD1.erase(it2);
			assert(HGSTL::Test::container_equal(tsD1, stdD1));

			tsDeque<int>::iterator it3 = find(tsD1.begin(), tsD1.end(), 2);
			stdDeque<int>::iterator it4 = std::find(stdD1.begin(), stdD1.end(), 2);
			//test erase(,)
			auto it5 = it3 + 2;
			auto it6 = it4 + 2;

			tsD1.erase(it3, it5);
			stdD1.erase(it4, it6);
			assert(HGSTL::Test::container_equal(tsD1, stdD1));

			//test insert()
			tsDeque<int>::iterator it7 = find(tsD1.begin(), tsD1.end(), 10);
			stdDeque<int>::iterator it8 = std::find(stdD1.begin(), stdD1.end(), 10);

			tsD1.insert(it7, 111);
			stdD1.insert(it8, 111);
			assert(HGSTL::Test::container_equal(tsD1, stdD1));
		}


		void testAllCases()
		{
			testCase1();
			testCase2();
			std::cout << "deque OK" << std::endl;
		}
	}
}