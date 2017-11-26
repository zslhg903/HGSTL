#ifndef _QUEUETEST_H_
#define _QUEUETEST_H_
#include<list>
#include<queue>
#include<deque>
#include<cassert>
#include"deque.h"
#include"queue.h"
#include"list.h"
#include"TestUtil.h"

namespace HGSTL {
	namespace QueueTest {
		template<class T>
		using tsQueueList = HGSTL::queue<T, HGSTL::list<T>>;
		template<class T>
		using stdQueueList = std::queue<T, std::list<T>>;

		template<class T>
		using tsQueueDeque = HGSTL::queue<T, HGSTL::deque<T>>;
		template<class T>
		using stdQueueDeque = std::queue<T, std::deque<T>>;

		void testCase1();
		void testCase2();
		void testAllCases();
	}
}
#endif // !_QUEUE_H_

