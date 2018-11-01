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
		entities.insert( this );
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
	list.erase( value );
	//printf( "remove before %d\n", list.size() );
	//auto first = std::remove( list.begin(), list.end(), value );
	//list.erase( first, list.end() );
	//printf( "remove after %d\n", list.size() );
}

void destroyNoRemove(Component * component) {
	printf( "destroyNoRemove Component %x\n", (unsigned int)component );
	if ( component == nullptr ) return;
	delete component;
	printf( "destroyNoRemove Component %x done\n", (unsigned int)component );
}

void destroy(Component * component) {
	if ( component == nullptr ) return;
	if ( Entity * e = component->entity )
		remove( e->components, component );
	destroyNoRemove( component );
}

template<typename T>
void destroyAll(T & list) {
	
	//printf( "destroyAll %d\n", list.size() );

	// was for vector
	//while ( list.size() > 0 )
	//	destroy( list[0] );

	for ( auto value : list )
		destroyNoRemove( value );

	//printf( "destroyAll %d half\n", list.size() );

	list.clear();

	//printf( "destroyAll %d done\n", list.size() );

}

void destroyNoRemove(Entity * entity) {
	if ( entity == nullptr ) return;
	destroyAll( entity->components );
	delete entity;
}

void destroy(Entity * entity) {
	printf( "destroy Entity\n");
	if ( entity == nullptr ) return;
	remove( entities, entity );
	destroyNoRemove( entity );
}

template<typename T>
T * addComponent(Entity * entity) {
	T * component = new T();
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

	for ( int i = 0; i < 8; i++ ) {
		Entity * e = new Entity();
		if ( rand() % 10 < 5 ) {
			Face * face = addComponent<Face>( e );
		}
	}

	for ( auto face : getComponents<Face>() ) {
		printf( "update face entity %d\n", face->entity->id );
	}

	destroyAll( entities );

	assert( entities.size() == 0 );

	return 0;

}
