#define _CRT_SECURE_NO_WARNINGS 1

#include <iostream>
#include <assert.h>
#include <string.h>
#include <vector>
using namespace std;

namespace xl
{
	template<class T>
	class vector 
	{
	public:
		typedef T* iterator;
		typedef const T* const_iterator;

		//两种迭代器实现
		iterator begin() {
			return _start;
		}

		iterator end() {
			return _finish;
		}

		const_iterator begin() const {
			return _start;
		}

		const_iterator end() const {
			return _finish;
		}

		vector() //构造
			: _start(nullptr)
			, _finish(nullptr)
			, _end_of_storage(nullptr)
		{}

		//vector(const vector<T>& v) { //拷贝构造 v1(v2)
		//	_start = new T[v.size()];
		//	//memcpy(_start, v._start, sizeof(T) * v.size());//不能用
		//	for (size_t i = 0; i < v.size(); i++) {
		//		_start[i] = v._start[i];
		//	}
		//	_finish = _start + v.size();
		//	_end_of_storage = _start + v.size();
		//}

		//vector(const vector<T>& v) //拷贝构造 v1(v2)
		//	: _start(nullptr)
		//	, _finish(nullptr)
		//	, _end_of_storage(nullptr)
		//{
		//	reserve(v.size());
		//	for (const auot& e : v) {
		//		v.push_back(e);
		//	}
		//}

		//vector(size_t n, const T& val = T()) //重载的构造方法 //需要修正
		//	: _start(nullptr)
		//	, _finish(nullptr)
		//	, _end_of_storage(nullptr)
		//{
		//	reserve(n);
		//	for (size_t i = 0; i < n; i++) {
		//		push_back(val);
		//	}
		//}

		template<class Inputiterator> //一个拷贝构造接口
		vector(Inputiterator first, Inputiterator last) 
			: _start(nullptr)
			, _finish(nullptr)
			, _end_of_storage(nullptr)
		{
			while (first != last) {
				push_back(*first);
				first++;
			}
		}

		void swap(vector<T>& v) { //vector内的swap
			::swap(_start, v._start);
			::swap(_finish, v._finish);
			::swap(_end_of_storage, v._end_of_storage);
		}

		vector(const vector<T>& v) //拷贝构造 //现代写法
			: _start(nullptr)
			, _finish(nullptr)
			, _end_of_storage(nullptr)
		{
			vector<T> tmp(v.begin(), v.end()); //借助这个构造
			swap(tmp); //让tmp帮我打工 
		} 

		vector<T>& operator=(vector<T> v) { //赋值构造 //现代方法
			swap(v);
			return *this;
		}

		~vector() { //析构
			delete[] _start;
			_start = _finish = _end_of_storage = nullptr;
		}

		T& operator[](size_t pos) { //取下标
			assert(pos < size());
			return _start[pos];
		}

		const T& operator[](size_t pos) const { //const重载
			assert(pos < size());
			return _start[pos];
		}

		size_t size() { //数据个数
			return _finish - _start;
		}

		size_t capacity() { //容量
			return _end_of_storage - _start;
		}

		void resize(size_t n, const T& val = T()) { //初始化
			if (n > capacity) {
				reserve(n);
			}
			else if (n > size) {
				while (_finish < _start + n) {
					*_finish = val;
					_finsh++;
				}
			}
			else {
				_finsh = _start + n;
			}
		}

		void reserve(size_t n) { //扩容
			if (n > capacity()) {
				size_t sz = size();//提前取好size
				T* tmp = new T[n];
				if (_start) { //如果里面有存放数据，就copy一下
 					//memcpy(tmp, _start, sizeof(T) * sz);
					for (size_t i; i < sz; i++) {
						tmp[i] = _start[i];
					}
					delete[] _start;
				}
				_start = tmp;
				_finish = _start + sz;
				_end_of_storage = _start + n;
			}
		}

		void push_back(const T& x) { //尾插
			//if (_finish == _end_of_storage) {
			//	reserve(capacity() == 0 ? 4 : capacity() * 2);
			//}
			//*_finish = n;
			//_finish++; 

			insert(end(), x);//复用
		}

		void pop_back() { //尾删
			assert(_finish > _start);
			_finish--;
		}

		void insert(iterator pos, const T& x) { //插入
			assert(pos >= _start);
			assert(pos <= _finish);

			if (_finish == _end_of_storage) {
				size_t len = pos - _start;
				reserve(capacity() == 0 ? 4 : capacity() * 2);
				pos = _start + len;
			}

			//挪动数据
			iterator end = _finish - 1;
			while (end >= pos) {
				*(end + 1) = *end;
				end--;
			}
			*pos = x;//插入
			_finish++; 
		}

		// stl 规定erase要返回删除位置的下一个位置的迭代器
		iterator erase(iterator pos) { //删除
			assert(pos >= _start);
			assert(pos < _finish);

			iterator begin = pos + 1;
			while (begin < _finish) {
				*(begin - 1) = *begin;
				begin++;
			}
			_finish--;
			return pos;
		}

		T& front() { //取头
			assert(size() > 0);
			return *_start;
		}

		T& back() { //取尾
			assert(size() > 0);
			return *(_finish - 1);
		}

	private:
		iterator _start;
		iterator _finish;
		iterator _end_of_storage;
	};

//***************************************************下面是测试****************************************************//

	void test_vector1() {
		vector<int> v;
		v.push_back(1);
		v.push_back(2);
		v.push_back(3);
		v.push_back(4);

		//v.push_back(5);
		for (size_t i = 0; i < v.size(); i++) {
			cout << v[i] << " ";
		}
		cout << endl;

		auto p = find(v.begin(), v.end(), 3);
		v.insert(p, 50);
		//在P位置插入数据之后不要访问P,因为可能P失效了
		//cout << *p << endl;

		vector<int>::iterator it = v.begin();
		while (it != v.end()) {
			cout << *it << " ";
			it++;
		}
		cout << endl;

		v.erase(v.begin());
		v.pop_back();

		for (auto e : v) {
			cout << e << " ";
		}
		cout << endl;
	} 

	void test_vector2() {
		vector<int> v;
		v.push_back(1);
		v.push_back(2);
		v.push_back(4);
		v.push_back(4);
		v.push_back(5);

		auto it = v.begin();
		while (it != v.end()) {
			if (*it % 2 == 0) {
				it = v.erase(it);// 用insert/erase一定要更新
			}                    // 不要直接访问他们的pos位置
			else {               // 这就是所谓迭代器失效的问题
				it++;
			}
		}

		for (auto e : v) {
			cout << e << " ";
		}
	}
	void test_vector3() {
		string s("hello world");
		vector<int> v(s.begin(), s.end());
		vector<int> vs;
		vs = v;
		for (auto e : vs) {
			cout << e << " ";
		}
		cout << endl;
	}
}