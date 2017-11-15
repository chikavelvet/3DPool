//#include <OGRE/OgreMeshManager.h>

#include "Wall.h"
#include "PhysicsComponent.h"
#include "GraphicsComponent.h"

Wall::Wall(Ogre::String _name, Ogre::SceneManager* _sceneMgr, 
        Simulator* _simulator, 
        Ogre::Vector3 normal,
        float d1, float d2, 
        float posx, float posy, float posz,
        Ogre::Vector3 up, std::map<size_t,objType>& typeMap) :
    GameObject(_name, _sceneMgr, _simulator,
            0, btVector3(0, 0, 0),
            1.0, 1.0,
            0.0, 0.0,
            false, false,
            COL_WALL, collisionType(COL_BALL | COL_CUEBALL))
{
    // Leave all Entity/SceneNode stuff as default
    rootNode->setVisible(false);

    tr.setIdentity();
    tr.setOrigin(btVector3(posx, posy, posz));
    
    if(normal == Ogre::Vector3::UNIT_Z || normal == Ogre::Vector3::NEGATIVE_UNIT_Z)
        shape = new btBoxShape(btVector3(btScalar(d1), btScalar(d2), btScalar(1.)));
    else if (normal == Ogre::Vector3::UNIT_Y || normal == Ogre::Vector3::NEGATIVE_UNIT_Y)
        shape = new btBoxShape(btVector3(btScalar(d1), btScalar(1.), btScalar(d2)));        
    else if (normal == Ogre::Vector3::UNIT_X || normal == Ogre::Vector3::NEGATIVE_UNIT_X)
        shape = new btBoxShape(btVector3(btScalar(1.), btScalar(d2), btScalar(d1)));  
    
    motionState = new btDefaultMotionState(tr);
    
    addToSimulator();
    
    body->setRollingFriction(1.0);
}

Wall::Wall(Simulator* _simulator, btVector3 _origin, Ogre::Vector3 normal,
        float dim1, float dim2)
{
    btCollisionShape* shape;

    if(normal == Ogre::Vector3::UNIT_Z || normal == Ogre::Vector3::NEGATIVE_UNIT_Z)
        shape = new btBoxShape(btVector3(btScalar(dim1), btScalar(dim2), btScalar(1.)));
    else if (normal == Ogre::Vector3::UNIT_Y || normal == Ogre::Vector3::NEGATIVE_UNIT_Y)
        shape = new btBoxShape(btVector3(btScalar(dim1), btScalar(1.), btScalar(dim2)));        
    else if (normal == Ogre::Vector3::UNIT_X || normal == Ogre::Vector3::NEGATIVE_UNIT_X)
        shape = new btBoxShape(btVector3(btScalar(1.), btScalar(dim2), btScalar(dim1)));        

    physics = new PhysicsComponent(this,
            _simulator,
            WALL_DEFAULT::PHYSICS::MASS, WALL_DEFAULT::PHYSICS::INERTIA,
            WALL_DEFAULT::PHYSICS::RESTITUTION, WALL_DEFAULT::PHYSICS::FRICTION,
            WALL_DEFAULT::PHYSICS::LINEAR_DAMPING, WALL_DEFAULT::PHYSICS::ANGULAR_DAMPING,
            WALL_DEFAULT::PHYSICS::KINEMATIC, WALL_DEFAULT::PHYSICS::NEEDS_UPDATES,
            WALL_DEFAULT::PHYSICS::COLTYPE, WALL_DEFAULT::PHYSICS::COLLIDES_WITH,
            _origin, WALL_DEFAULT::PHYSICS::ROTATION, 
            shape, NULL);
    
    physics->addToSimulator();
    physics->body->setRollingFriction(WallDefault::ROLLING_FRICTION);
}

Wall::Wall(Ogre::String _name, 
        Ogre::SceneManager* _sceneMgr,
        Ogre::Vector3 _position, 
        float dim1, float dim2,
        Ogre::Vector3 normal,
        Ogre::Vector3 up,
        std::string color)
{
    Ogre::Plane p(normal, 0);
    Ogre::MeshManager::getSingleton().createPlane(_name,
            Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
            p,
            dim1, dim2, 20, 20,
            true,
            1, 5, 5,
            up);
    
    graphics = new GraphicsComponent(this,
            _sceneMgr, _name, _position, 
            WALL_DEFAULT::GRAPHICS::SCALE,
            _name, color);
    
    graphics->geom->setCastShadows(false);
}

Wall* Wall::MakePhysicalWall(Simulator* _simulator, 
        btVector3 _origin, 
        Ogre::Vector3 normal, 
        float dim1, float dim2)
{
    return new Wall(_simulator, _origin, normal, dim1, dim2);
}

Wall* Wall::MakeVisualWall(Ogre::String _name,
        Ogre::SceneManager* _sceneMgr,
        Ogre::Vector3 _position, 
        float dim1, float dim2, 
        Ogre::Vector3 normal, 
        Ogre::Vector3 up, 
        std::string color)
{
    return new Wall(_name, _sceneMgr, _position, dim1, dim2, normal, up, color);
}