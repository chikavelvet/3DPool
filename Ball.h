#ifndef __Ball_h_
#define __Ball_h_

#include "GameObject.h"

class Simulator;

class Ball : public GameObject {
public:
    Ball(Ogre::SceneManager* mSceneMgr, Simulator* physicsEngine, 
        btScalar x, btScalar y, btScalar z, 
        std::string name, 
        std::map<size_t, objType> &typeMap,
        bool isCue = false);

    Ogre::Vector3 getPosition();

    btRigidBody* getRigidBody(){ return body; }

    Ogre::Entity* getOgreEntity(){ return geom; }

    Ogre::SceneNode* getOgreSceneNode(){ return rootNode; }
    
    void setAsCue (void);
};

#endif