#include"VectorTest.h"
namespace HGSTL {
	namespace VectorTest {
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
		void testAllCases() {
			testCase1();
		}
	}
}