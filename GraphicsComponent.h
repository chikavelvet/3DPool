/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GraphicsComponent.h
 * Author: treyr3
 *
 * Created on October 30, 2017, 3:04 PM
 */

#ifndef GRAPHICSCOMPONENT_H
#define GRAPHICSCOMPONENT_H

#include "Component.h"

#include <OgreSceneManager.h>
#include <OgreEntity.h>

class GraphicsComponent : public Component {
public:
    GraphicsComponent (GameObject* _owner, 
            Ogre::SceneManager* _sceneMgr,
            Ogre::String _name, 
            Ogre::Vector3 _position, Ogre::Vector3 _scale);
    
    Ogre::String name;
    Ogre::SceneManager* sceneMgr;
    Ogre::SceneNode* rootNode;
    Ogre::Entity* geom;
    std::string meshName;
    std::string materialName;
    Ogre::Vector3 initialPosition;
    Ogre::Vector3 initialScale;
};

#endif /* GRAPHICSCOMPONENT_H */

