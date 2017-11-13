/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "Room.h"

Room::Room(Ogre::SceneManager* _sceneMgr, Simulator* _simulator) :
            physicalWalls(),
            visualWalls()
{
    float offset = 0.0f;

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

//        makePlaneWithPhysics(Ogre::Vector3::NEGATIVE_UNIT_Z, "_one_one", 426, 480, Ogre::Vector3::UNIT_Y, 0, 0, 480, _sceneMgr, 0, 0, 480 - offset, _simulator);
//        makePlaneWithPhysics(Ogre::Vector3::NEGATIVE_UNIT_Z, "_one_two", 480, 426, Ogre::Vector3::UNIT_Y, 0, 0, 479, _sceneMgr, 0, 0, 480 - offset, _simulator);

//        makePlaneWithPhysics(Ogre::Vector3::UNIT_X, "_two_one", 480, 439.5, Ogre::Vector3::UNIT_Z, -240, 0, 233.25, _sceneMgr, -240 + offset, 0, 233.25, _simulator);
//        makePlaneWithPhysics(Ogre::Vector3::UNIT_X, "_two_two", 480, 439.5, Ogre::Vector3::UNIT_Z, -240, 0, -233.25, _sceneMgr, -240 + offset, 0, -233.25, _simulator);
//        makePlaneWithPhysics(Ogre::Vector3::UNIT_X, "_two_three", 426, 960, Ogre::Vector3::UNIT_Z, -239, 0, 0, _sceneMgr,       -240 + offset, 0, 0, _simulator);

//        makePlaneWithPhysics(Ogre::Vector3::NEGATIVE_UNIT_Y, "_three_one", 480, 439.5, Ogre::Vector3::UNIT_Z, 0, 240, 233.25, _sceneMgr,  0, 240 - offset, 233.25, _simulator);
//        makePlaneWithPhysics(Ogre::Vector3::NEGATIVE_UNIT_Y, "_three_two", 480, 439.5, Ogre::Vector3::UNIT_Z, 0, 240, -233.25, _sceneMgr, 0, 240 - offset, -233.25, _simulator);
//        makePlaneWithPhysics(Ogre::Vector3::NEGATIVE_UNIT_Y, "_three_three", 426, 960, Ogre::Vector3::UNIT_Z, 0, 239, 0, _sceneMgr,       0, 240,          0 - offset, _simulator);

//        makePlaneWithPhysics(Ogre::Vector3::NEGATIVE_UNIT_X, "_four_one", 480, 439.5, Ogre::Vector3::UNIT_Z, 240, 0, 233.25, _sceneMgr,  240 - offset, 0, 233.25, _simulator);
//        makePlaneWithPhysics(Ogre::Vector3::NEGATIVE_UNIT_X, "_four_two", 480, 439.5, Ogre::Vector3::UNIT_Z, 240, 0, -233.25, _sceneMgr, 240 - offset, 0, -233.25, _simulator);
//        makePlaneWithPhysics(Ogre::Vector3::NEGATIVE_UNIT_X, "_four_three", 426, 960, Ogre::Vector3::UNIT_Z, 239, 0, 0, _sceneMgr,       240 - offset, 0, 0, _simulator);

//        makePlaneWithPhysics(Ogre::Vector3::UNIT_Y, "_five_one", 480, 439.5, Ogre::Vector3::UNIT_Z, 0, -240, 233.25, _sceneMgr,  0, -240 + offset, 233.25, _simulator);
//        makePlaneWithPhysics(Ogre::Vector3::UNIT_Y, "_five_two", 480, 439.5, Ogre::Vector3::UNIT_Z, 0, -240, -233.25, _sceneMgr, 0, -240 + offset, -233.25, _simulator);
//        makePlaneWithPhysics(Ogre::Vector3::UNIT_Y, "_five_three", 426, 960, Ogre::Vector3::UNIT_Z, 0, -239, 0, _sceneMgr,       0, -240 + offset, 0, _simulator);

//        makePlaneWithPhysics(Ogre::Vector3::UNIT_Z, "_six_one", 426, 480, Ogre::Vector3::UNIT_Y, 0, 0, -479, _sceneMgr,          0, 0, -480 + offset, _simulator);
//        makePlaneWithPhysics(Ogre::Vector3::UNIT_Z, "_six_two", 480, 426, Ogre::Vector3::UNIT_Y, 0, 0, -480 - offset, _sceneMgr, 0, 0, -480 - offset, _simulator);

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
    
//    makePlane(Ogre::Vector3::NEGATIVE_UNIT_Z, "_one_one", 426, 426, Ogre::Vector3::UNIT_Y,       0,     0, 480, _sceneMgr, "Example/Green");
//    makePlane(Ogre::Vector3::NEGATIVE_UNIT_Z, "_one_two", 27, 426, Ogre::Vector3::UNIT_Y,    226.5,     0, 480, _sceneMgr, "Example/Wood");
//    makePlane(Ogre::Vector3::NEGATIVE_UNIT_Z, "_one_three", 27, 426, Ogre::Vector3::UNIT_Y, -226.5,     0, 480, _sceneMgr, "Example/Wood");
//    makePlane(Ogre::Vector3::NEGATIVE_UNIT_Z, "_one_four", 426, 27, Ogre::Vector3::UNIT_Y,       0, 226.5, 480, _sceneMgr, "Example/Wood");
//    makePlane(Ogre::Vector3::NEGATIVE_UNIT_Z, "_one_five", 426, 27, Ogre::Vector3::UNIT_Y,       0, -226.5, 480, _sceneMgr, "Example/Wood");

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
    
//    makePlane(Ogre::Vector3::UNIT_X, "_two_one", 906, 426, Ogre::Vector3::UNIT_Y,     -239, 0,       0, _sceneMgr, "Example/Green");
//    makePlane(Ogre::Vector3::UNIT_X, "_two_two", 439.5, 480, Ogre::Vector3::UNIT_Y,   -240, 0,  233.25, _sceneMgr, "Example/Wood");
//    makePlane(Ogre::Vector3::UNIT_X, "_two_three", 439.5, 480, Ogre::Vector3::UNIT_Y, -240, 0, -233.25, _sceneMgr, "Example/Wood");
//    makePlane(Ogre::Vector3::UNIT_X, "_two_four", 27, 426, Ogre::Vector3::UNIT_Y,     -240, 0,   466.5, _sceneMgr, "Example/Wood");
//    makePlane(Ogre::Vector3::UNIT_X, "_two_five", 27, 426, Ogre::Vector3::UNIT_Y,     -240, 0,  -466.5, _sceneMgr, "Example/Wood");        

    //THREE
    makePlane(Ogre::Vector3::NEGATIVE_UNIT_Y, "_three_one", 426, 906, Ogre::Vector3::UNIT_Z, 0, 239, 0, _sceneMgr, "Example/Green");
    makePlane(Ogre::Vector3::NEGATIVE_UNIT_Y, "_three_two", 480, 439.5, Ogre::Vector3::UNIT_Z, 0, 240, 233.25, _sceneMgr, "Example/Wood");
    makePlane(Ogre::Vector3::NEGATIVE_UNIT_Y, "_three_three", 480, 439.5, Ogre::Vector3::UNIT_Z, 0, 240, -233.25, _sceneMgr, "Example/Wood");
    makePlane(Ogre::Vector3::NEGATIVE_UNIT_Y, "_three_four", 426, 27, Ogre::Vector3::UNIT_Z, 0, 240, 466.5, _sceneMgr, "Example/Wood");
    makePlane(Ogre::Vector3::NEGATIVE_UNIT_Y, "_three_five", 426, 27, Ogre::Vector3::UNIT_Z, 0, 240, -466.5, _sceneMgr, "Example/Wood");        

    //FOUR
    makePlane(Ogre::Vector3::NEGATIVE_UNIT_X, "_four_one", 906, 426, Ogre::Vector3::UNIT_Y, 239, 0, 0, _sceneMgr, "Example/Green");
    makePlane(Ogre::Vector3::NEGATIVE_UNIT_X, "_four_two", 439.5, 480, Ogre::Vector3::UNIT_Y, 240, 0, 233.25, _sceneMgr, "Example/Wood");
    makePlane(Ogre::Vector3::NEGATIVE_UNIT_X, "_four_three", 439.5, 480, Ogre::Vector3::UNIT_Y, 240, 0, -233.25, _sceneMgr, "Example/Wood");
    makePlane(Ogre::Vector3::NEGATIVE_UNIT_X, "_four_four", 27, 426, Ogre::Vector3::UNIT_Y, 240, 0, 466.5, _sceneMgr, "Example/Wood");
    makePlane(Ogre::Vector3::NEGATIVE_UNIT_X, "_four_five", 27, 426, Ogre::Vector3::UNIT_Y, 240, 0, -466.5, _sceneMgr, "Example/Wood");

    //FIVE       
    makePlane(Ogre::Vector3::UNIT_Y, "_five_one", 426, 906, Ogre::Vector3::UNIT_Z, 0, -239, 0, _sceneMgr, "Example/Green");
    makePlane(Ogre::Vector3::UNIT_Y, "_five_two", 480, 439.5, Ogre::Vector3::UNIT_Z, 0, -240, 233.25, _sceneMgr, "Example/Wood");
    makePlane(Ogre::Vector3::UNIT_Y, "_five_three", 480, 439.5, Ogre::Vector3::UNIT_Z, 0, -240, -233.25, _sceneMgr, "Example/Wood");
    makePlane(Ogre::Vector3::UNIT_Y, "_five_four", 426, 27, Ogre::Vector3::UNIT_Z, 0, -240, 466.5, _sceneMgr, "Example/Wood");
    makePlane(Ogre::Vector3::UNIT_Y, "_five_five", 426, 27, Ogre::Vector3::UNIT_Z, 0, -240, -466.5, _sceneMgr, "Example/Wood");

    //SIX
    makePlane(Ogre::Vector3::UNIT_Z, "_six_one", 426, 426, Ogre::Vector3::UNIT_Y, 0, 0, -480, _sceneMgr, "Example/Green");
    makePlane(Ogre::Vector3::UNIT_Z, "_six_two", 27, 426, Ogre::Vector3::UNIT_Y, 226.5, 0, -480, _sceneMgr, "Example/Wood");
    makePlane(Ogre::Vector3::UNIT_Z, "_six_three", 27, 426, Ogre::Vector3::UNIT_Y, -226.5, 0, -480, _sceneMgr, "Example/Wood");
    makePlane(Ogre::Vector3::UNIT_Z, "_six_four", 426, 27, Ogre::Vector3::UNIT_Y, 0, 226.5, -480, _sceneMgr, "Example/Wood");
    makePlane(Ogre::Vector3::UNIT_Z, "_six_five", 426, 27, Ogre::Vector3::UNIT_Y, 0, -226.5, -480, _sceneMgr, "Example/Wood"); 
}