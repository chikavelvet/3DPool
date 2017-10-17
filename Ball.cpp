#include "Ball.h"

Ball::Ball(Ogre::SceneManager* _sceneMgr, Simulator* _simulator, 
        btScalar x, btScalar y, btScalar z, 
        std::string _name, 
        std::map<size_t,objType>& typeMap, 
        std::map<Ogre::SceneNode*, Ball*>& pocketMap,
        std::string color,
        bool isCue) :
    GameObject(_name, _sceneMgr, _simulator,
            5, btVector3(0, 0, 0),
            0.8, 1.0,
            0.05, 0.0,
            false, false,
            isCue ? COL_CUEBALL : COL_BALL,
            isCue ? COL_STICK   | COL_BALL | COL_WALL | COL_POCKET 
                  : COL_CUEBALL | COL_BALL | COL_WALL | COL_POCKET),
        initialX(x), initialY(y), initialZ(z)
{
    //----------------make a sphere-------------------//
    geom = sceneMgr->createEntity("sphere.mesh");
    geom->setMaterialName(color);
    
    rootNode = sceneMgr->getRootSceneNode()->createChildSceneNode(name);
    rootNode->attachObject(geom);
    rootNode->setPosition(x, y, z);
    rootNode->scale(0.05, 0.05, 0.05);

    shape = new btSphereShape(5);

    typeMap[((size_t) rootNode)] = isCue ? cueBallType : ballType;
    
    tr.setIdentity();
    tr.setRotation(btQuaternion(0.0f, 0.0f, 0.0f, 1));
    tr.setOrigin(btVector3(x, y, z));
    
    // motionState = new OgreMotionState(tr, rootNode);
    motionState = new btDefaultMotionState(tr);
    
    addToSimulator();
    
    body->setRollingFriction(btScalar(1.0));
    
    pocketMap[rootNode] = this;
}

Ogre::Vector3 Ball::getPosition() {
    btVector3 btPos = body->getCenterOfMassPosition();
    return Ogre::Vector3(float(btPos.x()), float(btPos.y()), float(btPos.z()));
}

void Ball::removeFromWorld() {
    body->clearForces();
    body->setLinearVelocity(btVector3(0, 0, 0));
    simulator->getDynamicsWorld()->removeRigidBody(body);
    geom->setVisible(false);
}

void Ball::resetCueBall() {        
    body->clearForces();
    body->setLinearVelocity(btVector3(0, 0, 0));

    simulator->getDynamicsWorld()->removeRigidBody(body);
    
    btTransform newTransform(btQuaternion(0, 0, 0, 1), 
        btVector3(initialX, initialY, initialZ));
    body->setCenterOfMassTransform(newTransform);
    
    simulator->getDynamicsWorld()->addRigidBody(body, coltype, collidesWith);
}