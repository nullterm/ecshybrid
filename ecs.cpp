#include "ecs.hpp"

EntityList entities;
ComponentList components;

int Entity::nextEntityId = 0;

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

