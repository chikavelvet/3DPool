/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "Pocket.h"

Pocket::Pocket(Ogre::SceneManager* mSceneMgr, 
        Simulator* _simulator, 
        btScalar x, btScalar y, btScalar z, 
        std::string _name, 
        std::map<size_t,objType>& typeMap)
{
    simulator = _simulator;
    name = _name;
    sceneMgr = mSceneMgr;
    coltype = COL_POCKET;
    collidesWith = COL_CUEBALL | COL_BALL;
    
    kinematic = false;
    needsUpdates = false;
    mass = 0;
    inertia = btVector3(0, 0, 0);
    restitution = 0.0;
    friction = 0.0;
    linearDamping = 0.0;
    angularDamping = 0.0;
    
    geom = sceneMgr->createEntity("sphere.mesh"); 
    rootNode = sceneMgr->getRootSceneNode()->createChildSceneNode(name);
    rootNode->attachObject(geom);
    rootNode->setPosition(x, y, z);
    rootNode->scale(0.27, 0.27, 0.27);
    geom->setMaterialName("Example/Purple");
    
    rootNode->setVisible(false);
       
    typeMap[((size_t) rootNode)] = pocketType;
    
    shape = new btSphereShape(27);
    shape->calculateLocalInertia(mass, inertia);
    
    tr.setIdentity();
    tr.setRotation(btQuaternion(0.0f, 0.0f, 0.0f, 1));
    tr.setOrigin(btVector3(x, y, z));
    
    motionState = new btDefaultMotionState(tr);
    
    addToSimulator();
}