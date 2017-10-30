/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GraphicsComponent.cpp
 * Author: treyr3
 * 
 * Created on October 30, 2017, 3:04 PM
 */

#include "GraphicsComponent.h"

GraphicsComponent::GraphicsComponent(GameObject* _owner,
        Ogre::SceneManager* _sceneMgr, Ogre::String _name,
        Ogre::Vector3 _position, Ogre::Vector3 _scale,
        std::string _meshName, std::string _materialName = NULL) :
    Component(_owner),
        sceneMgr(_sceneMgr),
        name(_name),
        meshName(_meshName),
        materialName(_materialName),
        initialPosition(_position),
        initialScale(_scale)
{
    geom = sceneMgr->createEntity(meshName);
    
    if (materialName)
        geom->setMaterialName(materialName);
    
    rootNode = sceneMgr->getRootSceneNode()->createChildSceneNode(name);
    rootNode->attachObject(geom);
    rootNode->setPosition(initialPosition);
    rootNode->setScale(initialScale); 
}
