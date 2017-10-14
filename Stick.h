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
            btRigidBody* _cueBall, std::map<size_t, objType> &typeMap);

    bool readjustStickToCueball (bool& adjustingStick);

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