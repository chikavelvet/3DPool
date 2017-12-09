/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Pocket.h
 * Author: treyr3
 *
 * Created on October 14, 2017, 5:47 PM
 */

#ifndef POCKET_H
#define POCKET_H

#include "GameObject.h"

class Simulator;

namespace POCKET_DEFAULT {
    namespace GRAPHICS {
        const float       SCALE_FACTOR_XYZ = 0.27;
        const std::string MESH = "sphere.mesh",
                          MATERIAL = "Example/Purple";
    }

    namespace PHYSICS {
        const int           MASS = 0;
        const btVector3     INERTIA = btVector3(0, 0, 0);
        const btQuaternion  ROTATION = btQuaternion(0, 0, 0, 1);
        const float         RESTITUTION = 0.0,
                            FRICTION = 0.0,
                            LINEAR_DAMPING = 0.0,
                            ANGULAR_DAMPING = 0.0;
        const bool          KINEMATIC = false,
                            NEEDS_UPDATES = false;
        const collisionType COLTYPE = COL_POCKET,
                            COLLIDES_WITH = collisionType(COL_CUEBALL | COL_BALL);
    }
}

class Pocket : public GameObject {
public:
    Pocket (Ogre::SceneManager* mSceneMgr,
            Simulator* _simulator,
            btScalar x, btScalar y, btScalar z,
            std::string _name,
            std::map<size_t, objType>& typeMap);
};

#endif /* POCKET_H */

