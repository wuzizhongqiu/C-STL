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
		if (_node->_right) { //如果右不为空
			//找下一个没走过的最左节点
			Node* left = _node->_right;
			while (left->_left) {
				left = left->_left;
			}
			_node = left;
		}
		else { //如果右为空
			//找一个右子树不是该节点所在子树的祖先
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

	Self& operator--() { // --的逻辑跟++的逻辑刚好相反
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
			_root->_col = BLACK; //根是黑节点
			return make_pair(iterator(_root), true);
		}
		Node* parent = nullptr;
		Node* cur = _root;
		while (cur) {
			if (kot(cur->_data) < kot(data)) { //kv大，走右
				parent = cur;
				cur = cur->_right;
			}
			else if (kot(cur->_data) > kot(data)) { //kv小，走左
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
		cur->_parent = parent; //因为是三叉链，所以最后要更新一下他的parent

		while (parent && parent->_col == RED) { //有连续红色，继续处理
			Node* grandfather = parent->_parent;
			assert(grandfather && grandfather->_col == BLACK);

			//关键看叔叔来分情况
			if (parent == grandfather->_left) { //分左右，情况一不看方向，
				Node* uncle = grandfather->_right;

				//情况一，叔叔存在且为红
				if (uncle && uncle->_col == RED) { //变色
					parent->_col = uncle->_col = BLACK;
					grandfather->_col = RED;

					//继续往上处理
					cur = grandfather;
					parent = cur->_parent;
				}
				else { //情况二 + 情况三，叔叔不存在或叔叔存在且为黑
					//情况二，单旋 + 变色
					//     g
					//   p   u
					// c
					if (cur == parent->_left) {
						RotateR(grandfather);
						parent->_col = BLACK;
						grandfather->_col = RED;
					}
					else {
						//情况三，双旋 + 变色
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

				//情况一，叔叔存在且为红
				if (uncle && uncle->_col == RED) { //变色 + 继续往上处理
					parent->_col = uncle->_col = BLACK;
					grandfather->_col = RED;

					//继续往上处理
					cur = grandfather;
					parent = cur->_parent;
				}
				else { //情况二 + 情况三，叔叔不存在或叔叔存在且为黑
					   //情况二，单旋 + 变色
					   //     g
					   //   u   p
					   //         c
					if (cur == parent->_right) {
						RotateL(grandfather);
						parent->_col = BLACK;
						grandfather->_col = RED;
					}
					else {
						//情况三，双旋 + 变色
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
		_root->_col = BLACK; //因为根节点一定是黑色，所以我们就直接处理
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
			cout << "根节点不是黑色" << endl;
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
				cout << "黑色节点的数量异常" << endl;
				return false;
			}
			cout << blackNum << " ";
			return true;
		}
		if (root->_col == RED && root->_parent->_col == RED) {
			cout << "存在连续的红色节点" << endl;
			return false;
		}
		if (root->_col == BLACK) blackNum++;
		return _PreCheck(root->_left, benchmark, blackNum) && _PreCheck(root->_right, benchmark, blackNum);
	}

private: //下面是旋转
	void RotateL(Node* parent) { //左单旋
		Node* subR = parent->_right;
		Node* subRL = subR->_left;
		Node* ppNode = parent->_parent;

		parent->_right = subRL;
		subR->_left = parent;

		//下面是改parent
		if (subRL) subRL->_parent = parent; //小心空指针

		parent->_parent = subR;

		if (_root == parent) { //parent节点就是根节点的情况
			_root = subR;
			subR->_parent = nullptr;
		}
		else { //parent节点还有parent的情况（parent节点是一个子树）
			if (ppNode->_left == parent) {
				ppNode->_left = subR;
			}
			else {
				ppNode->_right = subR;
			}
			subR->_parent = ppNode;
		}
	}

	void RotateR(Node* parent) { //右单旋 //跟左单旋是一样的
		Node* subL = parent->_left;
		Node* subLR = subL->_right;
		Node* ppNode = parent->_parent;

		parent->_left = subLR;
		subL->_right = parent;

		//下面是改parent
		if (subLR) subLR->_parent = parent; //小心空指针

		parent->_parent = subL;

		if (_root == parent) { //parent节点就是根节点的情况
			_root = subL;
			subL->_parent = nullptr;
		}
		else { //parent节点还有parent的情况（parent节点是一个子树）
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