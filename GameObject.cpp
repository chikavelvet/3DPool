#include "GameObject.h"

GameObject::GameObject (Ogre::String _name, Ogre::SceneManager* _sceneMgr,
            Simulator* _simulator, btScalar _mass, btVector3 _inertia, 
            btScalar _restitution, btScalar _friction, 
            btScalar _linearDamping, btScalar _angularDamping,
            bool _kinematic, bool _needsUpdates,
            collisionType _coltype, int _collidesWith) : 
        name(_name),
        sceneMgr(_sceneMgr),
        simulator(_simulator),
        mass(_mass),
        inertia(_inertia),
        restitution(_restitution),
        friction(_friction),
        linearDamping(_linearDamping),
        angularDamping(_angularDamping),
        kinematic(_kinematic),
        needsUpdates(_needsUpdates),
        coltype(_coltype),
        collidesWith(_collidesWith)
{
    
}

GameObject::GameObject ()
{
    
}

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
        
	simulator->addObject(this, coltype, collidesWith);
}

/*using motionstate is recommended, it provides interpolation capabilities, 
	and only synchronizes 'active' objects*/
void GameObject::updateTransform()
{
	
}

void GameObject::removeObject (void) {
    simulator->getDynamicsWorld()->removeRigidBody(body);
    geom->setVisible(false);
}