#pragma once

#include <iostream>
#include <string.h>
#include <assert.h>
using std::cout;
using std::endl;
using std::swap;

namespace xl {
	class string {
	public:
		//string里的迭代器就原生指针
		typedef char* iterator;

		//const迭代器
		typedef const char* const_iterator;

		//指向头
		iterator begin() {
			return _str;
		}

		//指向尾
		iterator end() {
			return _str + _size;
		}

		//指向头
		const_iterator begin() const {
			return _str;
		}

		//指向尾
		const_iterator end() const {
			return _str + _size;
		}

		//构造函数
		string(const char* str = "") {
			//初始化
			_size = strlen(str);
			_capacity = _size;
			_str = new char[_size + 1];

			//拷贝字符串
			strcpy(_str, str);
		}

		////拷贝构造（深拷贝）//传统写法
		//string(const string& s) 
		//	: _str(new char[s._capacity + 1])
		//	, _size(s._size)
		//	, _capacity(s._capacity)
		//{
		//	strcpy(_str, s._str);
		//}

		////赋值运算符 //传统写法
		//string& operator=(const string& s) {
		//	if (this == &s) return *this;//防止自己给自己赋值
		//	delete[] _str;
		//	_str = new char[s._capacity + 1];
		//	strcpy(_str, s._str);
		//	_size = s._size;
		//	_capacity = s._capacity;
		//	return *this;	
		//}

		//白嫖tmp构造的数据
		void swap(string& tmp) {
			//加上::表明这里用的是库里的
			::swap(_str, tmp._str);
			::swap(_size, tmp._size);
			::swap(_capacity, tmp._capacity);
		}

		//拷贝构造（深拷贝）//现代写法
		string(const string& s) 
			: _str(nullptr)
			, _size(0)
			, _capacity(0)
		{
			//利用tmp调用构造函数，再把构造好的数据换到手
			string tmp(s._str);
			swap(tmp);//调用自己的swap
		}

		////赋值运算符 //现代写法
		//string& operator=(const string& s) {
		//	if (this == &s) return *this;//防止自己给自己赋值
		//	string tmp(s._str);
		//	swap(tmp);//调用自己的swap
		//	return *this;
		//}

		//赋值运算符 //现代写法 //极简版
		string& operator=(string s) {
			swap(s);//利用传值的特性
			return *this;
		}

		//只读
		const char& operator[](size_t pos) const { 
			assert(pos < _size);
			return _str[pos];
		}

		//可读可写
		char& operator[](size_t pos) {
			assert(pos < _size);
			return _str[pos];
		}

		//初始化容量
		void reserve(size_t n) {
			if (n > _capacity) {
				char* tmp = new char[n + 1];
				strcpy(tmp, _str);
				delete[] _str;

				_str = tmp;
				_capacity = n;
			}
		}

		//初始化字符串
		void resize(size_t n, char ch) {
			if (n > _size) {
				//插入数据
				reserve(n);
				for (size_t i = _size; i < n; i++) {
					_str[i] = ch;
				}
				_str[n] = '\0';
				_size = n;
			}
			else {
				//删除数据
				_str[n] = '\0';
				_size = n;
			}
		}

		//尾插
		void push_back(char ch) {
			////扩容
			//if (_size == _capacity) {
			//	reserve(_capacity == 0 ? 4 : _capacity * 2);
			//}
			//_str[_size] = ch;
			//_size++;
			//_str[_size] = '\0';

			//复用
			insert(_size, ch);
		}

		//添加字符串
		void append(const char* str) {
			////扩容
			//size_t len = strlen(str);
			//if (_size + len > _capacity) {
			//	reserve(_size + len);
			//}
			//strcpy(_str + _size, str);
			//_size = _size + len;

			//复用
			insert(_size, str);
		}

		// += 字符
		string& operator+=(char ch) {
			push_back(ch);
			return *this;
		}

		// += 字符串
		string& operator+=(const char* str) {
			append(str);
			return *this;
		}

		//插入字符
		string& insert(size_t pos, char ch) {
			assert(pos <= _size);
			//扩容
			if (_size == _capacity) {
				reserve(_capacity == 0 ? 4 : _capacity * 2);
			}

			//挪动数据
			size_t end = _size + 1;
			while (end > pos) {
				_str[end] = _str[end - 1];
				end--;
			}
			_str[pos] = ch;
			_size++;
			return *this;
		}

