#ifndef __Ball_h_
#define __Ball_h_

#include "GameObject.h"
#include "PhysicsComponent.h"
#include "GraphicsComponent.h"
#include "ComponentNotFoundException.h"

class Simulator;

namespace BALL_DEFAULT {
    namespace GRAPHICS {
        const float       SCALE_FACTOR_XYZ = 0.05;
        const std::string MESH             = "sphere.mesh";
    }
    
    namespace PHYSICS {
        const int          MASS             = 5,
                           RADIUS           = 5;
        const btVector3    INERTIA          = btVector3(0, 0, 0);
        const btQuaternion ROTATION         = btQuaternion(0.0f, 0.0f, 0.0f, 1);
        const float        RESTITUTION      = 0.8,
                           FRICTION         = 1.0,
                           ROLLING_FRICTION = 1.0,
                           LINEAR_DAMPING   = 0.05,
                           ANGULAR_DAMPING  = 0.05;
        const bool         KINEMATIC        = false,
                           NEEDS_UPDATES    = false;
    }
}

class Ball : public GameObject {
protected:
    btScalar initialX, initialY, initialZ;
    
public:
    Ball(Ogre::SceneManager* _sceneMgr, Simulator* _simulator, 
        btScalar x, btScalar y, btScalar z, 
        std::string _name, 
        std::map<size_t, objType> &typeMap,
        std::map<Ogre::SceneNode*, Ball*>& pocketMap,
        std::string color,
        bool isRed,
        bool isCue = false);

    Ogre::Vector3 getPosition();
    
    bool redBall;

    btRigidBody* getBody();
    Simulator* getSimulator();
//
    Ogre::SceneNode* getNode();
    
    void removeFromWorld (void);
    void resetCueBall (void);
    
    void setAsCue (void);
};

#endif
