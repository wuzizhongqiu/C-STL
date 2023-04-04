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
		//string��ĵ�������ԭ��ָ��
		typedef char* iterator;

		//const������
		typedef const char* const_iterator;

		//ָ��ͷ
		iterator begin() {
			return _str;
		}

		//ָ��β
		iterator end() {
			return _str + _size;
		}

		//ָ��ͷ
		const_iterator begin() const {
			return _str;
		}

		//ָ��β
		const_iterator end() const {
			return _str + _size;
		}

		//���캯��
		string(const char* str = "") {
			//��ʼ��
			_size = strlen(str);
			_capacity = _size;
			_str = new char[_size + 1];

			//�����ַ���
			strcpy(_str, str);
		}

		////�������죨�����//��ͳд��
		//string(const string& s) 
		//	: _str(new char[s._capacity + 1])
		//	, _size(s._size)
		//	, _capacity(s._capacity)
		//{
		//	strcpy(_str, s._str);
		//}

		////��ֵ����� //��ͳд��
		//string& operator=(const string& s) {
		//	if (this == &s) return *this;//��ֹ�Լ����Լ���ֵ
		//	delete[] _str;
		//	_str = new char[s._capacity + 1];
		//	strcpy(_str, s._str);
		//	_size = s._size;
		//	_capacity = s._capacity;
		//	return *this;	
		//}

		//����tmp���������
		void swap(string& tmp) {
			//����::���������õ��ǿ����
			::swap(_str, tmp._str);
			::swap(_size, tmp._size);
			::swap(_capacity, tmp._capacity);
		}

		//�������죨�����//�ִ�д��
		string(const string& s) 
			: _str(nullptr)
			, _size(0)
			, _capacity(0)
		{
			//����tmp���ù��캯�����ٰѹ���õ����ݻ�����
			string tmp(s._str);
			swap(tmp);//�����Լ���swap
		}

		////��ֵ����� //�ִ�д��
		//string& operator=(const string& s) {
		//	if (this == &s) return *this;//��ֹ�Լ����Լ���ֵ
		//	string tmp(s._str);
		//	swap(tmp);//�����Լ���swap
		//	return *this;
		//}

		//��ֵ����� //�ִ�д�� //�����
		string& operator=(string s) {
			swap(s);//���ô�ֵ������
			return *this;
		}

		//ֻ��
		const char& operator[](size_t pos) const { 
			assert(pos < _size);
			return _str[pos];
		}

		//�ɶ���д
		char& operator[](size_t pos) {
			assert(pos < _size);
			return _str[pos];
		}

		//��ʼ������
		void reserve(size_t n) {
			if (n > _capacity) {
				char* tmp = new char[n + 1];
				strcpy(tmp, _str);
				delete[] _str;

				_str = tmp;
				_capacity = n;
			}
		}

		//��ʼ���ַ���
		void resize(size_t n, char ch) {
			if (n > _size) {
				//��������
				reserve(n);
				for (size_t i = _size; i < n; i++) {
					_str[i] = ch;
				}
				_str[n] = '\0';
				_size = n;
			}
			else {
				//ɾ������
				_str[n] = '\0';
				_size = n;
			}
		}

		//β��
		void push_back(char ch) {
			////����
			//if (_size == _capacity) {
			//	reserve(_capacity == 0 ? 4 : _capacity * 2);
			//}
			//_str[_size] = ch;
			//_size++;
			//_str[_size] = '\0';

			//����
			insert(_size, ch);
		}

		//����ַ���
		void append(const char* str) {
			////����
			//size_t len = strlen(str);
			//if (_size + len > _capacity) {
			//	reserve(_size + len);
			//}
			//strcpy(_str + _size, str);
			//_size = _size + len;

			//����
			insert(_size, str);
		}

		// += �ַ�
		string& operator+=(char ch) {
			push_back(ch);
			return *this;
		}

		// += �ַ���
		string& operator+=(const char* str) {
			append(str);
			return *this;
		}

		//�����ַ�
		string& insert(size_t pos, char ch) {
			assert(pos <= _size);
			//����
			if (_size == _capacity) {
				reserve(_capacity == 0 ? 4 : _capacity * 2);
			}

			//Ų������
			size_t end = _size + 1;
			while (end > pos) {
				_str[end] = _str[end - 1];
				end--;
			}
			_str[pos] = ch;
			_size++;
			return *this;
		}

		//�����ַ���
		string& insert(size_t pos, const char* str) {
			assert(pos <= _size); 
			//����
			size_t len = strlen(str);
			if (_size + len > _capacity) {
				reserve(_size + len);
			}

			//Ų������
			size_t end = _size + len;
			while (end >= pos + len) {
				_str[end] = _str[end - len];
				end--;
			}

			strncpy(_str + pos, str, len);
			_size += len;
			return *this;
		}

		//ɾ��
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

		//�����ַ�
		size_t find(char ch, size_t pos = 0) const {
			assert(pos < _size);
			for (size_t i = pos; i < _size; i++) {
				if (ch == _str[i]) return i;
			}
			return npos;
		}

		//�����ַ���
		size_t find(const char* sub, size_t pos = 0) const {
			assert(sub);
			assert(pos < _size);
			const char* ptr = strstr(_str + pos, sub);
			if (ptr == nullptr) return npos;
			else return ptr - _str;
		}

		//���Ҷ������ַ���
		string substr(size_t pos, size_t len = npos) const {
			assert(pos < _size);
			size_t real_len = len;
			//����Ѿ�len >= ʣ���ַ��ĳ��ȣ���ȡ��
			if (len == npos || len + pos > _size) {
				real_len += _size - pos;
			}
			string sub;
			for (size_t i = 0; i < real_len; i++) {
				sub += _str[pos + i];
			}
			return sub;
		}
		
		//�����ǱȽ������ //ʵ��������ʣ�¸���
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
		  
		//���ش�С
		size_t size() const {
			return _size;
		}

		//��������
		size_t capacity() const {
			return _capacity;
		}

		//�����ַ����׵�ַ
		const char* c_str() const {
			return _str;
		}

		//����
		void clear() {
			_str[0] = '\0';
			_size = 0;
		}

		//��������
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
		//C++��������const static (���԰�����ֱ�Ӷ����ʼ��)
		const static size_t npos = -1;
	};

	//size_t string::npos = -1;

	//����������
	std::ostream& operator<<(std::ostream& out, const string& s) {
		for (size_t i = 0; i < s.size(); i++) {
			out << s[i];
		}
		return out;
	}

	//��������ȡ
	std::istream& operator>>(std::istream& in, string& s) {
		char ch;
		ch = in.get();//in.get�ܽ��յ��ո�

		//�������Ч��
		const size_t N = 32;
		char buff[N];
		size_t i = 0;
		s.clear();
		while (ch != ' ' && ch != '\n') {
			buff[i++] = ch;
			if (i == N - 1) {
				buff[i] = '\0';
				s += buff;//������һ����ӱȽ϶���ַ�
				i = 0;
			}
			ch = in.get();
		}
		buff[i] = '\0';
		s += buff;
		return in;
	}

//******************************************�����ǲ���**********************************************//

	//����
	void test_string() {
		//��ʼ��
		string s("hello string");

		//����[]
		for (int i = 0; i < s.size(); i++) {
			cout << s[i];
		}
		cout << endl;

		//���Ե�����(�ɶ���д)
		string::iterator it = s.begin();
		while (it != s.end()) {
			cout << *it++;
			it++;
		}
		cout << endl;

		//֧���˵�������֧���˷�Χfor
		//��Χfor�ĵײ���Ǳ����������滻�ɵ�����
		for (auto e : s) {
			cout << e;
		}
		cout << endl;
	}

	//������ԣ�
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