#ifndef __Stick_h_
#define __Stick_h_

#include "BaseApplication.h"


class Stick {

private:
	Ogre::Entity* entity;
	Ogre::SceneNode* node;
	btCollisionShape* colShape;
	btScalar mass;
	btVector3 localInertia;
	btRigidBody *body;
    float cueStickMax;
    float cueStickMin;
    float powerMultiplier;
    btRigidBody* cueBall;

public:
	~Stick();
	Stick();

	Stick(Ogre::SceneManager* mSceneMgr, Simulator* physicsEngine, btScalar x, btScalar y, btScalar z, std::string name, float _cueStickMax, float _cueStickMin, float _powerMultiplier, btRigidBody* _cueBall) {
        entity = mSceneMgr->createEntity("cube.mesh"); 
        node = mSceneMgr->getRootSceneNode()->createChildSceneNode(name);
        node->attachObject(entity);
        node->setPosition(x, y, z);
        node->scale(0.05, 0.05, 0.5);
         
        //create the new shape, and tell the physics that is a sphere
        colShape = new btBoxShape(btVector3(5, 5, 23));
        physicsEngine->getCollisionShapes().push_back(colShape);
        btTransform startTransform;
        startTransform.setIdentity();
        startTransform.setRotation(btQuaternion(0.0f, 0.0f, 0.0f, 1));
         
        //set the mass of the object. a mass of "0" means that it is an immovable object
        mass = 10;
        localInertia = btVector3(0,0,0);
         
        btVector3 initialPosition(x, y, z);
        startTransform.setOrigin(initialPosition);
        colShape->calculateLocalInertia(mass, localInertia);
         
        //actually contruvc the body and add it to the dynamics world
        btDefaultMotionState *myMotionState = new btDefaultMotionState(startTransform); 
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
        body = new btRigidBody(rbInfo);
        body->setUserPointer(node);
        body->setRestitution(2.0);
        body->setFriction(btScalar(1.0));
        body->setRollingFriction(btScalar(1.0));
        body->setDamping(0, 0);

        physicsEngine->getDynamicsWorld()->addRigidBody(body);
        physicsEngine->trackRigidBodyWithName(body, name);    

        cueStickMax = _cueStickMax;
        cueStickMin = _cueStickMin;
        powerMultiplier = _powerMultiplier;
        cueBall = _cueBall;
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

    bool readjustStickToCueball(bool& adjustingStick){
        float cueBallSpeed = fabs(cueBall->getLinearVelocity().length());
        float cueStickSpeed = fabs(body->getLinearVelocity().length());
        if(cueStickSpeed<0.5f){
            body->activate(false);
            entity->setVisible(false);
        }

        bool turnIsOver = (cueStickSpeed<0.5f) && (cueBallSpeed<0.5f);
        if(!turnIsOver) return false;
        // std::cout << turnIsOver << std::boolalpha << std::endl;

        btVector3 ballPos = cueBall->getCenterOfMassPosition();
        btTransform newTransform = body->getCenterOfMassTransform();
        newTransform.setOrigin(btVector3(ballPos.getX(), ballPos.getY(), ballPos.getZ() + cueStickMin));
        body->setCenterOfMassTransform(newTransform);
        adjustingStick = false;
        body->activate(true);
        entity->setVisible(true);
        return true;
    }

    void chargeStick(bool adjustingStick, float& cueStickTotal, float& cueStickDelta, bool LMBDown){
        if(adjustingStick)
            return;

        body->activate(true);
        
        if(cueStickTotal < cueStickMax && cueStickTotal > cueStickMin) {
            if(cueStickTotal + cueStickDelta > cueStickMax)
                cueStickDelta = cueStickMax - cueStickTotal;
            if(cueStickTotal + cueStickDelta < cueStickMin){
                cueStickTotal = cueStickMin;
                cueStickDelta = 0.0f;
                return;
            }
            if(LMBDown) {
                body->translate(btVector3(0.f, 0.f, cueStickDelta));
            }
        }

        cueStickTotal += cueStickDelta;
        cueStickDelta = 0;
    }


    void releaseStick(bool& adjustingStick, bool& hitBall, float& cueStickTotal, float& cueStickDelta) {
        if(cueStickTotal >= cueStickMin){
            body->activate(true);
            body->applyCentralImpulse( btVector3( 0.f, 0.f, -powerMultiplier * cueStickTotal * cueStickTotal) );
        }
        cueStickTotal = 0;
        cueStickDelta = 0;
        adjustingStick = true;
        hitBall = false;
    }


};

#endif