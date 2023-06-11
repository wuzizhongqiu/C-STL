#pragma once

#include <iostream>
#include <assert.h>
using namespace std;

enum Color {
	RED,
	BLACK
};

template<class T>
struct RBTreeNode {
	RBTreeNode<T>* _left;
	RBTreeNode<T>* _right;
	RBTreeNode<T>* _parent;

	T _data;
	Color _col;

	RBTreeNode(const T& data)
		: _left(nullptr)
		, _right(nullptr)
		, _parent(nullptr)
		, _data(data)
	{}
};

template<class T, class Ref, class Ptr>
class __RBTreeIterator {
	typedef RBTreeNode<T> Node;
	typedef __RBTreeIterator<T, Ref, Ptr> Self;

	Node* _node;

public:
	__RBTreeIterator(Node* node)
		:_node(node)
	{}

public:
	Self& operator++() {
		if (_node->_right) { //����Ҳ�Ϊ��
			//����һ��û�߹�������ڵ�
			Node* left = _node->_right;
			while (left->_left) {
				left = left->_left;
			}
			_node = left;
		}
		else { //�����Ϊ��
			//��һ�����������Ǹýڵ���������������
			Node* parent = _node->_parent;
			Node* cur = _node;
			while (parent && cur == parent->_right) {
				cur = cur->_parent;
				parent = parent->_parent;
			}
			_node = parent;
		}
		return *this;
	}

	Self& operator++(int) {
		auto tmp = *this;
		++* this;
		return tmp;
	}

	Self& operator--() { // --���߼���++���߼��պ��෴
		if (_node->_left) {
			Node* right = _node->_left;
			while (right->_right) {
				right = right->_right;
			}
			_node = right;
		}
		else {
			Node* parent = _node->_parent;
			Node* cur = _node;
			while (parent && cur == parent->_left) {
				cur = cur->_parent;
				parent = parent->_parent;
			}
			_node = parent;
		}
		return *this;
	}

	Self& operator--(int) {
		auto tmp = *this;
		--* this;
		return tmp;
	}

	Ref operator*() {
		return _node->_data;
	}

	Ptr operator->() {
		return &_node->_data;
	}

	bool operator!=(const Self& s) const {
		return _node != s._node;
	}

	bool operator==(const Self& s) const {
		return _node == s._node;
	}

};

template<class K, class T, class KeyOfT>
class RBTree {
	typedef RBTreeNode<T> Node;

public:
	typedef __RBTreeIterator<T, T&, T*> iterator;

	iterator begin() {
		Node* left = _root;
		while (left && left->_left) {
			left = left->_left;
		}
		return iterator(left);
	}

	iterator end() {
		return iterator(nullptr);
	}

public:
	pair<iterator, bool> Insert(const T& data) {
		KeyOfT kot;
		if (_root == nullptr) {
			_root = new Node(data);
			_root->_col = BLACK; //���Ǻڽڵ�
			return make_pair(iterator(_root), true);
		}
		Node* parent = nullptr;
		Node* cur = _root;
		while (cur) {
			if (kot(cur->_data) < kot(data)) { //kv������
				parent = cur;
				cur = cur->_right;
			}
			else if (kot(cur->_data) > kot(data)) { //kvС������
				parent = cur;
				cur = cur->_left;
			}
			else return make_pair(iterator(cur), false);
		}
		cur = new Node(data);
		Node* newnode = cur;
		cur->_col = RED;
		if (kot(parent->_data) < kot(data)) {
			parent->_right = cur;
		}
		else {
			parent->_left = cur;
		}
		cur->_parent = parent; //��Ϊ�����������������Ҫ����һ������parent

		while (parent && parent->_col == RED) { //��������ɫ����������
			Node* grandfather = parent->_parent;
			assert(grandfather && grandfather->_col == BLACK);

			//�ؼ��������������
			if (parent == grandfather->_left) { //�����ң����һ��������
				Node* uncle = grandfather->_right;

				//���һ�����������Ϊ��
				if (uncle && uncle->_col == RED) { //��ɫ
					parent->_col = uncle->_col = BLACK;
					grandfather->_col = RED;

					//�������ϴ���
					cur = grandfather;
					parent = cur->_parent;
				}
				else { //����� + ����������岻���ڻ����������Ϊ��
					//����������� + ��ɫ
					//     g
					//   p   u
					// c
					if (cur == parent->_left) {
						RotateR(grandfather);
						parent->_col = BLACK;
						grandfather->_col = RED;
					}
					else {
						//�������˫�� + ��ɫ
						//     g
						//   p   u
						//     c
						RotateL(parent);
						RotateR(grandfather);
						cur->_col = BLACK;
						grandfather->_col = RED;
					}
					break;
				}
			}
			else { //parent == grandfather->_right
				Node* uncle = grandfather->_left;

				//���һ�����������Ϊ��
				if (uncle && uncle->_col == RED) { //��ɫ + �������ϴ���
					parent->_col = uncle->_col = BLACK;
					grandfather->_col = RED;

					//�������ϴ���
					cur = grandfather;
					parent = cur->_parent;
				}
				else { //����� + ����������岻���ڻ����������Ϊ��
					   //����������� + ��ɫ
					   //     g
					   //   u   p
					   //         c
					if (cur == parent->_right) {
						RotateL(grandfather);
						parent->_col = BLACK;
						grandfather->_col = RED;
					}
					else {
						//�������˫�� + ��ɫ
						//     g
						//   u   p
						//     c
						RotateR(parent);
						RotateL(grandfather);
						cur->_col = BLACK;
						grandfather->_col = RED;
					}
					break;
				}
			}
		}
		_root->_col = BLACK; //��Ϊ���ڵ�һ���Ǻ�ɫ���������Ǿ�ֱ�Ӵ���
		return make_pair(iterator(newnode), true);
	}

