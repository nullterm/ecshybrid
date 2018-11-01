#include "ecs.hpp"
#include "ecspool.hpp"



// Face - an example component that allocates from a pool

struct Face : Component {

	static Pool<Face> pool;

	int mouth = 0;

	void * operator new(size_t size) {
		return pool.allocate( size );
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



int main() {

	printf( "ecshybrid.cpp - Blaine Hodge <blaine@stingergames.com>\n" );

	Entity * randoFace = nullptr;



	// create a bunch of entities, randomly assign some faces
	// new entities automatically enter into "entities"
	for ( int i = 0; i < 8; i++ ) {
		Entity * e = new Entity();
		if ( rand() % 10 < 5 ) {
			Face * face = addComponent<Face>( e );
			randoFace = e;
		}
	}



	// iterate all faces using pool's iterator and range based for
	// simplest and cleanest way to traverse all the active components
	for ( auto face : Face::pool ) {
		printf( "update face=%d entity %d\n", face->mouth, face->entity->id );
	}



	// iterate all faces using pool's forEach
	Face::pool.forEach( [](Face * face) {
		printf( "update face=%d entity %d\n", face->mouth, face->entity->id );
	} );



	// delete one of the random face components
	if ( randoFace )
		delete randoFace;



	// iterate all faces using getAllComponents which returns a vector of all faces from entities
	// less desirable as it creates and populates a vector, which is then returned
	for ( auto face : getAllComponents<Face>() ) {
		printf( "update face=%d entity %d\n", face->mouth, face->entity->id );
	}



	destroyAll( entities );

	assert( entities.size() == 0 );

	return 0;

}
