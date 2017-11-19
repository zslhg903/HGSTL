#include"VectorTest.h"
namespace HGSTL {
	namespace VectorTest {
		//ππ‘Ï∫Ø ˝≤‚ ‘
		void testCase1() {
			stdVec<std::string> v1(10, "zxh");
			tsVec<std::string> v2(10, "zxh");
			assert(HGSTL::Test::container_equal(v1, v2));

			stdVec<std::string> v3(10);
			tsVec<std::string> v4(10);
			assert(HGSTL::Test::container_equal(v3, v4));

			/*std::array<std::string, 3> arr = { "abc", "def", "ghi" };
			stdVec<std::string> v5(std::begin(arr), std::end(arr));
			tsVec<std::string> v6(std::begin(arr), std::end(arr));
			assert(HGSTL::Test::container_equal(v5, v6));*/

		}
		void testCase2() {
			stdVec<int> temp1(10, 1);
			tsVec<int> temp2(10, 1);

			auto v1(temp1);
			auto v2(temp2);
			
			assert(HGSTL::Test::container_equal(v1, v2));

			auto v3(std::move(temp1));
			auto v4(std::move(temp2));
			assert(HGSTL::Test::container_equal(v3, v4));

			auto v5 = v1;
			auto v6 = v2;
			assert(HGSTL::Test::container_equal(v5, v6));

			auto v7 = std::move(v3);
			auto v8 = std::move(v4);
			assert(HGSTL::Test::container_equal(v7, v8));
		}
		//≤‚ ‘push_back
		void testCase3() {
			tsVec<int> v1, v2;
			for (int i = 0; i != 100; ++i) {
				v1.push_back(i);
				v2.push_back(i);
			}
			assert(v1 == v2);
			assert(!(v1 != v2));
		}
		void testCase4() {
			tsVec<int> myvector;
			for (int i = 1; i <= 5; i++) myvector.push_back(i);

			auto i = 1;
			for (tsVec<int>::iterator it = myvector.begin(); it != myvector.end(); ++it, ++i) {
				assert(*it == i);
			}

			i = 1;
			
		}
		void testCase5() {
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
		void testCase6() {
			tsVec<int> v(11, 0);
			assert(v.size() == 11);

			v.resize(5);
			assert(v.size() == 5);

			v.resize(20);
			assert(v.size() == 20);
		}
		

		void testAllCases() {
			testCase1();
			testCase2();
			testCase3();
			testCase4();
			testCase5();
			testCase6();
			
			
		}
	}
}