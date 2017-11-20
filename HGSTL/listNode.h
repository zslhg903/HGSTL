#ifndef _LISTNODE_H_
#define _LISTNODE_H_
template<class T>
struct __list_node {
	typedef void* void_pointer;
	void_pointer prev;
	void_pointer next;
	T data;
};
#endif // !_LISTNODE_H_

