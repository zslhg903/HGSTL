#include"VectorTest.h"
namespace HGSTL {
	namespace VectorTest {
		
	
		//≤‚ ‘push_back
		void testCase1() {
			tsVec<int> v1, v2;
			for (int i = 0; i != 100; ++i) {
				v1.push_back(i);
				v2.push_back(i);
			}
			assert(v1 == v2);
			assert(!(v1 != v2));
		}
		void testCase2() {
			tsVec<int> myvector;
			for (int i = 1; i <= 5; i++) myvector.push_back(i);

			auto i = 1;
			for (tsVec<int>::iterator it = myvector.begin(); it != myvector.end(); ++it, ++i) {
				assert(*it == i);
			}

			i = 1;
			
		}
		void testCase3() {
			tsVec<int> myvector(5);  
			int i = 0;
			for (int j = 0; j < myvector.size(); j++)
			{
				myvector[j]=j;
			}
			i = 0;
			for (tsVec<int>::iterator it = myvector.begin(); it != myvector.end(); ++it) {
				assert(*it == i++);
			}
		}
		void testCase4() {
			//test resize(size_type new_size)
			tsVec<int> v(11, 0);
			assert(v.size() == 11);
			v.resize(5);
			assert(v.size() == 5);
			
		}
		
		void testCase5()
		{
			tsVec<int> tsV1(10, 1);
			stdVec<int> stdV1(10, 1);
			//test size()
			assert(tsV1.size() == stdV1.size());
			assert(HGSTL::Test::container_equal(tsV1, stdV1));

			//test push_back()
			for (int i = 0; i < 10; i++)
			{
				tsV1.push_back(i);
				stdV1.push_back(i);
			}
			assert(HGSTL::Test::container_equal(tsV1, stdV1));

			//test front()  back()
			assert(tsV1.front() == stdV1.front());
			assert(tsV1.back() == stdV1.back());

			//test pop_back()
			tsV1.pop_back();
			stdV1.pop_back();
			assert(HGSTL::Test::container_equal(tsV1, stdV1));

			//test erase(iterator)
			auto it1 = HGSTL::find(tsV1.begin(), tsV1.end(), 2);
			auto it2 = std::find(stdV1.begin(), stdV1.end(), 2);
			it1=tsV1.erase(it1);
			it2=stdV1.erase(it2);
			assert(HGSTL::Test::container_equal(tsV1, stdV1));

			//test erase(iterator first, iterator last) 
			auto it1_2 = it1 + 2;
			auto it2_2 = it2 + 2;
			it1 = tsV1.erase(it1, it1_2);
			it2 = stdV1.erase(it2, it2_2);
			assert(HGSTL::Test::container_equal(tsV1, stdV1));

			//test insert(iterator position, size_type n, const T& x)
			tsV1.insert(it1, 10, 111);
			stdV1.insert(it2, 10, 111);
			assert(HGSTL::Test::container_equal(tsV1, stdV1));

			//test resize(size_type new_size, const T& x) 
			tsV1.resize(10, 10);
			stdV1.resize(10, 10);
			assert(HGSTL::Test::container_equal(tsV1, stdV1));


		}

		void testAllCases() {
			testCase1();
			testCase2();
			testCase3();
			testCase4();
			testCase5();
			std::cout << "VectorTest is OK" << std::endl;
			
		}
	}
}