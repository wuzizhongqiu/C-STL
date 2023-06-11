#define _CRT_SECURE_NO_WARNINGS 1

#include "map.h"
#include <vector>

void test() {
	xl::map<int, int> mp;
	vector<int> v{ 3, 2, 5, 9, 7, 9, 2, 3, 4 };
	for (auto e : v) {
		mp[e]++;
	}
	xl::map<int, int>::iterator it = mp.begin();
	while (it != mp.end()) {
		cout << it->first << ":" << it->second << " ";
		it++;
	}
	cout << endl;
	for (auto e : mp) {
		cout << e.first << ":" << e.second << " ";
	}
}

int main()
{
	test();
	return 0;
}