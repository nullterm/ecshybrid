#pragma once

#include <assert.h>
#include <stdio.h>

#include <unordered_set>
#include <vector>
#include <algorithm>

#include "ecspool.hpp"

struct Entity;
struct Component;

using EntityList = std::unordered_set<Entity *>;
using ComponentList = std::unordered_set<Component *>;

extern EntityList entities;
extern ComponentList components;

void destroy(Entity * entity);
void destroy(Component * component);

template<typename T>
void destroyAll(T & list) {

	// we make a copy so that when removed from list, list iteration isn't invalidated
	T copy = list;

	for ( auto value : copy )
		destroy( value );

	list.clear();

}


#include "ecscomponent.hpp"
#include "ecsentity.hpp"

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
