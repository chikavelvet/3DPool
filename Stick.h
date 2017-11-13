#ifndef __Stick_h_
#define __Stick_h_

#include "GameObject.h"
#include <OgreParticleIterator.h>
#include <OgreParticleSystem.h>
#include <OgreParticleSystemManager.h>
#include <OgreParticle.h>


class GameObject;

class Stick : public GameObject {
private:
    float cueStickMax;
    float cueStickMin;
    float powerMultiplier;
    btRigidBody* cueBall;
    Ogre::SceneNode* cueBallNode;
    Ogre::ParticleSystem* guideLineParticle;
    Ogre::SceneNode* guideLineNode;


public:
    Stick(Ogre::SceneManager* _sceneMgr, Simulator* _simulator, 
            btScalar x, btScalar y, btScalar z, 
            std::string _name, 
            float _cueStickMax, float _cueStickMin, float _powerMultiplier, 
            btRigidBody* _cueBall, std::map<size_t, objType> &typeMap, Ogre::SceneNode* _cueBallNode);

    bool readjustStickToCueball (bool& adjustingStick, bool ballsStopped);

    void chargeStick (bool adjustingStick, float& cueStickTotal, float& cueStickDelta, bool LMBDown);

    void releaseStick (bool& adjustingStick, bool& hitBall, float& cueStickTotal, float& cueStickDelta);

    void rotateToMouseInput (float& deltaRotationX, float& deltaRotationY);

    void rotateToMouseXInput(float& deltaRotationX);

    void rotateToMouseYInput(float& deltaRotationY);

    btRigidBody* getRigidBody() { return body; }

    Ogre::Entity* getOgreEntity(){ return geom; }

    Ogre::SceneNode* getOgreSceneNode(){ return rootNode; }
    
    Ogre::Vector3 getPosition();
};

#endif // ifndef __Stick_h_