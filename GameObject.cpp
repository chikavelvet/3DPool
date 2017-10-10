#include "GameObject.h"

//Add the game object to the simulator
void GameObject::addToSimulator() {
	
	updateTransform();
	
	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	if (mass != 0.0f) 
		shape->calculateLocalInertia(mass, inertia);
	
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, shape, inertia);
	rbInfo.m_restitution = restitution;
	rbInfo.m_friction = friction;
	body = new btRigidBody(rbInfo);
	body->setUserPointer(rootNode);
        body->setDamping(linearDamping, angularDamping);
	
	if (kinematic) {
		body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
		body->setActivationState(DISABLE_DEACTIVATION);
	}
        
	simulator->addObject(this);
}

/*using motionstate is recommended, it provides interpolation capabilities, 
	and only synchronizes 'active' objects*/
void GameObject::updateTransform()
{
	
}
