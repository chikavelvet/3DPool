#include <OGRE/OgreMeshManager.h>

#include "Wall.h"
#include "PhysicsComponent.h"
#include "GraphicsComponent.h"

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
    physics->body->setRollingFriction(WALL_DEFAULT::PHYSICS::ROLLING_FRICTION);
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