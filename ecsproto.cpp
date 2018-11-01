#include <assert.h>
#include <stdio.h>

#include <unordered_set>
#include <vector>
#include <algorithm>

static int nextEntityId = 0;

struct Entity;
struct Component;

using EntityList = std::unordered_set<Entity *>;
using ComponentList = std::unordered_set<Component *>;

EntityList entities;
ComponentList components;

void destroy(Entity * entity);
void destroy(Component * component);

template<typename T>
void destroyAll(T & list) {

	// we make a copy so that when removed from list, list isn't invalidated
	T copy = list;

	for ( auto value : copy )
		destroy( value );

	list.clear();

}

struct Component {

	Entity * entity = nullptr;

	Component(Entity * e) {
		entity = e;
		components.insert( this );
	}

	virtual ~Component() {
		components.erase( this );
	}

};

struct Entity {

	int id = -1;

	ComponentList components;

	Entity() {
		id = nextEntityId;
		nextEntityId++;
		printf( "construct Entity %d\n", id );
		entities.insert( this );
	}

	~Entity() {
		destroyAll( components );
		entities.erase( this );
	}

};

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

struct Face : Component {

	static Pool<Face> pool;

	int mouth = 0;

	void * operator new(size_t size) {
		return pool.allocate();
	}

	void operator delete(void * data) {
		return pool.free( data );
	}

	Face(Entity * e) : Component( e ) {
		printf( "construct Face entity %d\n", entity->id );
		mouth = rand() % 100;
	}

	virtual ~Face() {
		printf( "destruct Face entity %d\n", entity->id );
	}

};

Pool<Face> Face::pool;

template< typename T1, typename T2 >
void remove(T1 & list, T2 value) {
	list.erase( value );
}

void destroy(Component * component) {
	if ( component == nullptr ) return;
	if ( Entity * e = component->entity )
		remove( e->components, component );
	delete component;
}

void destroy(Entity * entity) {
	printf( "destroy Entity\n");
	if ( entity == nullptr ) return;
	remove( entities, entity );
	destroyAll( entity->components );
	delete entity;
}

template<typename T>
T * addComponent(Entity * entity) {
	T * component = new T( entity );
	component->entity = entity;
	entity->components.insert( component );
	return component;
}

template<typename T>
T * getComponent(Entity * entity) {
	for ( Component * c : entity->components )
		if ( T * c2 = dynamic_cast<T *>( c ) )
			return c2;
	return nullptr;
}

template<typename T>
std::vector<T *> getComponents() {
	std::vector<T *> list;
	for ( Entity * entity : entities )
		if ( T * component = getComponent<T>( entity ) )
			list.push_back( component );
	return list;
}

int main() {

	printf( "ecshybrid.cpp - Blaine Hodge <blaine@stingergames.com>\n" );

	Entity * saved = nullptr;

	for ( int i = 0; i < 8; i++ ) {
		Entity * e = new Entity();
		if ( rand() % 10 < 5 ) {
			Face * face = addComponent<Face>( e );
			saved = e;
		}
	}

	Face::pool.forEach( [](Face * face) {
		printf( "update face=%d entity %d\n", face->mouth, face->entity->id );
	} );

	if ( saved )
		delete saved;

	for ( auto face : getComponents<Face>() ) {
		printf( "update face=%d entity %d\n", face->mouth, face->entity->id );
	}

	for ( auto face : Face::pool ) {
		printf( "update face=%d entity %d\n", face->mouth, face->entity->id );
	}

	destroyAll( entities );

	assert( entities.size() == 0 );

	return 0;

}
