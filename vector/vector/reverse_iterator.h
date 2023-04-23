#pragma once

namespace xl {
	//µü´úÆ÷ÊÊÅäÆ÷
	template<class Iterator, class Ref, class Ptr>
	class __reverse_iterator {
	public:
		typedef __reverse_iterator<Iterator, Ref, Ptr> RIterator;
		Iterator _cur;

		__reverse_iterator(Iterator it)
			:_cur(it)
		{}

		RIterator& operator++() {
			--_cur;
			return *this;
		}

		RIterator& operator++(int) {
			RIterator tmp(*this);
			--_cur;
			return tmp;
		}

		RIterator& operator--() {
			++_cur;
			return *this;
		}

		RIterator& operator--(int) {
			RIterator tmp(*this);
			++_cur;
			return tmp;
		}

		Ref operator*() {
			Iterator tmp = _cur;
			return *(--tmp);
		}

		Ptr operator->() {
			return &(operator*());
		}

		bool operator!=(const RIterator& it) {
			return _cur != it._cur;
		}
	};
}