		//插入字符串
		string& insert(size_t pos, const char* str) {
			assert(pos <= _size); 
			//扩容
			size_t len = strlen(str);
			if (_size + len > _capacity) {
				reserve(_size + len);
			}

			//挪动数据
			size_t end = _size + len;
			while (end >= pos + len) {
				_str[end] = _str[end - len];
				end--;
			}

			strncpy(_str + pos, str, len);
			_size += len;
			return *this;
		}

		//删除
		void erase(size_t pos, size_t len = npos) {
			assert(pos < _size);
			
			if (len == pos || pos + len >= _size) {
				_str[pos] = '\0';
				_size = pos;
			}
			else {
				strcpy(_str + pos, _str + pos + len);
				_size = _size - len;
			}
		}

		//查找字符
		size_t find(char ch, size_t pos = 0) const {
			assert(pos < _size);
			for (size_t i = pos; i < _size; i++) {
				if (ch == _str[i]) return i;
			}
			return npos;
		}

		//查找字符串
		size_t find(const char* sub, size_t pos = 0) const {
			assert(sub);
			assert(pos < _size);
			const char* ptr = strstr(_str + pos, sub);
			if (ptr == nullptr) return npos;
			else return ptr - _str;
		}

		//查找定长的字符串
		string substr(size_t pos, size_t len = npos) const {
			assert(pos < _size);
			size_t real_len = len;
			//如果已经len >= 剩下字符的长度，就取完
			if (len == npos || len + pos > _size) {
				real_len += _size - pos;
			}
			string sub;
			for (size_t i = 0; i < real_len; i++) {
				sub += _str[pos + i];
			}
			return sub;
		}
		
		//下面是比较运算符 //实现两个，剩下复用
		bool operator==(const string& s) const {
			return strcmp(_str, s._str) == 0;
		}
		bool operator>(const string& s) const {
			return strcmp(_str, s._str) > 0;
		}
		bool operator<(const string& s) const {
			return !(*this > s);
		}
		bool operator>=(const string& s) const {
			return *this > s || *this == s; 
		}
		bool operator<=(const string& s) const {
			return !(*this > s) || *this == s;
		}
		bool operator!=(const string& s) const {
			return !(*this == s);
		}
		  
		//返回大小
		size_t size() const {
			return _size;
		}

		//返回容量
		size_t capacity() const {
			return _capacity;
		}

		//返回字符串首地址
		const char* c_str() const {
			return _str;
		}

		//清理
		void clear() {
			_str[0] = '\0';
			_size = 0;
		}

		//析构函数
		~string() {
			delete[] _str;
			_str = nullptr;
			_size = 0;
			_capacity = 0;
		}
	private:
		char* _str;
		size_t _size;
		size_t _capacity;

	public:
		//C++的特例：const static (可以把声明直接定义初始化)
		const static size_t npos = -1;
	};

	//size_t string::npos = -1;

	//重载流插入
	std::ostream& operator<<(std::ostream& out, const string& s) {
		for (size_t i = 0; i < s.size(); i++) {
			out << s[i];
		}
		return out;
	}

	//重载流提取
	std::istream& operator>>(std::istream& in, string& s) {
		char ch;
		ch = in.get();//in.get能接收到空格

		//提高扩容效率
		const size_t N = 32;
		char buff[N];
		size_t i = 0;
		s.clear();
		while (ch != ' ' && ch != '\n') {
			buff[i++] = ch;
			if (i == N - 1) {
				buff[i] = '\0';
				s += buff;//方法是一次添加比较多的字符
				i = 0;
			}
			ch = in.get();
		}
		buff[i] = '\0';
		s += buff;
		return in;
	}

//******************************************下面是测试**********************************************//

	//测试
	void test_string() {
		//初始化
		string s("hello string");

		//测试[]
		for (int i = 0; i < s.size(); i++) {
			cout << s[i];
		}
		cout << endl;

		//测试迭代器(可读可写)
		string::iterator it = s.begin();
		while (it != s.end()) {
			cout << *it++;
			it++;
		}
		cout << endl;

		//支持了迭代器就支持了范围for
		//范围for的底层就是编译器将它替换成迭代器
		for (auto e : s) {
			cout << e;
		}
		cout << endl;
	}

	//随意测试：
	void test2() {
		string s1("111111");
		string s3("hello");
		s3 += "12415"; 
		s3.insert(3, "apole");
		cout << s3.c_str() << endl;
	}

	void test3() {
		string s1("hello");
		cout << s1 << endl;
	}

	void test4() {
		string s1("hello string");
		string s2 = s1.substr(5, 10);
		string s3 = s1.substr(0, 5);
		s2.resize(12, 'x');
		cout << s2 << endl;
		cout << s3 << endl;
	}
}