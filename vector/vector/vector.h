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

		//���ֵ�����ʵ��
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

		vector() //����
			: _start(nullptr)
			, _finish(nullptr)
			, _end_of_storage(nullptr)
		{}

		//vector(const vector<T>& v) { //�������� v1(v2)
		//	_start = new T[v.size()];
		//	//memcpy(_start, v._start, sizeof(T) * v.size());//������
		//	for (size_t i = 0; i < v.size(); i++) {
		//		_start[i] = v._start[i];
		//	}
		//	_finish = _start + v.size();
		//	_end_of_storage = _start + v.size();
		//}

		//vector(const vector<T>& v) //�������� v1(v2)
		//	: _start(nullptr)
		//	, _finish(nullptr)
		//	, _end_of_storage(nullptr)
		//{
		//	reserve(v.size());
		//	for (const auot& e : v) {
		//		v.push_back(e);
		//	}
		//}

		//vector(size_t n, const T& val = T()) //���صĹ��췽�� //��Ҫ����
		//	: _start(nullptr)
		//	, _finish(nullptr)
		//	, _end_of_storage(nullptr)
		//{
		//	reserve(n);
		//	for (size_t i = 0; i < n; i++) {
		//		push_back(val);
		//	}
		//}

		template<class Inputiterator> //һ����������ӿ�
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

		void swap(vector<T>& v) { //vector�ڵ�swap
			::swap(_start, v._start);
			::swap(_finish, v._finish);
			::swap(_end_of_storage, v._end_of_storage);
		}

		vector(const vector<T>& v) //�������� //�ִ�д��
			: _start(nullptr)
			, _finish(nullptr)
			, _end_of_storage(nullptr)
		{
			vector<T> tmp(v.begin(), v.end()); //�����������
			swap(tmp); //��tmp���Ҵ� 
		} 

		vector<T>& operator=(vector<T> v) { //��ֵ���� //�ִ�����
			swap(v);
			return *this;
		}

		~vector() { //����
			delete[] _start;
			_start = _finish = _end_of_storage = nullptr;
		}

		T& operator[](size_t pos) { //ȡ�±�
			assert(pos < size());
			return _start[pos];
		}

		const T& operator[](size_t pos) const { //const����
			assert(pos < size());
			return _start[pos];
		}

		size_t size() { //���ݸ���
			return _finish - _start;
		}

		size_t capacity() { //����
			return _end_of_storage - _start;
		}

		void resize(size_t n, const T& val = T()) { //��ʼ��
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

		void reserve(size_t n) { //����
			if (n > capacity()) {
				size_t sz = size();//��ǰȡ��size
				T* tmp = new T[n];
				if (_start) { //��������д�����ݣ���copyһ��
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

		void push_back(const T& x) { //β��
			//if (_finish == _end_of_storage) {
			//	reserve(capacity() == 0 ? 4 : capacity() * 2);
			//}
			//*_finish = n;
			//_finish++; 

			insert(end(), x);//����
		}

		void pop_back() { //βɾ
			assert(_finish > _start);
			_finish--;
		}

		void insert(iterator pos, const T& x) { //����
			assert(pos >= _start);
			assert(pos <= _finish);

			if (_finish == _end_of_storage) {
				size_t len = pos - _start;
				reserve(capacity() == 0 ? 4 : capacity() * 2);
				pos = _start + len;
			}

			//Ų������
			iterator end = _finish - 1;
			while (end >= pos) {
				*(end + 1) = *end;
				end--;
			}
			*pos = x;//����
			_finish++; 
		}

		// stl �涨eraseҪ����ɾ��λ�õ���һ��λ�õĵ�����
		iterator erase(iterator pos) { //ɾ��
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

		T& front() { //ȡͷ
			assert(size() > 0);
			return *_start;
		}

		T& back() { //ȡβ
			assert(size() > 0);
			return *(_finish - 1);
		}

	private:
		iterator _start;
		iterator _finish;
		iterator _end_of_storage;
	};

//***************************************************�����ǲ���****************************************************//

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
		//��Pλ�ò�������֮��Ҫ����P,��Ϊ����PʧЧ��
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
				it = v.erase(it);// ��insert/eraseһ��Ҫ����
			}                    // ��Ҫֱ�ӷ������ǵ�posλ��
			else {               // �������ν������ʧЧ������
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