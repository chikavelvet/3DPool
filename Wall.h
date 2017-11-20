/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Wall.h
 * Author: treyr3
 *
 * Created on October 15, 2017, 1:23 PM
 */

#ifndef WALL_H
#define WALL_H

#include "GameObject.h"

namespace WALL_DEFAULT {
    namespace PHYSICS {
        const int           MASS = 0;
        const btVector3     INERTIA = btVector3(0, 0, 0);
        const btQuaternion  ROTATION = btQuaternion(0, 0, 0, 1);
        const float         RESTITUTION = 1.0,
                            FRICTION = 0.5,
                            ROLLING_FRICTION = 1.0,
                            LINEAR_DAMPING = 0.0,
                            ANGULAR_DAMPING = 0.0;
        const bool          KINEMATIC = false,
                            NEEDS_UPDATES = false;
        const collisionType COLTYPE = COL_WALL,
                            COLLIDES_WITH = collisionType(COL_BALL | COL_CUEBALL);
    }
    
    namespace GRAPHICS {
        const Ogre::Vector3 SCALE = Ogre::Vector3(1.0, 1.0, 1.0);
    }
}

class Wall : public GameObject {
protected:
    Wall(Simulator* _simulator, 
        btVector3 _origin, 
        Ogre::Vector3 normal,
        float dim1, float dim2);
    Wall(Ogre::String _name, 
        Ogre::SceneManager* _sceneMgr, 
        Ogre::Vector3 _position, 
        float dim1, float dim2,
        Ogre::Vector3 normal, 
        Ogre::Vector3 up, 
        std::string color);
public:
    static Wall* MakePhysicalWall (
        Simulator* _simulator, 
        btVector3 _origin, 
        Ogre::Vector3 normal,
        float dim1, float dim2);
    
    static Wall* MakeVisualWall (
        Ogre::String _name, 
        Ogre::SceneManager* _sceneMgr, 
        Ogre::Vector3 _position, 
        float dim1, float dim2,
        Ogre::Vector3 normal, 
        Ogre::Vector3 up, 
        std::string color);
};

#endif /* WALL_H */

