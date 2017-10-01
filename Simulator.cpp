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
}

void Simulator::addObject (GameObject* o) { 
	objList.push_back(o); 
	dynamicsWorld->addRigidBody(o->getBody());       
}