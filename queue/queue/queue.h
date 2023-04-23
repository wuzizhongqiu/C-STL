#define _CRT_SECURE_NO_WARNINGS 1

#include <iostream>
#include <assert.h>
using namespace std;

#include <iostream>
#include <vector>
#include <deque>
using namespace std;

namespace xl {
	template<class T, class Container = deque<int>>
	class queue {
	public:
		void push(const T& x) {
			_con.push_back(x);
		}

		void pop() {
			_con.pop_front();
		}

		T& back() {
			return _con.back();
		}

		const T& back() const {
			return _con.front();
		}

		T& front() {
			return _con.front();
		}

		const T& front() const {
			return _con.back();
		}

		bool empty() const {
			return _con.empty();
		}

		size_t size() const {
			return _con.size();
		}

	private:
		Container _con; //适配器
	};

	void test_stack() {
		queue<int> q; //这个是不支持vector的
		q.push(1);
		q.push(2);
		q.push(3);
		q.push(4);
		cout << q.front() << endl;
	}
}