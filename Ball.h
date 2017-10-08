#ifndef __Ball_h_
#define __Ball_h_

#include "BaseApplication.h"


class Ball {

private:
	Ogre::Entity* entity;
	Ogre::SceneNode* node;
	btCollisionShape* colShape;
	btScalar mass;
	btVector3 localInertia;
	btRigidBody *body;

public:
	~Ball();
	Ball();

	Ball(Ogre::SceneManager* mSceneMgr, Simulator* physicsEngine, btScalar x, btScalar y, btScalar z, std::string name) {
        //----------------make a cube-------------------//
        entity = mSceneMgr->createEntity("sphere.mesh"); 
        node = mSceneMgr->getRootSceneNode()->createChildSceneNode(name);
        node->attachObject(entity);
        node->setPosition(x, y, z);
        node->scale(0.1, 0.1, 0.1);
         
        //create the new shape, and tell the physics that is a sphere
        colShape = new btSphereShape(9);
        physicsEngine->getCollisionShapes().push_back(colShape);
        btTransform startTransform;
        startTransform.setIdentity();
        startTransform.setRotation(btQuaternion(0.0f, 0.0f, 0.0f, 1));
         
        //set the mass of the object. a mass of "0" means that it is an immovable object
        mass = 5;
        localInertia = btVector3(0,0,0);
         
        btVector3 initialPosition(x, y, z);
        startTransform.setOrigin(initialPosition);
        colShape->calculateLocalInertia(mass, localInertia);
         
        //actually contruvc the body and add it to the dynamics world
        btDefaultMotionState *myMotionState = new btDefaultMotionState(startTransform); 
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
        body = new btRigidBody(rbInfo);
        body->setUserPointer(node);
        body->setRestitution(0.8);
        body->setFriction(btScalar(1.0));
        body->setRollingFriction(btScalar(1.0));
        body->setDamping(0.03, 0);

        physicsEngine->getDynamicsWorld()->addRigidBody(body);
        physicsEngine->trackRigidBodyWithName(body, name);    
	}

    Ogre::Vector3 getPosition(){
            btVector3 btPos = body->getCenterOfMassPosition();
            return Ogre::Vector3(float(btPos.x()), float(btPos.y()), float(btPos.z()));
    }

	btRigidBody* getRigidBody(){
		return body;
	}

	Ogre::Entity* getOgreEntity(){
		return entity;
	}

	Ogre::SceneNode* getOgreSceneNode(){
		return node;
	}


};

#endif