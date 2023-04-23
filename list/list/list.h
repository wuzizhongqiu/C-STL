#pragma once

#include "reverse_iterator.h"
#include <iostream>
#include <assert.h>
using namespace std;

namespace xl {

	template<class T>
	class list_node { //链表节点
	public:
		list_node(const T& x = T())
			: _data(x)
			, _next(nullptr)
			, _prev(nullptr)
		{}

		T _data;
		list_node<T>* _next;
		list_node<T>* _prev;
	};

	template<class T, class Ref, class Ptr>
	class __list_iterator { //迭代器 //迭代器的操作符重载
	public:
		typedef list_node<T> Node;
		typedef __list_iterator<T, Ref, Ptr> iterator;

		Node* _node; //节点

		__list_iterator(Node* node) //构造
			:_node(node) 
		{}

		bool operator!=(const iterator& it) const {
			return _node != it. _node;
		}

		bool operator==(const iterator& it) const {
			return _node == it._node;
		}

		//const operator*() const {
		Ref operator*() {
			return _node->_data;
		}

		//const operator->() const {
		Ptr operator->() {	
			return &(operator*()); 
		}

		iterator& operator++() {
			_node = _node->_next;
			return *this;
		}

		iterator& operator++(int) {
			iterator tmp(*this);
			_node = _node->_next;
			return tmp;
		}

		iterator& operator--() {
			_node = _node->_prev;
			return *this;
		}

		iterator& operator--(int) {
			iterator tmp(*this);
			_node = _node->_prev;
			return tmp;
		}
	};

	template<class T>
	class list { //链表
		typedef list_node<T> Node;	
	public:
		typedef __list_iterator<T, T&, T*> iterator;
		typedef __list_iterator<T, const T&, const T*> const_iterator;

		typedef __reverse_iterator<iterator, T&, T*> reverse_iterator;
		typedef __reverse_iterator<const_iterator, const T&, const T*> const_reverse_iterator;

		//迭代器的接口
		iterator begin() {
			return iterator(_head->_next);
		}

		iterator end() {
			return iterator(_head);
		}

		const_iterator begin() const{
			return const_iterator(_head->_next);
		}

		const_iterator end() const {
			return const_iterator(_head);
		}

		//反向迭代器
		reverse_iterator rbegin() {
			return reverse_iterator(end());
		}

		reverse_iterator rend() {
			return reverse_iterator(begin());
		}

		const_reverse_iterator rbegin() const {
			return const_reverse_iterator(end());
		}

		const_reverse_iterator rend() const {
			return const_reverse_iterator(begin());
		}

		void empty_init() {
			_head = new Node;
			_head->_next = _head;
			_head->_prev = _head;
		}

		void swap(list<T>& lt) { //一般而言，每个容器会有一个自己的swap函数
			::swap(_head, lt._head);
		}

		template<class IteratorInput>
		list(IteratorInput first, IteratorInput last) { //迭代器区间构造
			empty_init();
			while (first != last) {
				push_back(*first);
				first++;
			}
		}

		list() { //构造 //链表的基础接口 //核心是insert和erase
			empty_init();
		}

		//lt2(lt)
		list(const list<T>& lt) { //拷贝构造 //现代写法
			empty_init();
			list<T> tmp(lt.begin(), lt.end());
			swap(tmp);
		}

		//lt = lt2
		list<T>& operator=(list<T> lt) {
			swap(lt);
			return *this;
		}

		~list() { //析构
			clear();
			delete _head;
			_head = nullptr;
		}

		void clear() { //清理
			iterator it = begin();
			while (it != end()) {
				it = erase(it);
			}
		}

		void push_back(const T& x) { //尾插
			////找尾
			//Node* tail = _head->_prev;
			//Node* newnode = new Node(x);

			////插入
			//tail->_next = newnode;
			//newnode->_prev = tail;
			//_head->_prev = newnode;
			//newnode->_next = _head;

			//复用
			insert(end(), x);
		}

		void push_front(const T& x) { //头插
			insert(begin(), x);
		}

		iterator insert(iterator pos, const T& x) {
			Node* cur = pos._node;
			Node* prev = cur->_prev;

			Node* newnode = new Node(x);
			
			//prev newnode cur
			prev->_next = newnode;
			newnode->_prev = prev;
			newnode->_next = cur;
			cur->_prev = newnode;

			return iterator(newnode);
		}

		void pop_back() {
			erase(--end());
		}

		void pop_front() {
			erase(begin());
		}

		iterator erase(iterator pos) {
			assert(pos != end());

			Node* cur = pos._node;
			Node* prev = cur->_prev;
			Node* next = cur->_next;

			//prev cur next
			prev->_next = next;
			next->_prev = prev;
			delete cur;

			return iterator(next);
		}

	private:
		Node* _head;
	};

//***********************************************下面是测试**************************************************

	void test_const(const list<int>& lt) {
		list<int>::const_iterator it = lt.begin();
		while (it != lt.end()) {
			cout << *it << " ";
			++it;
		}
		cout << endl;
	}
	 
	void test_list1() {
		list<int> lt;
		lt.push_back(1);
		lt.push_back(2);
		lt.pop_front();
		lt.push_back(3);
		lt.push_back(4);
		lt.pop_back();

		test_const(lt);
		
		list<int>::iterator it = lt.begin();
		while (it != lt.end()) {
			cout << *it << " ";
			++it;
		}
		cout << endl;

		for (auto e : lt) {
			cout << e << " ";
		}
		cout << endl;
	}

	void test_list2() {
		list<int> lt;
		lt.push_back(1);
		lt.push_back(2);
		lt.push_back(3);
		lt.push_back(4);

		list<int> lt2(lt); //拷贝构造

		list<int> copy; 

		copy = lt2; //赋值构造
	}

	void test_reverse() {
		list<int> lt;
		lt.push_back(1);
		lt.push_back(2);
		lt.push_back(3);
		lt.push_back(4);
		lt.push_back(5);
		list<int>::reverse_iterator rit = lt.rbegin();
		while (rit != lt.rend()) {
			cout << *rit << " ";
			rit++;
		}
	}
}