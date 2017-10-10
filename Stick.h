#ifndef __Stick_h_
#define __Stick_h_

#include "GameObject.h"

class GameObject;

class Stick : public GameObject {

private:
    float cueStickMax;
    float cueStickMin;
    float powerMultiplier;
    btRigidBody* cueBall;

public:
    ~Stick() = default;

    Stick(Ogre::SceneManager* _sceneMgr, Simulator* _simulator, 
            btScalar x, btScalar y, btScalar z, 
            std::string _name, 
            float _cueStickMax, float _cueStickMin, float _powerMultiplier, 
            btRigidBody* _cueBall);

    bool readjustStickToCueball (bool& adjustingStick);

    void chargeStick (bool adjustingStick, float& cueStickTotal, float& cueStickDelta, bool LMBDown);

    void releaseStick(bool& adjustingStick, bool& hitBall, float& cueStickTotal, float& cueStickDelta) {
        if(cueStickTotal >= cueStickMin){
            body->activate(true);
            btVector3 movement = btVector3(body->getCenterOfMassPosition()-cueBall->getCenterOfMassPosition()).normalize() * -powerMultiplier * cueStickTotal;    
            body->applyCentralImpulse(movement);
        }
        cueStickTotal = cueStickMin;
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