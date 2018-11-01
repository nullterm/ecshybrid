#pragma once

template<typename T>
struct Pool {

	static const int BLOCK_SIZE = sizeof(T);
	static const int MAX_COUNT = 256;

	struct Node {
		bool allocated = false;
		unsigned char data[BLOCK_SIZE];
	};

	Node nodes[MAX_COUNT];

	void * allocate(size_t size) {
		assert( size <= BLOCK_SIZE );
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
			while ( index < MAX_COUNT ) {
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
		iterator iter{ this, MAX_COUNT };
		return iter;
	}

};

