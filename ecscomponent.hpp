#pragma once

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
