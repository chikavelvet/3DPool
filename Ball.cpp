#include "Ball.h"

Ball::Ball(Ogre::SceneManager* mSceneMgr, Simulator* physicsEngine, 
        btScalar x, btScalar y, btScalar z, 
        std::string name, 
        std::map<size_t,objType>& typeMap, bool isCue) :
    colShape(new btSphereShape(5)),
    mass(5),
    localInertia(btVector3(0, 0, 0))
{
    //----------------make a sphere-------------------//
    entity = mSceneMgr->createEntity("sphere.mesh"); 
    rootNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(name);
    rootNode->attachObject(entity);
    rootNode->setPosition(x, y, z);
    rootNode->scale(0.05, 0.05, 0.05);
            
    int collidesWith = isCue ? (COL_STICK | COL_BALL | COL_WALL) 
                             : (COL_CUEBALL | COL_BALL | COL_WALL);

    typeMap[((size_t) rootNode)] = isCue ? cueBallType : ballType;
    
    coltype = isCue ? COL_CUEBALL : COL_BALL;

    physicsEngine->getCollisionShapes().push_back(colShape);
    btTransform startTransform;
    startTransform.setIdentity();
    startTransform.setRotation(btQuaternion(0.0f, 0.0f, 0.0f, 1));

    btVector3 initialPosition(x, y, z);
    startTransform.setOrigin(initialPosition);
    colShape->calculateLocalInertia(mass, localInertia);

    //actually contruct the body and add it to the dynamics world
    btDefaultMotionState *myMotionState = new btDefaultMotionState(startTransform); 
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
    body = new btRigidBody(rbInfo);
    body->setUserPointer(rootNode);
    body->setFriction(btScalar(1.0));
    body->setRollingFriction(btScalar(1.0));

    body->setRestitution(0.8);
    body->setDamping(0.1, 0);

//    body->setUserIndex(ballType);
    physicsEngine->getDynamicsWorld()->addRigidBody(body, coltype, collidesWith);
}

Ogre::Vector3 Ball::getPosition() {
    btVector3 btPos = body->getCenterOfMassPosition();
    return Ogre::Vector3(float(btPos.x()), float(btPos.y()), float(btPos.z()));
}