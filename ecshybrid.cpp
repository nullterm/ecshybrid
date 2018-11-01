#include <stdio.h>

#include <vector>
#include <algorithm>

static int nextEntityId = 0;

struct Entity;
struct Component;

using EntityList = std::vector<Entity *>;
using ComponentList = std::vector<Component *>;

EntityList entities;
ComponentList components;

struct Component {

	Entity * entity = nullptr;

	Component() {
		//printf( "construct Component\n" );
	}

	virtual ~Component() {
		//printf( "destruct Component\n" );
	}

};

struct Entity {

	int id = -1;

	ComponentList components;

	Entity() {
		id = nextEntityId;
		nextEntityId++;
		printf( "construct Entity %d\n", id );
		entities.push_back( this );
	}

	~Entity() {
		//printf( "destruct Entity %d\n", id );
	}

};

struct Face : Component {

	int mouth = 0;

	Face() {
		printf( "construct Face\n" );
	}

	virtual ~Face() {
		//printf( "destruct Face\n" );
	}

};

template< typename T1, typename T2 >
void remove(T1 & list, T2 value) {
	//printf( "remove before %d\n", list.size() );
	auto first = std::remove( list.begin(), list.end(), value );
	list.erase( first, list.end() );
	//printf( "remove after %d\n", list.size() );
}

void destroy(Component * component) {
	//printf( "destroy Component %x\n", (unsigned int)component );
	if ( component == nullptr ) return;
	remove( components, component );
	if ( Entity * e = component->entity ) {
		remove( e->components, component );
	}
	delete component;
}

template<typename T>
void destroyAll(T & list) {
	//printf( "destroyAll %d\n", list.size() );
	while ( list.size() > 0 )
		destroy( list[0] );
}

void destroy(Entity * entity) {
	//printf( "destroy Entity\n");
	if ( entity == nullptr ) return;
	remove( entities, entity );
	destroyAll( entity->components );
	delete entity;
}

template<typename T>
T * addComponent(Entity * entity) {
	T * component = new T();
	component->entity = entity;
	components.push_back( component );
	entity->components.push_back( component );
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

	for ( int i =0; i < 8; i++ ) {
		Entity * e = new Entity();
		if ( rand() % 10 < 5 ) {
			Face * face = addComponent<Face>( e );
		}
	}

	for ( auto face : getComponents<Face>() ) {
		printf( "update face entity %d\n", face->entity->id );
	}

	destroyAll( entities );

	return 0;
}
