#pragma once

struct Entity {

	static int nextEntityId;

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
