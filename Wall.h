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

class Wall : public GameObject {
protected:
    Wall(Ogre::String _name, Ogre::SceneManager* _sceneMgr,
            Simulator* _simulator,
            Ogre::Vector3 normal,
            float d1, float d2,
            float posx, float posy, float posz,
            float btx, float bty, float btz,
            Ogre::Vector3 up, std::map<size_t, objType>& typeMap);
public:
    
    static Wall* MakePhysicalWall ();
    
    static Wall* MakeVisualWall ();
    
    static Ogre::SceneNode* makeVisualPlane (Ogre::SceneManager* mSceneMgr, Ogre::Vector3 normal,
            std::string name, 
            float d1, float d2, 
            Ogre::Vector3 up, 
            float posx, float posy, float posz, 
            std::string color);
};

#endif /* WALL_H */

