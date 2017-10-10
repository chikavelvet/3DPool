#ifndef __Stick_h_
#define __Stick_h_

#include "BaseApplication.h"
#include "GameObject.h"

// class GameObject;

class Stick: public GameObject {

private:
    float cueStickMax;
    float cueStickMin;
    float powerMultiplier;
    btRigidBody* cueBall;

public:
	~Stick();
	Stick();

	Stick(Ogre::SceneManager* _sceneMgr, Simulator* _simulator, btScalar x, btScalar y, btScalar z, std::string _name, float _cueStickMax, float _cueStickMin, float _powerMultiplier, btRigidBody* _cueBall) {
        kinematic = false;
        simulator = _simulator;
        name = _name;
        sceneMgr = _sceneMgr;
        needsUpdates = false;

        geom = sceneMgr->createEntity("cube.mesh"); 
        rootNode = sceneMgr->getRootSceneNode()->createChildSceneNode(name);
        rootNode->attachObject(geom);
        rootNode->setPosition(x, y, z);
        rootNode->scale(0.01, 0.01, 0.5);
         
        //create the new shape, and tell the physics that is a sphere
        shape = new btBoxShape(btVector3(1, 1, 23));
        simulator->getCollisionShapes().push_back(shape);
        tr.setIdentity();
        tr.setRotation(btQuaternion(0.0f, 0.0f, 0.0f, 1));
         
        //set the mass of the object. a mass of "0" means that it is an immovable object
        mass = 10;
        inertia = btVector3(0,0,0);
         
        btVector3 initialPosition(x, y, z);
        tr.setOrigin(initialPosition);
        shape->calculateLocalInertia(mass, inertia);
         
        //actually contruvc the body and add it to the dynamics world
        // motionState = new OgreMotionState(tr, rootNode); 
        motionState = new btDefaultMotionState(tr); 
        restitution = 1.0;
        friction = 1.0;
        linearDamping = 0;
        addToSimulator();
        // simulator->getDynamicsWorld()->addRigidBody(body);

        cueStickMax = _cueStickMax;
        cueStickMin = _cueStickMin;
        powerMultiplier = _powerMultiplier;
        cueBall = _cueBall;
    }

    bool readjustStickToCueball(bool& adjustingStick){
        bool cueStickStopped = (fabs(body->getLinearVelocity().length()) < 0.5f) && (fabs(body->getTotalForce().length()) < 0.5f);        
        bool cueBallStopped = (fabs(cueBall->getLinearVelocity().length()) < 0.5f) && (fabs(cueBall->getTotalForce().length()) < 0.5f);

        if(cueStickStopped){
            simulator->getDynamicsWorld()->removeRigidBody(body);
            // body->activate(false);
            geom->setVisible(false);
        }

        bool turnIsOver = cueBallStopped && cueStickStopped;
        if(!turnIsOver) return false;
        // std::cout << turnIsOver << std::boolalpha << std::endl;

        btVector3 ballPos = cueBall->getCenterOfMassPosition();
        btTransform newTransform (btQuaternion(0, 0, 0, 1), btVector3(ballPos.getX(), ballPos.getY(), ballPos.getZ() + cueStickMin));
        body->setCenterOfMassTransform(newTransform);
        adjustingStick = false;
        // body->activate(true);
        simulator->getDynamicsWorld()->addRigidBody(body);
        geom->setVisible(true);
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

                btVector3 movement = btVector3(body->getCenterOfMassPosition()-cueBall->getCenterOfMassPosition()).normalize() * cueStickDelta;

                std::cout << movement.length() << std::endl;

                body->translate(movement);
            }
        }

        cueStickTotal += cueStickDelta;
        cueStickDelta = 0;
    }


    void releaseStick(bool& adjustingStick, bool& hitBall, float& cueStickTotal, float& cueStickDelta) {
        if(cueStickTotal >= cueStickMin){
            body->activate(true);
            btVector3 movement = btVector3(body->getCenterOfMassPosition()-cueBall->getCenterOfMassPosition()).normalize() * -powerMultiplier * cueStickTotal;    
            body->applyCentralImpulse(movement);
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

    btRigidBody* getRigidBody(){ return body; }
    Ogre::Vector3 getPosition(){
            btVector3 btPos = body->getCenterOfMassPosition();
            return Ogre::Vector3(float(btPos.x()), float(btPos.y()), float(btPos.z()));
    }

    Ogre::Entity* getOgreEntity(){
        return geom;
    }

    Ogre::SceneNode* getOgreSceneNode(){
        return rootNode;
    }



};

#endif