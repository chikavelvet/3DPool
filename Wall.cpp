#include "Wall.h"
#include "PhysicsComponent.h"

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
            COL_WALL, COL_BALL | COL_CUEBALL)
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
            WallDefault::MASS, WallDefault::INERTIA,
            WallDefault::RESTITUTION, WallDefault::FRICTION,
            WallDefault::LINEAR_DAMPING, WallDefault::ANGULAR_DAMPING,
            WallDefault::KINEMATIC, WallDefault::NEEDS_UPDATES,
            WallDefault::COLTYPE, WallDefault::COLLIDES_WITH,
            _origin, btQuaternion(0.0, 0.0, 0.0, 1), 
            shape, NULL);
    
    physics->addToSimulator();
    physics->body->setRollingFriction(1.0);
}

static Ogre::SceneNode* makeVisualPlane (Ogre::SceneManager* mSceneMgr, 
        Ogre::Vector3 normal,
        std::string name, 
        float d1, float d2, 
        Ogre::Vector3 up, 
        float posx, float posy, float posz, 
        std::string color) 
{
    
}