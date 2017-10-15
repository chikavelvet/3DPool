#ifndef __Ball_h_
#define __Ball_h_

#include "GameObject.h"

class Simulator;

class Ball : public GameObject {
public:
    Ball(Ogre::SceneManager* _sceneMgr, Simulator* _simulator, 
        btScalar x, btScalar y, btScalar z, 
        std::string _name, 
        std::map<size_t, objType> &typeMap,
        std::map<Ogre::SceneNode*, Ball*>& pocketMap,
        bool isCue = false);

    Ogre::Vector3 getPosition();

    btRigidBody* getRigidBody(){ return body; }

    Ogre::Entity* getOgreEntity(){ return geom; }

    Ogre::SceneNode* getOgreSceneNode(){ return rootNode; }
    
    void removeFromWorld (void);
    void resetCueBall (void);
    
    void setAsCue (void);
    
private:
    btScalar initialX, initialY, initialZ;
};

#endif