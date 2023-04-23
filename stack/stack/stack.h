#define _CRT_SECURE_NO_WARNINGS 1

#include <iostream>
#include <vector>
#include <deque>
using namespace std;

namespace xl {
	template<class T, class Container = deque<int>>
	class stack {
	public:
		void push(const T& x) {
			_con.push_back(x);
		}

		void pop() {
			_con.pop_back();	
		}

		T& top() {
			return _con.back();
		}

		const T& top() const {
			return _con.back();
		}

		bool empty() const {
			return _con.empty();
		}

		size_t size() const {
			return _con.size();
		}

	private:
		Container _con; //  ≈‰∆˜
	};

	void test_stack() {
		stack<int> s;
		s.push(1);
		s.push(2);
		s.push(3);
		s.push(4);
		cout << s.top() << endl;
	}
}
