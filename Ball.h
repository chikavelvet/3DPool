#ifndef __Ball_h_
#define __Ball_h_

#include "GameObject.h"

class Simulator;

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

    Ball(Ogre::SceneManager* mSceneMgr, Simulator* physicsEngine, 
        btScalar x, btScalar y, btScalar z, 
        std::string name, 
        std::map<size_t, objType> &typeMap);

    Ogre::Vector3 getPosition();

    btRigidBody* getRigidBody(){ return body; }

    Ogre::Entity* getOgreEntity(){ return entity; }

    Ogre::SceneNode* getOgreSceneNode(){ return node; }
};

#endif