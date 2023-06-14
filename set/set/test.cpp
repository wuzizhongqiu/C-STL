#define _CRT_SECURE_NO_WARNINGS 1

#include "set.h"
#include <vector>

void test() {
	xl::set<int> st;
	vector<int> v{ 3, 2, 5, 9, 7, 9, 2, 3, 4 };
	for (auto e : v) {
		st.Insert(e);
	}

	xl::set<int>::iterator it = st.begin();
	while (it != st.end()) {
		cout << *it << " ";
		it++;
	}
}

int main()
{
	test();
	return 0;
}