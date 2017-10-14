#include "Simulator.h"
#include "GameObject.h"

void Simulator::initObjects() {
    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
    overlappingPairCache = new btDbvtBroadphase();
    solver = new btSequentialImpulseConstraintSolver();
    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,
            overlappingPairCache,
            solver,
            collisionConfiguration);

    dynamicsWorld->setGravity(btVector3(0, 0, 0));
}

void Simulator::addObject (GameObject* o, collisionType coltype, int collidesWith) { 
	objList.push_back(o); 
	dynamicsWorld->addRigidBody(o->getBody(), coltype, collidesWith);       
}
