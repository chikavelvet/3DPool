#ifndef __Room_h_
#define __Room_h_

#include "BaseApplication.h"


class Room {

private:
    void makePlane(Ogre::Vector3 normal, name, float d1, float d2, Ogre::Vector3 up, float posx, float posy, float posz)
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
        sNode11->attachObject(entity);
        sNode11->setPosition(posx, posy, posz);
        one__one->setCastShadows(false);
        one__one->setMaterialName("Examples/Rockwall");        
    }


public:



    Room() {

    //ONE_ONE
        // Ogre::Plane one_one(Ogre::Vector3::NEGATIVE_UNIT_Z, 0);
        // Ogre::MeshManager::getSingleton().createPlane("_one_one",
        //     Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        //     one_one,
        //     426, 480, 20, 20,
        //     true,
        //     1, 5, 5,
        //     Ogre::Vector3::UNIT_Y);
        // Ogre::Entity* one__one = mSceneMgr->createEntity("_one_one");
        // Ogre::SceneNode* sNode11 = mSceneMgr->getRootSceneNode()->createChildSceneNode();
        // sNode11->attachObject(one__one);
        // sNode11->setPosition(0, 0, 480);
        // one__one->setCastShadows(false);
        // one__one->setMaterialName("Examples/Rockwall");

    makePlane(Ogre::Vector3::NEGATIVE_UNIT_Z, "_one_one", 426, 480, Ogre::Vector3::UNIT_Y, 0, 0, 480);


    //ONE_TWO
        // Ogre::Plane one_two(Ogre::Vector3::NEGATIVE_UNIT_Z, 0);
        // Ogre::MeshManager::getSingleton().createPlane("_one_two",
        //     Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        //     one_two,
        //     480, 426, 20, 20,
        //     true,
        //     1, 5, 5,
        //     Ogre::Vector3::UNIT_Y);
        // Ogre::Entity* one__two = mSceneMgr->createEntity("_one_two");
        // Ogre::SceneNode* sNode12 = mSceneMgr->getRootSceneNode()->createChildSceneNode();
        // sNode12->attachObject(one__two);
        // sNode12->setPosition(0, 0, 480);
        // one__two->setCastShadows(false);
        // one__two->setMaterialName("Examples/Rockwall");

    makePlane(Ogre::Vector3::NEGATIVE_UNIT_Z, "_one_two", 480, 426, Ogre::Vector3::UNIT_Y, 0, 0, 480);

    //TWO_ONE
        // Ogre::Plane two_one(Ogre::Vector3::UNIT_X, 0);
        // Ogre::MeshManager::getSingleton().createPlane("_two_one",
        //     Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        //     two_one,
        //     480, 439.5, 20, 20,
        //     true,
        //     1, 5, 5,
        //     Ogre::Vector3::UNIT_Z);
        // Ogre::Entity* two__one = mSceneMgr->createEntity("_two_one");
        // Ogre::SceneNode* sNode21 = mSceneMgr->getRootSceneNode()->createChildSceneNode();
        // sNode21->attachObject(two__one);
        // sNode21->setPosition(-240, 0, 233.25);
        // two__one->setCastShadows(false);
        // two__one->setMaterialName("Examples/Rockwall");

    makePlane(Ogre::Vector3::UNIT_X, "_two_one", 480, 439.5, Ogre::Vector3::UNIT_Z, -240, 0, 233.25);


    //TWO_TWO
        // Ogre::Plane two_two(Ogre::Vector3::UNIT_X, Ogre::Vector3(0,0,0));
        // Ogre::MeshManager::getSingleton().createPlane("_two_two",
        //     Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        //     two_two,
        //     480, 439.5, 20, 20,
        //     true,
        //     1, 5, 5,
        //     Ogre::Vector3::UNIT_Z);
        // Ogre::Entity* two__two = mSceneMgr->createEntity("_two_two");
        // Ogre::SceneNode* sNode22 = mSceneMgr->getRootSceneNode()->createChildSceneNode();
        // sNode22->attachObject(two__two);
        // sNode22->setPosition(-240, 0, -233.25);
        // two__two->setCastShadows(false);
        // two__two->setMaterialName("Examples/Rockwall");

        makePlane(Ogre::Vector3::UNIT_X, "_two_two", 480, 439.5, Ogre::Vector3::UNIT_Z, -240, 0, -233.25);

    //TWO_THREE
        // Ogre::Plane two_three(Ogre::Vector3::UNIT_X, 0);
        // Ogre::MeshManager::getSingleton().createPlane("_two_three",
        //     Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        //     two_three,
        //     426, 960, 20, 20,
        //     true,
        //     1, 5, 5,
        //     Ogre::Vector3::UNIT_Z);
        // Ogre::Entity* two__three = mSceneMgr->createEntity("_two_three");
        // Ogre::SceneNode* sNode23 = mSceneMgr->getRootSceneNode()->createChildSceneNode();
        // sNode23->attachObject(two__three);
        // sNode23->setPosition(-240, 0, 0);
        // two__three->setCastShadows(false);
        // two__three->setMaterialName("Examples/Rockwall");

        makePlane(Ogre::Vector3::UNIT_X, "_two_three", 426, 960, Ogre::Vector3::UNIT_Z, -240, 0, 0);

    //THREE_ONE
        // Ogre::Plane three_one(Ogre::Vector3::NEGATIVE_UNIT_Y, 0);
        // Ogre::MeshManager::getSingleton().createPlane("_three_one",
        //     Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        //     three_one,
        //     480, 439.5, 20, 20,
        //     true,
        //     1, 5, 5,
        //     Ogre::Vector3::UNIT_Z);
        // Ogre::Entity* three__one = mSceneMgr->createEntity("_three_one");
        // Ogre::SceneNode* sNode31 = mSceneMgr->getRootSceneNode()->createChildSceneNode();
        // sNode31->attachObject(three__one);
        // sNode31->setPosition(0, 240, 233.25);
        // three__one->setCastShadows(false);
        // three__one->setMaterialName("Examples/Rockwall");

        makePlane(Ogre::Vector3::NEGATIVE_UNIT_Y, "_three_one", 480, 439.5, Ogre::Vector3::UNIT_Z, 0, 240, 233.25);

    //THREE_TWO
        // Ogre::Plane three_two(Ogre::Vector3::NEGATIVE_UNIT_Y, 0);
        // Ogre::MeshManager::getSingleton().createPlane("_three_two",
        //     Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        //     three_two,
        //     480, 439.5, 20, 20,
        //     true,
        //     1, 5, 5,
        //     Ogre::Vector3::UNIT_Z);
        // Ogre::Entity* three__two = mSceneMgr->createEntity("_three_two");
        // Ogre::SceneNode* sNode32 = mSceneMgr->getRootSceneNode()->createChildSceneNode();
        // sNode32->attachObject(three__two);
        // sNode32->setPosition(0, 240, -233.25);
        // three__two->setCastShadows(false);
        // three__two->setMaterialName("Examples/Rockwall");

        makePlane(Ogre::Vector3::NEGATIVE_UNIT_Y, "_three_two", 480, 439.5, Ogre::Vector3::UNIT_Z, 0, 240, -233.25);

    //THREE_THREE
        // Ogre::Plane three_three(Ogre::Vector3::NEGATIVE_UNIT_Y, 0);
        // Ogre::MeshManager::getSingleton().createPlane("_three_three",
        //     Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        //     three_three,
        //     426, 960, 20, 20,
        //     true,
        //     1, 5, 5,
        //     Ogre::Vector3::UNIT_Z);
        // Ogre::Entity* three__three = mSceneMgr->createEntity("_three_three");
        // Ogre::SceneNode* sNode33 = mSceneMgr->getRootSceneNode()->createChildSceneNode();
        // sNode33->attachObject(three__three);
        // sNode33->setPosition(0, 240, 0);
        // three__three->setCastShadows(false);
        // three__three->setMaterialName("Examples/Rockwall");

        makePlane(Ogre::Vector3::NEGATIVE_UNIT_Y, "_three_three", 426, 960, Ogre::Vector3::UNIT_Z, 0, 240, 0);

    //FOUR_ONE
        // Ogre::Plane four_one(Ogre::Vector3::NEGATIVE_UNIT_X, 0);
        // Ogre::MeshManager::getSingleton().createPlane("_four_one",
        //     Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        //     four_one,
        //     480, 439.5, 20, 20,
        //     true,
        //     1, 5, 5,
        //     Ogre::Vector3::UNIT_Z);
        // Ogre::Entity* four__one = mSceneMgr->createEntity("_four_one");
        // Ogre::SceneNode* sNode41 = mSceneMgr->getRootSceneNode()->createChildSceneNode();
        // sNode41->attachObject(four__one);
        // sNode41->setPosition(240, 0, 233.25);
        // four__one->setCastShadows(false);
        // four__one->setMaterialName("Examples/Rockwall");

        makePlane(Ogre::Vector3::NEGATIVE_UNIT_X, "_four_one", 480, 439.5, Ogre::Vector3::UNIT_Z, 240, 0, 233.25);

    //FOUR_TWO
        // Ogre::Plane four_two(Ogre::Vector3::NEGATIVE_UNIT_X, 0);
        // Ogre::MeshManager::getSingleton().createPlane("_four_two",
        //     Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        //     four_two,
        //     480, 439.5, 20, 20,
        //     true,
        //     1, 5, 5,
        //     Ogre::Vector3::UNIT_Z);
        // Ogre::Entity* four__two = mSceneMgr->createEntity("_four_two");
        // Ogre::SceneNode* sNode42 = mSceneMgr->getRootSceneNode()->createChildSceneNode();
        // sNode42->attachObject(four__two);
        // sNode42->setPosition(240, 0, -233.25);
        // four__two->setCastShadows(false);
        // four__two->setMaterialName("Examples/Rockwall");

        makePlane(Ogre::Vector3::NEGATIVE_UNIT_X, "_four_two", 480, 439.5, Ogre::Vector3::UNIT_Z, 240, 0, -233.25);

    //FOUR_THREE
        // Ogre::Plane four_three(Ogre::Vector3::NEGATIVE_UNIT_X, 0);
        // Ogre::MeshManager::getSingleton().createPlane("_four_three",
        //     Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        //     four_three,
        //     426, 960, 20, 20,
        //     true,
        //     1, 5, 5,
        //     Ogre::Vector3::UNIT_Z);
        // Ogre::Entity* four__three = mSceneMgr->createEntity("_four_three");
        // Ogre::SceneNode* sNode43 = mSceneMgr->getRootSceneNode()->createChildSceneNode();
        // sNode43->attachObject(four__three);
        // sNode43->setPosition(240, 0, 0);
        // four__three->setCastShadows(false);
        // four__three->setMaterialName("Examples/Rockwall");

        makePlane(Ogre::Vector3::NEGATIVE_UNIT_X, "_four_three", 426, 960, Ogre::Vector3::UNIT_Z, 240, 0, 0);

    //FIVE_ONE
        // Ogre::Plane five_one(Ogre::Vector3::UNIT_Y, 0);
        // Ogre::MeshManager::getSingleton().createPlane("_five_one",
        //     Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        //     five_one,
        //     480, 439.5, 20, 20,
        //     true,
        //     1, 5, 5,
        //     Ogre::Vector3::UNIT_Z);
        // Ogre::Entity* five__one = mSceneMgr->createEntity("_five_one");
        // Ogre::SceneNode* sNode51 = mSceneMgr->getRootSceneNode()->createChildSceneNode();
        // sNode51->attachObject(five__one);
        // sNode51->setPosition(0, -240, 233.25);
        // five__one->setCastShadows(false);
        // five__one->setMaterialName("Examples/Rockwall");

        makePlane(Ogre::Vector3::UNIT_Y, "_five_one", 480, 439.5, Ogre::Vector3::UNIT_Z, 0, -240, 233.25);

    //FIVE_TWO
        // Ogre::Plane five_two(Ogre::Vector3::UNIT_Y, 0);
        // Ogre::MeshManager::getSingleton().createPlane("_five_two",
        //     Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        //     five_two,
        //     480, 439.5, 20, 20,
        //     true, 1, 5, 5,
        //     Ogre::Vector3::UNIT_Z);
        // Ogre::Entity* five__two = mSceneMgr->createEntity("_five_two");
        // Ogre::SceneNode* sNode52 = mSceneMgr->getRootSceneNode()->createChildSceneNode();
        // sNode52->attachObject(five__two);
        // sNode52->setPosition(0, -240, -233.25);
        // five__two->setCastShadows(false);
        // five__two->setMaterialName("Examples/Rockwall");

        makePlane(Ogre::Vector3::UNIT_Y, "_five_two", 480, 439.5, Ogre::Vector3::UNIT_Z, 0, -240, -233.25);

    //FIVE_THREE
        // Ogre::Plane five_three(Ogre::Vector3::UNIT_Y, 0);
        // Ogre::MeshManager::getSingleton().createPlane("_five_three",
        //     Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        //     five_three,
        //     426, 960, 20, 20,
        //     true,
        //     1, 5, 5,
        //     Ogre::Vector3::UNIT_Z);
        // Ogre::Entity* five__three = mSceneMgr->createEntity("_five_three");
        // Ogre::SceneNode* sNode53 = mSceneMgr->getRootSceneNode()->createChildSceneNode();
        // sNode53->attachObject(five__three);
        // sNode53->setPosition(0, -240, 0);
        // five__three->setCastShadows(false);
        // five__three->setMaterialName("Examples/Rockwall");

        makePlane(Ogre::Vector3::UNIT_Y, "_five_three", 426, 960, Ogre::Vector3::UNIT_Z, 0, -240, 0);

    //SIX_ONE
        // Ogre::Plane six_one(Ogre::Vector3::UNIT_Z, 0);
        // Ogre::MeshManager::getSingleton().createPlane("_six_one",
        //     Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        //     six_one,
        //     426, 480, 20, 20,
        //     true,
        //     1, 5, 5,
        //     Ogre::Vector3::UNIT_Y);
        // Ogre::Entity* six__one = mSceneMgr->createEntity("_six_one");
        // Ogre::SceneNode* sNode61 = mSceneMgr->getRootSceneNode()->createChildSceneNode();
        // sNode61->attachObject(six__one);
        // sNode61->setPosition(0, 0, -480);
        // six__one->setCastShadows(false);
        // six__one->setMaterialName("Examples/Rockwall");

        makePlane(Ogre::Vector3::UNIT_Z, "_six_one", 426, 480, Ogre::Vector3::UNIT_Y, 0, 0, -480);

    //SIX_TWO
        // Ogre::Plane six_two(Ogre::Vector3::UNIT_Z, 0);
        // Ogre::MeshManager::getSingleton().createPlane("_six_two",
        //     Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        //     six_two,
        //     480, 426, 20, 20,
        //     true,
        //     1, 5, 5,
        //     Ogre::Vector3::UNIT_Y);
        // Ogre::Entity* six__two = mSceneMgr->createEntity("_six_two");
        // Ogre::SceneNode* sNode62 = mSceneMgr->getRootSceneNode()->createChildSceneNode();
        // sNode62->attachObject(six__two);
        // sNode62->setPosition(0, 0, -480);
        // six__two->setCastShadows(false);
        // six__two->setMaterialName("Examples/Rockwall");

        makePlane(Ogre::Vector3::UNIT_Z, "_six_two", 480, 426, Ogre::Vector3::UNIT_Y, 0, 0, -480);
    }
};

#endif