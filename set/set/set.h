#pragma once

#include "RBTree_set.h"

namespace xl {
	template<class K>
	class set {
		class SetKeyOfT {
		public:
			const K& operator()(const K& key) { return key; }
		};

	public:
		typedef typename RBTree<K, K, SetKeyOfT>::iterator iterator;

		iterator begin() {
			return _t.begin();
		}

		iterator end() {
			return _t.end();
		}

	public:
		pair<iterator, bool> Insert(const K& key) {
			return _t.Insert(key);
		}

	private:
		RBTree<K, K, SetKeyOfT> _t;
	};
}