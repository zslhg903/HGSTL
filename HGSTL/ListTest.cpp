#include"ListTest.h"
namespace HGSTL {
	namespace ListTest {
		void testCase1()
		{
			tsList<int> tslist1;
			stdList<int> stdlist1;
			assert(stdlist1.size() == tslist1.size());
			//test push_back()
			for (int i = 0; i < 10; i++)
			{
				tslist1.push_back(i);
				stdlist1.push_back(i);
			}
			assert(HGSTL::Test::container_equal(tslist1, stdlist1));
			//test insert
			auto it1 = HGSTL::find(tslist1.begin(), tslist1.end(), 3);
			auto it2 = std::find(stdlist1.begin(), stdlist1.end(), 3);

			tslist1.insert(it1, 20);
			stdlist1.insert(it2, 20);
			assert(HGSTL::Test::container_equal(tslist1, stdlist1));

			//test erase()
			tslist1.erase(it1);
			stdlist1.erase(it2);
			assert(HGSTL::Test::container_equal(tslist1, stdlist1));

			
		}

		void testCase2()
		{
			tsList<int> tslist1;
			stdList<int> stdlist1;

			for (int i = 0; i < 10; i++)
			{
				tslist1.push_back(1);
				stdlist1.push_back(1);
			}
			for (int i = 0; i < 12; i++)
			{
				tslist1.push_back(2);
				stdlist1.push_back(2);
			}

			auto tslist2 = tslist1;
			auto stdlist2 = stdlist1;

			//test remove
			tslist1.remove(1);
			stdlist1.remove(1);
			assert(HGSTL::Test::container_equal(tslist1, stdlist1));

			//test clear()
			tslist1.clear();
			stdlist1.clear();
			assert(tslist1.size() == stdlist1.size());

			//test unique
			tslist2.unique();
			stdlist2.unique();
			assert(HGSTL::Test::container_equal(tslist2, stdlist2));

		}
		void testCase3()
		{
			tsList<int> tslist1;
			stdList<int> stdlist1;
			
			for (int i = 0; i < 10; i++)
			{
				tslist1.push_back(i);
				stdlist1.push_back(i);
			}
			auto it1 = HGSTL::find(tslist1.begin(), tslist1.end(), 3);
			auto it2 = std::find(stdlist1.begin(), stdlist1.end(), 3);

			//test splice(iterator,list&)
			tsList<int> tsTmp;
			stdList<int> stdTmp;
			for (int i = 0; i < 10; i++)
			{
				tsTmp.push_back(i);
				tsTmp.push_back(i);
			}
			tslist1.splice(it1, tsTmp);
			stdlist1.splice(it2, stdTmp);
			assert(HGSTL::Test::container_equal(tslist1, stdlist1));

			//test splice(iterator,list&,iterator)
			auto it3= HGSTL::find(tslist1.begin(), tslist1.end(), 6);
			auto it4 = std::find(stdlist1.begin(), stdlist1.end(), 6);
			tslist1.splice(it1, tslist1, it3);
			stdlist1.splice(it2, stdlist1, it4);
			assert(HGSTL::Test::container_equal(tslist1, stdlist1));

			//test splice(iterator,list&,iterator,iterator)
			it3 = it1 + 1;
			it4 = it1 + 1;
			auto it5 = ++it3;
			auto it6 = ++it4;
			tslist1.splice(it1, tslist1, it3, it5);
			stdlist1.splice(it2, stdlist1,it4, it6);

			//test merge()
			tslist1.clear();
			stdlist1.clear();
			tsTmp.clear();
			stdTmp.clear();
			for (int i = 0; i < 10; i++)
			{
				tslist1.push_back(i);
				stdlist1.push_back(i);
			}
			for (int i = 10; i < 20; i++)
			{
				tsTmp.push_back(i);
				tsTmp.push_back(i);
			}
			tslist1.merge(tsTmp);
			stdlist1.merge(stdTmp);
			assert(HGSTL::Test::container_equal(tslist1, stdlist1));

			//test reverse()
			tslist1.reverse();
			stdlist1.reverse();
			assert(HGSTL::Test::container_equal(tslist1, stdlist1));

			//test sort()
			tslist1.sort();
			stdlist1.sort();
			assert(HGSTL::Test::container_equal(tslist1, stdlist1));

		}
		void testAllCases()
		{
			testCase1();
			testCase2();
			testCase3();
		}
	}
}