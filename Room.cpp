/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "Room.h"
#include "Simulator.h"

const float Room::offset = 0.0;

Room::Room(Ogre::SceneManager* _sceneMgr, Simulator* _simulator) :
            physicalWalls(),
            visualWalls()
{
    physicalWalls.push_back(Wall::MakePhysicalWall(
        _simulator, btVector3(            0,             0, 480 - offset), Ogre::Vector3::NEGATIVE_UNIT_Z, 426, 480  ));
    physicalWalls.push_back(Wall::MakePhysicalWall(
        _simulator, btVector3(            0,             0, 480 - offset), Ogre::Vector3::NEGATIVE_UNIT_Z, 480, 426  ));

    physicalWalls.push_back(Wall::MakePhysicalWall(
        _simulator, btVector3(-240 + offset,             0,       233.25), Ogre::Vector3::UNIT_X,          480, 439.5));  
    physicalWalls.push_back(Wall::MakePhysicalWall(
        _simulator, btVector3(-240 + offset,             0,      -233.25), Ogre::Vector3::UNIT_X,          480, 439.5));    
    physicalWalls.push_back(Wall::MakePhysicalWall(
        _simulator, btVector3(-240 + offset,             0,            0), Ogre::Vector3::UNIT_X,          426, 426  ));

    physicalWalls.push_back(Wall::MakePhysicalWall(
        _simulator, btVector3(            0,  240 - offset,       233.25), Ogre::Vector3::NEGATIVE_UNIT_Y, 480, 439.5));     
    physicalWalls.push_back(Wall::MakePhysicalWall(
        _simulator, btVector3(            0,  240 - offset,      -233.25), Ogre::Vector3::NEGATIVE_UNIT_Y, 480, 439.5));
    physicalWalls.push_back(Wall::MakePhysicalWall(
        _simulator, btVector3(            0,           240,   0 - offset), Ogre::Vector3::NEGATIVE_UNIT_Y, 426, 960  ));

    physicalWalls.push_back(Wall::MakePhysicalWall(
        _simulator, btVector3( 240 - offset,             0,       233.25), Ogre::Vector3::NEGATIVE_UNIT_X, 480, 439.5));     
    physicalWalls.push_back(Wall::MakePhysicalWall(
        _simulator, btVector3( 240 - offset,             0,      -233.25), Ogre::Vector3::NEGATIVE_UNIT_X, 480, 439.5));
    physicalWalls.push_back(Wall::MakePhysicalWall(
        _simulator, btVector3( 240 - offset,             0,            0), Ogre::Vector3::NEGATIVE_UNIT_X, 426, 960  ));

    physicalWalls.push_back(Wall::MakePhysicalWall(
        _simulator, btVector3(            0, -240 + offset,       233.25), Ogre::Vector3::UNIT_Y,          480, 439.5));     
    physicalWalls.push_back(Wall::MakePhysicalWall(
        _simulator, btVector3(            0, -240 + offset,      -233.25), Ogre::Vector3::UNIT_Y,          480, 439.5));
    physicalWalls.push_back(Wall::MakePhysicalWall(
        _simulator, btVector3(            0, -240 + offset,            0), Ogre::Vector3::UNIT_Y,          426, 960  ));

    physicalWalls.push_back(Wall::MakePhysicalWall(
        _simulator, btVector3(            0,             0, -480 + offset), Ogre::Vector3::UNIT_Z,          426, 480  ));
    physicalWalls.push_back(Wall::MakePhysicalWall(
        _simulator, btVector3(            0,             0, -480 - offset), Ogre::Vector3::UNIT_Z,          480, 426  ));

    //ONE
    visualWalls.push_back(Wall::MakeVisualWall(
        "_one_one", _sceneMgr,   Ogre::Vector3(      0,       0,     480), 426, 426, Ogre::Vector3::NEGATIVE_UNIT_Z, Ogre::Vector3::UNIT_Y, "Example/Green"));
    visualWalls.push_back(Wall::MakeVisualWall(
        "_one_two", _sceneMgr,   Ogre::Vector3(  226.5,       0,     480),  27, 426, Ogre::Vector3::NEGATIVE_UNIT_Z, Ogre::Vector3::UNIT_Y, "Example/Wood"));
    visualWalls.push_back(Wall::MakeVisualWall(
        "_one_three", _sceneMgr, Ogre::Vector3( -226.5,       0,     480),  27, 426, Ogre::Vector3::NEGATIVE_UNIT_Z, Ogre::Vector3::UNIT_Y, "Example/Wood"));
    visualWalls.push_back(Wall::MakeVisualWall(
        "_one_four", _sceneMgr,  Ogre::Vector3(      0,   226.5,     480), 426,  27, Ogre::Vector3::NEGATIVE_UNIT_Z, Ogre::Vector3::UNIT_Y, "Example/Wood"));
    visualWalls.push_back(Wall::MakeVisualWall(
        "_one_five", _sceneMgr,  Ogre::Vector3(      0,  -226.5,     480), 426,  27, Ogre::Vector3::NEGATIVE_UNIT_Z, Ogre::Vector3::UNIT_Y, "Example/Wood"));
    
    //TWO
    visualWalls.push_back(Wall::MakeVisualWall(
        "_two_one", _sceneMgr,   Ogre::Vector3(   -239,       0,       0),   906, 426, Ogre::Vector3::UNIT_X, Ogre::Vector3::UNIT_Y, "Example/Green"));
    visualWalls.push_back(Wall::MakeVisualWall( 
        "_two_two", _sceneMgr,   Ogre::Vector3(   -240,       0,  233.25), 439.5, 480, Ogre::Vector3::UNIT_X, Ogre::Vector3::UNIT_Y, "Example/Wood"));
    visualWalls.push_back(Wall::MakeVisualWall(
        "_two_three", _sceneMgr, Ogre::Vector3(   -240,       0, -233.25), 439.5, 480, Ogre::Vector3::UNIT_X, Ogre::Vector3::UNIT_Y, "Example/Wood"));
    visualWalls.push_back(Wall::MakeVisualWall(
        "_two_four", _sceneMgr,  Ogre::Vector3(   -240,       0,   466.5),    27, 426, Ogre::Vector3::UNIT_X, Ogre::Vector3::UNIT_Y, "Example/Wood"));
    visualWalls.push_back(Wall::MakeVisualWall(
        "_two_five", _sceneMgr,  Ogre::Vector3(   -240,       0,  -466.5),    27, 426, Ogre::Vector3::UNIT_X, Ogre::Vector3::UNIT_Y, "Example/Wood"));

    //THREE    
    visualWalls.push_back(Wall::MakeVisualWall(
        "_three_one", _sceneMgr,   Ogre::Vector3(      0,     239,       0),   426,   906, Ogre::Vector3::NEGATIVE_UNIT_Y, Ogre::Vector3::UNIT_Z, "Example/Green"));
    visualWalls.push_back(Wall::MakeVisualWall( 
        "_three_two", _sceneMgr,   Ogre::Vector3(      0,     240,  233.25),   480, 439.5, Ogre::Vector3::NEGATIVE_UNIT_Y, Ogre::Vector3::UNIT_Z, "Example/Wood"));
    visualWalls.push_back(Wall::MakeVisualWall(
        "_three_three", _sceneMgr, Ogre::Vector3(      0,     240, -233.25),   480, 439.5, Ogre::Vector3::NEGATIVE_UNIT_Y, Ogre::Vector3::UNIT_Z, "Example/Wood"));
    visualWalls.push_back(Wall::MakeVisualWall(
        "_three_four", _sceneMgr,  Ogre::Vector3(      0,     240,   466.5),   426,    27, Ogre::Vector3::NEGATIVE_UNIT_Y, Ogre::Vector3::UNIT_Z, "Example/Wood"));
    visualWalls.push_back(Wall::MakeVisualWall(
        "_three_five", _sceneMgr,  Ogre::Vector3(      0,     240,  -466.5),   426,    27, Ogre::Vector3::NEGATIVE_UNIT_Y, Ogre::Vector3::UNIT_Z, "Example/Wood"));
 
    //FOUR
    visualWalls.push_back(Wall::MakeVisualWall(
        "_four_one", _sceneMgr,   Ogre::Vector3(    239,       0,       0),   906,   426, Ogre::Vector3::NEGATIVE_UNIT_X, Ogre::Vector3::UNIT_Y, "Example/Green"));
    visualWalls.push_back(Wall::MakeVisualWall( 
        "_four_two", _sceneMgr,   Ogre::Vector3(    240,       0,  233.25), 439.5,   480, Ogre::Vector3::NEGATIVE_UNIT_X, Ogre::Vector3::UNIT_Y, "Example/Wood"));
    visualWalls.push_back(Wall::MakeVisualWall(
        "_four_three", _sceneMgr, Ogre::Vector3(    240,       0, -233.25), 439.5,   480, Ogre::Vector3::NEGATIVE_UNIT_X, Ogre::Vector3::UNIT_Y, "Example/Wood"));
    visualWalls.push_back(Wall::MakeVisualWall(
        "_four_four", _sceneMgr,  Ogre::Vector3(    240,       0,   466.5),    27,   426, Ogre::Vector3::NEGATIVE_UNIT_X, Ogre::Vector3::UNIT_Y, "Example/Wood"));
    visualWalls.push_back(Wall::MakeVisualWall(
        "_four_five", _sceneMgr,  Ogre::Vector3(    240,       0,  -466.5),    27,   426, Ogre::Vector3::NEGATIVE_UNIT_X, Ogre::Vector3::UNIT_Y, "Example/Wood"));
    
    //FIVE       
    visualWalls.push_back(Wall::MakeVisualWall(
        "_five_one", _sceneMgr,   Ogre::Vector3(      0,    -239,       0),   426,   906, Ogre::Vector3::UNIT_Y, Ogre::Vector3::UNIT_Z, "Example/Green"));
    visualWalls.push_back(Wall::MakeVisualWall( 
        "_five_two", _sceneMgr,   Ogre::Vector3(      0,    -240,  233.25),   480, 439.5, Ogre::Vector3::UNIT_Y, Ogre::Vector3::UNIT_Z, "Example/Wood"));
    visualWalls.push_back(Wall::MakeVisualWall(
        "_five_three", _sceneMgr, Ogre::Vector3(      0,    -240, -233.25),   480, 439.5, Ogre::Vector3::UNIT_Y, Ogre::Vector3::UNIT_Z, "Example/Wood"));
    visualWalls.push_back(Wall::MakeVisualWall(
        "_five_four", _sceneMgr,  Ogre::Vector3(      0,    -240,   466.5),   426,    27, Ogre::Vector3::UNIT_Y, Ogre::Vector3::UNIT_Z, "Example/Wood"));
    visualWalls.push_back(Wall::MakeVisualWall(
        "_five_five", _sceneMgr,  Ogre::Vector3(      0,    -240,  -466.5),   426,    27, Ogre::Vector3::UNIT_Y, Ogre::Vector3::UNIT_Z, "Example/Wood"));

    //SIX
    visualWalls.push_back(Wall::MakeVisualWall(
        "_six_one", _sceneMgr,   Ogre::Vector3(      0,       0,    -480),   426,   426, Ogre::Vector3::UNIT_Z, Ogre::Vector3::UNIT_Y, "Example/Green"));
    visualWalls.push_back(Wall::MakeVisualWall( 
        "_six_two", _sceneMgr,   Ogre::Vector3(  226.5,       0,    -480),    27,   426, Ogre::Vector3::UNIT_Z, Ogre::Vector3::UNIT_Y, "Example/Wood"));
    visualWalls.push_back(Wall::MakeVisualWall(
        "_six_three", _sceneMgr, Ogre::Vector3( -226.5,       0,    -480),    27,   426, Ogre::Vector3::UNIT_Z, Ogre::Vector3::UNIT_Y, "Example/Wood"));
    visualWalls.push_back(Wall::MakeVisualWall(
        "_six_four", _sceneMgr,  Ogre::Vector3(      0,   226.5,    -480),   426,    27, Ogre::Vector3::UNIT_Z, Ogre::Vector3::UNIT_Y, "Example/Wood"));
    visualWalls.push_back(Wall::MakeVisualWall(
        "_six_five", _sceneMgr,  Ogre::Vector3(      0,  -226.5,    -480),   426,    27, Ogre::Vector3::UNIT_Z, Ogre::Vector3::UNIT_Y, "Example/Wood"));
}