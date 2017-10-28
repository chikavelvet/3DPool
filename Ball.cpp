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
    geom = sceneMgr->createEntity("sphere.mesh");
    geom->setMaterialName(color);
    
    rootNode = sceneMgr->getRootSceneNode()->createChildSceneNode(name);
    rootNode->attachObject(geom);
    rootNode->setPosition(x, y, z);
    rootNode->scale(0.05, 0.05, 0.05);
    
    physics = new PhysicsComponent(this, _simulator, 
                BallDefault::MASS, BallDefault::INERTIA, 
                BallDefault::RESTITUTION, BallDefault::FRICTION,
                BallDefault::LINEAR_DAMPING, BallDefault::ANGULAR_DAMPING,
                BallDefault::KINEMATIC, BallDefault::NEEDS_UPDATES,
                isCue ? COL_CUEBALL : COL_BALL, 
                isCue ? COL_STICK   | COL_BALL | COL_WALL | COL_POCKET
                      : COL_CUEBALL | COL_BALL | COL_WALL | COL_POCKET,
                btVector3(x, y, z), BallDefault::ROTATION,
                new btSphereShape(BallDefault::RADIUS),
                rootNode
            );

    typeMap[((size_t) rootNode)] = isCue ? cueBallType : ballType;
        
    // motionState = new OgreMotionState(tr, rootNode);
        
    
    pocketMap[rootNode] = this;
    
    physics->addToSimulator();
    physics->body->setRollingFriction(btScalar(1.0));
}

Ogre::Vector3 Ball::getPosition() {
    btVector3 btPos = getBody()->getCenterOfMassPosition();
    return Ogre::Vector3(float(btPos.x()), float(btPos.y()), float(btPos.z()));
}

void Ball::removeFromWorld() {
    try {
        PhysicsComponent* phys = getPhysics();
        
        phys->body->clearForces();
        phys->body->setLinearVelocity(btVector3(0, 0, 0));
        phys->simulator->getDynamicsWorld()->removeRigidBody(phys->body);
        geom->setVisible(false);
    } catch (ComponentNotFoundException& e) {
        
    }
}

void Ball::resetCueBall() {  
    try {
        PhysicsComponent* phys = getPhysics();
        
        phys->body->clearForces();
        phys->body->setLinearVelocity(btVector3(0, 0, 0));

        phys->simulator->getDynamicsWorld()->removeRigidBody(phys->body);

        btTransform newTransform(btQuaternion(0, 0, 0, 1), 
            btVector3(initialX, initialY, initialZ));
        phys->body->setCenterOfMassTransform(newTransform);

        phys->simulator->getDynamicsWorld()->addRigidBody(phys->body, phys->coltype, phys->collidesWith);
    } catch (ComponentNotFoundException& e) {
        
    }
}

btRigidBody* Ball::getBody() {
    return getPhysics()->body;
}

Simulator* Ball::getSimulator() {
    return getPhysics()->simulator;
}