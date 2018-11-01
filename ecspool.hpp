#pragma once

template<typename T>
struct Pool {

	struct Node {
		bool allocated = false;
		unsigned char data[sizeof(T)];
	};

	static const int SIZE = 256;

	Node nodes[SIZE];

	void * allocate() {
		for ( Node & n : nodes ) {
			if ( n.allocated ) continue;
			n.allocated = true;
			return n.data;
		}
		return nullptr;
	}

	void free(void * ptr) {
		for ( Node & n : nodes ) {
			if ( n.data == ptr ) {
				n.allocated = false;
				return;
			}
		}
	}

	template<typename F>
	void forEach(F f) {
		for ( Node & n : nodes ) {
			if ( n.allocated ) {
				f( (T *)n.data );
			}
		}
	}

	struct iterator {

		Pool * pool;
		int index;

		T * operator *() {
			return (T *)pool->nodes[index].data;
		}

		iterator & operator ++() {
			index++;
			skipEmpty();
			return *this;
		}

		bool operator !=(const iterator & other) const {
			return index < other.index;
		}

		void skipEmpty() {
			while ( index < SIZE ) {
				if ( pool->nodes[index].allocated )
					return;
				index++;
			}
		}

	};

	iterator begin() {
		iterator iter{ this, 0 };
		iter.skipEmpty();
		return iter;
	}

	iterator end() {
		iterator iter{ this, SIZE };
		return iter;
	}

};

