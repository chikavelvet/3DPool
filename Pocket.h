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

class Pocket : public GameObject {
public:
    Pocket (Ogre::SceneManager* mSceneMgr,
            Simulator* _simulator,
            btScalar x, btScalar y, btScalar z,
            std::string _name,
            std::map<size_t, objType>& typeMap);
};

#endif /* POCKET_H */

