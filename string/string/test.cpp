#define _CRT_SECURE_NO_WARNINGS 1

#include "string.h"

//调用测试
void test() {
	//抓取异常
	try {
		//xl::test_string();
		//xl::test2();
		//xl::test3();
		xl::test4();
	}
	catch (const std::exception& e) {
		cout << e.what() << endl;
	}
}

int main() {
	test();
	return 0;
}