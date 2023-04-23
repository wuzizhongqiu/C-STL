#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include <functional>
using namespace std;

namespace xl {
	template<class T, class Container = vector<T>, class Compare = less<T>>
	class priority_queue {
	public:
		priority_queue() {}

		template<class InputIterator>
		priority_queue(InputIterator first, InputIterator last) {
			while (first != last) {
				_con.push_back(*first);
				first++;
			}
			for (int i = (_con.size() - 1 - 1) / 2; i >= 0; i--) {
				adjust_down(i);
			}
		}

		//向上调整
		void adjust_up(size_t child) {
			Compare com;
			int parent = (child + 1) / 2;
			while (child > 0) {
				if (com(_con[parent], _con[child])) {
					swap(_con[child], _con[parent]);
					child = parent;
					parent = (child + 1) / 2;
				}
				else {
					break;
				}
			}
		}

		void push(const T& x) {
			_con.push_back(x);
			adjust_up(_con.size() - 1);
		}

		//向下调整
		void adjust_down(size_t parent) {
			Compare com;
			int child = parent * 2 + 1;
			while (child < _con.size()) {
				if (child + 1 < _con.size() && com(_con[child], _con[child + 1])) {
					child++;
				}
				if (com(_con[parent], _con[child])) {
					swap(_con[child], _con[parent]);
					parent = child;
					child = parent * 2 + 1;
				}
				else {
					break;
				}
			}
		}

		void pop() {
			swap(_con[0], _con[_con.size() - 1]);
			_con.pop_back();
			adjust_down(0);
		}

		const T& top() const {
			return _con[0];
		}

		bool empty() const {
			return _con.empty();
		}

		size_t size() const {
			return _con.size();
		}
	private:
		Container _con;
	};

	void test() {
		vector<int> v{ 1, 4, 7, 3, 9, 5 };
		xl::priority_queue<int, vector<int>, greater<int>> pq(v.begin(), v.end());
		while (!pq.empty()) {
			cout << pq.top() << " ";
			pq.pop();
		}
		cout << endl;
	}
}
