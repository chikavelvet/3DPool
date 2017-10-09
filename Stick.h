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
        node->scale(0.01, 0.01, 0.5);
         
        //create the new shape, and tell the physics that is a sphere
        colShape = new btBoxShape(btVector3(1, 1, 23));
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

    bool readjustStickToCueball(bool& adjustingStick, Simulator* physicsEngine){
        bool cueStickStopped = (fabs(body->getLinearVelocity().length()) < 0.5f) && (fabs(body->getTotalForce().length()) < 0.5f);        
        bool cueBallStopped = (fabs(cueBall->getLinearVelocity().length()) < 0.5f) && (fabs(cueBall->getTotalForce().length()) < 0.5f);

        if(cueStickStopped){
            physicsEngine->getDynamicsWorld()->removeRigidBody(body);
            // body->activate(false);
            entity->setVisible(false);
        }

        bool turnIsOver = cueBallStopped && cueStickStopped;
        if(!turnIsOver) return false;
        // std::cout << turnIsOver << std::boolalpha << std::endl;

        btVector3 ballPos = cueBall->getCenterOfMassPosition();
        btTransform newTransform = body->getCenterOfMassTransform();
        newTransform.setOrigin(btVector3(ballPos.getX(), ballPos.getY(), ballPos.getZ() + cueStickMin));
        body->setCenterOfMassTransform(newTransform);
        adjustingStick = false;
        // body->activate(true);
        physicsEngine->getDynamicsWorld()->addRigidBody(body);    
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
            body->applyCentralImpulse( btVector3( 0.f, 0.f, -powerMultiplier * cueStickTotal) );
        }
        cueStickTotal = 0;
        cueStickDelta = 0;
        adjustingStick = true;
        hitBall = false;
    }


    void rotateToMouseInput(float& deltaRotationX, float& deltaRotationY){
        rotateToMouseXInput(deltaRotationX);
        rotateToMouseYInput(deltaRotationY);
    }

    void rotateToMouseXInput(float& deltaRotationX){
        btVector3 difference = cueBall->getCenterOfMassPosition() - body->getCenterOfMassPosition();
        body->translate(difference);

         //make X rotation
        btQuaternion rotation(btVector3(0, 1, 0),btRadians(deltaRotationX));
        rotation *= body->getOrientation();
       
        //actually apply the rotations
        body->setCenterOfMassTransform(btTransform(rotation, body->getCenterOfMassPosition()));
        
        btVector3 invDifference = difference;
        invDifference = invDifference.rotate(btVector3(0, 1, 0),btRadians(deltaRotationX));
        body->translate(-invDifference);

        //reset delta rotations
        deltaRotationX = 0.0f;
    }

    void rotateToMouseYInput(float& deltaRotationY){
        btVector3 difference = cueBall->getCenterOfMassPosition() - body->getCenterOfMassPosition();
        body->translate(difference);

        //make Y rotation
        btQuaternion rotation(btVector3(1, 0, 0),btRadians(deltaRotationY));
        rotation *= body->getOrientation();
       
        //actually apply the rotations
        body->setCenterOfMassTransform(btTransform(rotation, body->getCenterOfMassPosition()));
        
        btVector3 invDifference = difference;
        invDifference = invDifference.rotate(btVector3(1, 0, 0), btRadians(deltaRotationY));
        body->translate(-invDifference);

        //reset delta rotations
        deltaRotationY = 0.0f;
    }


};

#endif