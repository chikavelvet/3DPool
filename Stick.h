#ifndef __Stick_h_
#define __Stick_h_

#include "GameObject.h"
#include <OgreParticleIterator.h>
#include <OgreParticleSystem.h>
#include <OgreParticleSystemManager.h>
#include <OgreParticle.h>

class GameObject;

namespace STICK_DEFAULT {
    namespace GRAPHICS {
        const float       SCALE_FACTOR_XY = 0.01,
                          SCALE_FACTOR_Z  = 0.5;
        const std::string MESH            = "cube.mesh",
                          MATERIAL        = "Example/Stick";
    }
    
    namespace PHYSICS {
        const int           MASS            = 10;
        const btVector3     DIMENSIONS      = btVector3(1, 1, 23),
                            INERTIA         = btVector3(0, 0, 0);
        const btQuaternion  ROTATION        = btQuaternion(0, 0, 0, 1);
        const float         RESTITUTION     = 1.0,
                            FRICTION        = 1.0,
                            LINEAR_DAMPING  = 0.0,
                            ANGULAR_DAMPING = 1.0;
        const bool          KINEMATIC       = false, 
                            NEEDS_UPDATES   = false;
        const collisionType COLTYPE         = COL_STICK,
                            COLLIDES_WITH   = COL_CUEBALL;
    }
}

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
    
    btRigidBody* getBody();
    Ogre::SceneNode* getNode();
    
    Ogre::Vector3 getPosition();
};

#endif // ifndef __Stick_h_