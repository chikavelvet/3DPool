/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "Pocket.h"

Pocket::Pocket(Ogre::SceneManager* _sceneMgr, 
        Simulator* _simulator, 
        btScalar x, btScalar y, btScalar z, 
        std::string _name, 
        std::map<size_t,objType>& typeMap) :
GameObject(_name, _sceneMgr, _simulator,
            0, btVector3(0, 0, 0),
            0.0, 0.0,
            0.0, 0.0,
            false, false,
            COL_POCKET, COL_CUEBALL | COL_BALL)
{    
    geom = sceneMgr->createEntity("sphere.mesh"); 
    geom->setMaterialName("Example/Purple");
    
    rootNode = sceneMgr->getRootSceneNode()->createChildSceneNode(name);
    rootNode->attachObject(geom);
    rootNode->setPosition(x, y, z);
    rootNode->scale(0.27, 0.27, 0.27);
    
    rootNode->setVisible(false);
    
    shape = new btSphereShape(27);
       
    typeMap[((size_t) rootNode)] = pocketType;
        
    tr.setIdentity();
    tr.setRotation(btQuaternion(0.0f, 0.0f, 0.0f, 1));
    tr.setOrigin(btVector3(x, y, z));
    
    // motionState = new OgreMotionState(tr, rootNode);
    motionState = new btDefaultMotionState(tr);
    
    addToSimulator();
}