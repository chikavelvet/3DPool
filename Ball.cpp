#include "Ball.h"

Ball::Ball(Ogre::SceneManager* _sceneMgr, Simulator* _simulator, 
        btScalar x, btScalar y, btScalar z, 
        std::string _name, 
        std::map<size_t,objType>& typeMap, 
        std::map<Ogre::SceneNode*, Ball*>& pocketMap,
        std::string color,
        bool isCue) :
        initialX(x), initialY(y), initialZ(z)
{    
    graphics = new GraphicsComponent(this, _sceneMgr, Ogre::String(_name),
            Ogre::Vector3(x, y, z), 
            Ogre::Vector3(BALL_DEFAULT::GRAPHICS::SCALE_FACTOR,
                          BALL_DEFAULT::GRAPHICS::SCALE_FACTOR,
                          BALL_DEFAULT::GRAPHICS::SCALE_FACTOR),
            "sphere.mesh", color);
    
    Ogre::SceneNode* rootNode = getGraphics()->rootNode;
    
    physics = new PhysicsComponent(this, _simulator, 
                BALL_DEFAULT::PHYSICS::MASS, BALL_DEFAULT::PHYSICS::INERTIA, 
                BALL_DEFAULT::PHYSICS::RESTITUTION, BALL_DEFAULT::PHYSICS::FRICTION,
                BALL_DEFAULT::PHYSICS::LINEAR_DAMPING, BALL_DEFAULT::PHYSICS::ANGULAR_DAMPING,
                BALL_DEFAULT::PHYSICS::KINEMATIC, BALL_DEFAULT::PHYSICS::NEEDS_UPDATES,
                isCue ? COL_CUEBALL : COL_BALL, 
                isCue ? COL_STICK   | COL_BALL | COL_WALL | COL_POCKET
                      : COL_CUEBALL | COL_BALL | COL_WALL | COL_POCKET,
                btVector3(x, y, z), BALL_DEFAULT::PHYSICS::ROTATION,
                new btSphereShape(BALL_DEFAULT::PHYSICS::RADIUS),
                rootNode);

    typeMap[((size_t) rootNode)] = isCue ? cueBallType : ballType;
        
    // motionState = new OgreMotionState(tr, rootNode);
        
    pocketMap[rootNode] = this;
    
    physics->addToSimulator();
    physics->body->setRollingFriction(BALL_DEFAULT::ROLLING_FRICTION);
}

Ogre::Vector3 Ball::getPosition() {
    btVector3 btPos = getBody()->getCenterOfMassPosition();
    return Ogre::Vector3(float(btPos.x()), float(btPos.y()), float(btPos.z()));
}

void Ball::removeFromWorld() {
    try {
        GraphicsComponent* graph = getGraphics();
        PhysicsComponent* phys = getPhysics();
        
        phys->body->clearForces();
        phys->body->setLinearVelocity(btVector3(0, 0, 0));
        phys->simulator->getDynamicsWorld()->removeRigidBody(phys->body);
        graph->geom->setVisible(false);
    } catch (ComponentNotFoundException& e) {
        std::cout << "ERROR: " << e.what() << " Ball::removeFromWorld()" << std::endl();
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
        std::cout << "ERROR: " << e.what() << " Ball::resetCueBall()" << std::endl;
    }
}

btRigidBody* Ball::getBody() {
    return getPhysics()->body;
}

Simulator* Ball::getSimulator() {
    return getPhysics()->simulator;
}

Ogre::SceneNode* Ball::getNode() {
    return getGraphics()->rootNode;
}