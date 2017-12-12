#ifndef __Stick_h_
#define __Stick_h_

#include <OgreParticleIterator.h>
#include <OgreParticleSystem.h>
#include <OgreParticleSystemManager.h>
#include <OgreParticle.h>
#include "Enums.h"
#include <btBulletDynamicsCommon.h>
#include "GameObject.h"

class ThreeDPool;
class Ball;

namespace STICK_DEFAULT {
    namespace GRAPHICS {
        // const float       SCALE_FACTOR_XY = 0.01,
        //                   SCALE_FACTOR_Z  = 0.5;
        //MATERIAL        = "Example/Stick";
       const float       SCALE_FACTOR_X = 1,
                         SCALE_FACTOR_Y = 50,
                         SCALE_FACTOR_Z = 1;
        const std::string MESH            = "Cone.mesh",
                         MATERIAL = "Material.003";
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
protected:
    ThreeDPool* game;

private:
    float cueStickMax;
    float cueStickMin;
    float powerMultiplier;
    Ball* cueBall;
    Ogre::ParticleSystem* guideLineParticle;
    Ogre::SceneNode* guideLineNode;

public:
    Stick(Ogre::SceneManager* _sceneMgr, Simulator* _simulator, 
            btScalar x, btScalar y, btScalar z, 
            std::string _name, 
            float _cueStickMax, float _cueStickMin, float _powerMultiplier, 
            Ball* _cueBall, std::map<size_t, objType> &typeMap, ThreeDPool* _game);

    bool readjustStickToCueball (bool& adjustingStick, bool ballsStopped, const bool& letTurnEnd, const bool& scratched, const bool& scratchedOnBall);

    void chargeStick (bool adjustingStick, float& cueStickTotal, float& cueStickDelta, bool LMBDown);

    void releaseStick (bool& adjustingStick, bool& hitBall, float& cueStickTotal, float& cueStickDelta);

    void rotateToMouseInput (float& deltaRotationX, float& deltaRotationY);

    void rotateToMouseXInput(float& deltaRotationX);

    void rotateToMouseYInput(float& deltaRotationY);

    Ball* closestBallAimingAt();
        
    Ogre::Vector3 getPosition() const;
};

#endif // ifndef __Stick_h_