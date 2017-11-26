#include"HeapTest.h"
namespace HGSTL {
	namespace HeapTest {
		void testCase1()
		{
			int ia[9] = { 0,1,2,3,4,8,9,3,5 };
			vector<int> ivec1(ia, ia + 9);
			vector<int> ivec2(ia, ia + 9);
			//*************[make_heap]***************
			HGSTL::make_heap(ivec1.begin(), ivec1.end());
			std::make_heap(ivec2.begin(), ivec2.end());
			for (int i = 0; i < ivec1.size(); i++)
			{
				assert(ivec1[i] == ivec2[i]);
			}
			//*************[push_heap]***************
			ivec1.push_back(7);
			ivec2.push_back(7);
			HGSTL::push_heap(ivec1.begin(), ivec1.end());
			std::push_heap(ivec2.begin(), ivec2.end());
			for (int i = 0; i < ivec1.size(); i++)
			{
				assert(ivec1[i] == ivec2[i]);
			}
			//*************[pop_heap]***************
			HGSTL::pop_heap(ivec1.begin(), ivec1.end());
			std::pop_heap(ivec2.begin(), ivec2.end());
			ivec1.pop_back();
			ivec2.pop_back();
			for (int i = 0; i < ivec1.size(); i++)
			{
				assert(ivec1[i] == ivec2[i]);
			}
			//*************[sort_heap]***************
			HGSTL::sort_heap(ivec1.begin(), ivec1.end());
			std::sort_heap(ivec2.begin(), ivec2.end());
			for (int i = 0; i < ivec1.size(); i++)
			{
				assert(ivec1[i] == ivec2[i]);
			}
		}
		void testAllCases()
		{
			testCase1();
			cout << "HeapTest is OK" << endl;
		}
	}
}