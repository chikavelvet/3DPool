#ifndef __Room_h_
#define __Room_h_

#include "BaseApplication.h"


class Room {

private:
    void makePlane(Ogre::Vector3 normal, std::string name, float d1, float d2, Ogre::Vector3 up, float posx, float posy, float posz, Ogre::SceneManager* mSceneMgr)
    {
        Ogre::Plane p(normal, 0);
        Ogre::MeshManager::getSingleton().createPlane(name,
            Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
            p,
            d1, d2, 20, 20,
            true,
            1, 5, 5,
            up);
        Ogre::Entity* entity = mSceneMgr->createEntity(name);
        Ogre::SceneNode* node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
        node->attachObject(entity);
        node->setPosition(posx, posy, posz);
        entity->setCastShadows(false);
        entity->setMaterialName("Examples/Rockwall");        
    }

public:
    ~Room();
    Room();

    Room(Ogre::SceneManager* mSceneMgr) {
        makePlane(Ogre::Vector3::NEGATIVE_UNIT_Z, "_one_one", 426, 480, Ogre::Vector3::UNIT_Y, 0, 0, 480, mSceneMgr);
        makePlane(Ogre::Vector3::NEGATIVE_UNIT_Z, "_one_two", 480, 426, Ogre::Vector3::UNIT_Y, 0, 0, 480, mSceneMgr);
        makePlane(Ogre::Vector3::UNIT_X, "_two_one", 480, 439.5, Ogre::Vector3::UNIT_Z, -240, 0, 233.25, mSceneMgr);
        makePlane(Ogre::Vector3::UNIT_X, "_two_two", 480, 439.5, Ogre::Vector3::UNIT_Z, -240, 0, -233.25, mSceneMgr);
        makePlane(Ogre::Vector3::UNIT_X, "_two_three", 426, 960, Ogre::Vector3::UNIT_Z, -240, 0, 0, mSceneMgr);
        makePlane(Ogre::Vector3::NEGATIVE_UNIT_Y, "_three_one", 480, 439.5, Ogre::Vector3::UNIT_Z, 0, 240, 233.25, mSceneMgr);
        makePlane(Ogre::Vector3::NEGATIVE_UNIT_Y, "_three_two", 480, 439.5, Ogre::Vector3::UNIT_Z, 0, 240, -233.25, mSceneMgr);
        makePlane(Ogre::Vector3::NEGATIVE_UNIT_Y, "_three_three", 426, 960, Ogre::Vector3::UNIT_Z, 0, 240, 0, mSceneMgr);
        makePlane(Ogre::Vector3::NEGATIVE_UNIT_X, "_four_one", 480, 439.5, Ogre::Vector3::UNIT_Z, 240, 0, 233.25, mSceneMgr);
        makePlane(Ogre::Vector3::NEGATIVE_UNIT_X, "_four_two", 480, 439.5, Ogre::Vector3::UNIT_Z, 240, 0, -233.25, mSceneMgr);
        makePlane(Ogre::Vector3::NEGATIVE_UNIT_X, "_four_three", 426, 960, Ogre::Vector3::UNIT_Z, 240, 0, 0, mSceneMgr);
        makePlane(Ogre::Vector3::UNIT_Y, "_five_one", 480, 439.5, Ogre::Vector3::UNIT_Z, 0, -240, 233.25, mSceneMgr);
        makePlane(Ogre::Vector3::UNIT_Y, "_five_two", 480, 439.5, Ogre::Vector3::UNIT_Z, 0, -240, -233.25, mSceneMgr);
        makePlane(Ogre::Vector3::UNIT_Y, "_five_three", 426, 960, Ogre::Vector3::UNIT_Z, 0, -240, 0, mSceneMgr);
        makePlane(Ogre::Vector3::UNIT_Z, "_six_one", 426, 480, Ogre::Vector3::UNIT_Y, 0, 0, -480, mSceneMgr);
        makePlane(Ogre::Vector3::UNIT_Z, "_six_two", 480, 426, Ogre::Vector3::UNIT_Y, 0, 0, -480, mSceneMgr);
    }
};

#endif