	void Inorder() {
		_Inorder(_root);
		cout << endl;
	}

	bool IsBalance() {
		return _IsBalance(_root);
		cout << endl;
	}

private:
	void _Inorder(Node* root) {
		if (root == nullptr) return;
		_Inorder(root->_left);
		cout << root->_kv.first << ":" << root->_kv.second << " ";
		_Inorder(root->_right);
	}

	bool _IsBalance(Node* root) {
		if (root == nullptr) return true;
		if (root->_col != BLACK) {
			cout << "���ڵ㲻�Ǻ�ɫ" << endl;
			return false;
		}

		int benchmark = 0;
		Node* cur = root;
		while (cur) {
			if (cur->_col == BLACK) benchmark++;
			cur = cur->_left;
		}

		return _PreCheck(root, benchmark, 0);
	}

	bool _PreCheck(Node* root, int benchmark, int blackNum) {
		if (root == nullptr) {
			if (blackNum != benchmark) {
				cout << "��ɫ�ڵ�������쳣" << endl;
				return false;
			}
			cout << blackNum << " ";
			return true;
		}
		if (root->_col == RED && root->_parent->_col == RED) {
			cout << "���������ĺ�ɫ�ڵ�" << endl;
			return false;
		}
		if (root->_col == BLACK) blackNum++;
		return _PreCheck(root->_left, benchmark, blackNum) && _PreCheck(root->_right, benchmark, blackNum);
	}

private: //��������ת
	void RotateL(Node* parent) { //����
		Node* subR = parent->_right;
		Node* subRL = subR->_left;
		Node* ppNode = parent->_parent;

		parent->_right = subRL;
		subR->_left = parent;

		//�����Ǹ�parent
		if (subRL) subRL->_parent = parent; //С�Ŀ�ָ��

		parent->_parent = subR;

		if (_root == parent) { //parent�ڵ���Ǹ��ڵ�����
			_root = subR;
			subR->_parent = nullptr;
		}
		else { //parent�ڵ㻹��parent�������parent�ڵ���һ��������
			if (ppNode->_left == parent) {
				ppNode->_left = subR;
			}
			else {
				ppNode->_right = subR;
			}
			subR->_parent = ppNode;
		}
	}

	void RotateR(Node* parent) { //�ҵ��� //��������һ����
		Node* subL = parent->_left;
		Node* subLR = subL->_right;
		Node* ppNode = parent->_parent;

		parent->_left = subLR;
		subL->_right = parent;

		//�����Ǹ�parent
		if (subLR) subLR->_parent = parent; //С�Ŀ�ָ��

		parent->_parent = subL;

		if (_root == parent) { //parent�ڵ���Ǹ��ڵ�����
			_root = subL;
			subL->_parent = nullptr;
		}
		else { //parent�ڵ㻹��parent�������parent�ڵ���һ��������
			if (ppNode->_left == parent) {
				ppNode->_left = subL;
			}
			else {
				ppNode->_right = subL;
			}
			subL->_parent = ppNode;
		}
	}

private:
	Node* _root = nullptr;
};