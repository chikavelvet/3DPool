#include "Ball.h"

Ball::Ball(Ogre::SceneManager* _sceneMgr, Simulator* _simulator, 
        btScalar x, btScalar y, btScalar z, 
        std::string _name, 
        std::map<size_t,objType>& typeMap, 
        std::map<Ogre::SceneNode*, Ball*>& pocketMap,
        std::string color, int _number,
        bool isSolid,
        bool isCue) :
        initialX(x), initialY(y), initialZ(z), solidBall(isSolid), number(_number)
{    

    float scaleFactor = 0.0f;
    std::string chosenMesh;

    if(isCue){
        scaleFactor = BALL_DEFAULT::GRAPHICS::SCALE_FACTOR_XYZ_CUE;
        chosenMesh = BALL_DEFAULT::GRAPHICS::MESH_CUE;
    } else {
        scaleFactor = BALL_DEFAULT::GRAPHICS::SCALE_FACTOR_XYZ;
        chosenMesh = BALL_DEFAULT::GRAPHICS::MESH;
    }

    graphics = new GraphicsComponent(this, _sceneMgr, Ogre::String(_name),
            Ogre::Vector3(x, y, z), 
            Ogre::Vector3(scaleFactor,
                          scaleFactor,
                          scaleFactor),
            chosenMesh, color);
    
    Ogre::SceneNode* rootNode = getGraphics()->rootNode;
    
    physics = new PhysicsComponent(this, _simulator, 
                BALL_DEFAULT::PHYSICS::MASS, BALL_DEFAULT::PHYSICS::INERTIA, 
                BALL_DEFAULT::PHYSICS::RESTITUTION, BALL_DEFAULT::PHYSICS::FRICTION,
                BALL_DEFAULT::PHYSICS::LINEAR_DAMPING, BALL_DEFAULT::PHYSICS::ANGULAR_DAMPING,
                BALL_DEFAULT::PHYSICS::KINEMATIC, BALL_DEFAULT::PHYSICS::NEEDS_UPDATES,
                collisionType(isCue ? COL_CUEBALL : COL_BALL), 
                isCue ? collisionType(COL_STICK   | COL_BALL | COL_WALL | COL_POCKET)
                      : collisionType(COL_CUEBALL | COL_BALL | COL_WALL | COL_POCKET),
                btVector3(x, y, z), BALL_DEFAULT::PHYSICS::ROTATION,
                new btSphereShape(BALL_DEFAULT::PHYSICS::RADIUS),
                rootNode);

    typeMap[((size_t) rootNode)] = isCue ? cueBallType : ballType;
        
    // motionState = new OgreMotionState(tr, rootNode);
        
    pocketMap[rootNode] = this;
    
    physics->addToSimulator();
    physics->body->setRollingFriction(BALL_DEFAULT::PHYSICS::ROLLING_FRICTION);
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
        std::cout << "ERROR: " << e.what() << " Ball::removeFromWorld()" << std::endl;
    }
}

void Ball::removeCueBall() {
    try {
        PhysicsComponent* phys = getPhysics();
        
        phys->body->clearForces();
        phys->body->setLinearVelocity(btVector3(0, 0, 0));

        phys->simulator->getDynamicsWorld()->removeRigidBody(phys->body);
        getGraphics()->geom->setVisible(false);
        
    } catch (ComponentNotFoundException& e) {
        std::cout << "ERROR: " << e.what() << " Ball::resetCueBall()" << std::endl;
    }
}

void Ball::addCueBall() {
    try {
        PhysicsComponent* phys = getPhysics();
        
        btTransform newTransform(btQuaternion(0, 0, 0, 1), 
            btVector3(initialX, initialY, initialZ));
        phys->body->setCenterOfMassTransform(newTransform);

        phys->simulator->getDynamicsWorld()->addRigidBody(phys->body, phys->coltype, phys->collidesWith);
        
        phys->body->getMotionState()->setWorldTransform(phys->body->getCenterOfMassTransform());

        getGraphics()->geom->setVisible(true);
    } catch (ComponentNotFoundException& e) {
        std::cout << "ERROR: " << e.what() << " Ball::resetCueBall()" << std::endl;
    }
}

void Ball::resetCueBall() {  
    this->removeCueBall();
    this->addCueBall();
}
