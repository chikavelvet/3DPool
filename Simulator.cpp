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

void Simulator::addObject (GameObject* o) { 
	objList.push_back(o); 
	dynamicsWorld->addRigidBody(o->getBody());       
}

void Simulator::trackRigidBodyWithName(btRigidBody* body, std::string name){
}
