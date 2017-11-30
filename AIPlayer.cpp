/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   AIPlayer.cpp
 * Author: treyr3
 * 
 * Created on November 15, 2017, 5:19 PM
 */

#include <vector>

#include <OgreVector3.h>
#include <OgreSceneNode.h>
#include <OGRE/OgreQuaternion.h>

#include "AIPlayer.h"
#include "ThreeDPool.h"
#include "GraphicsComponent.h"

#include "Ball.h"
#include "Pocket.h"
#include "Stick.h"

AIPlayer::AIPlayer(ThreeDPool* _game) :
        game(_game),
        decided(false)
{
}

AIPlayer::AIPlayer(const AIPlayer& orig) {
}

AIPlayer::~AIPlayer() {
}

void AIPlayer::decideShot()
{
    Ogre::SceneNode* cueBallNode = game->cueBall->getNode();
    
    std::vector<Pocket*> pockets = game->pockets;//pockets
    std::vector<Ball*> ourBalls, oppBalls;
    
    if(game->ballsAssignedToPlayers){
        ourBalls = targetRedBall ? game->redBalls : game->blueBalls;
        oppBalls = targetRedBall ? game->blueBalls : game->redBalls;
    }
    else {
        ourBalls = std::vector<Ball*>(game->redBalls);
        ourBalls.insert(ourBalls.end(), game->blueBalls.begin(), game->blueBalls.end());
        oppBalls = std::vector<Ball*>();    
    }    

    Pocket* bestPocket;
    Ball* bestBall;
    double bestChance = 0.0;
    
    for(std::vector<Ball*>::iterator ballIt = ourBalls.begin(); ballIt != ourBalls.end(); ++ballIt) {
        Ball* curBall = *ballIt;
        std::cout << curBall << std::endl;
        if(!curBall->getGraphics()->geom->isVisible())
            continue;

        Ogre::SceneNode* ballNode = curBall->getNode();
        
        for(std::vector<Pocket*>::iterator pocketIt = pockets.begin(); pocketIt != pockets.end(); ++pocketIt) {
            Pocket* curPocket = *pocketIt;
            Ogre::SceneNode* pocketNode = curPocket->getNode();
            
            Ogre::Vector3 A = ballNode->getPosition() - cueBallNode->getPosition();
            Ogre::Vector3 B = pocketNode->getPosition() - ballNode->getPosition();
            Ogre::Vector3 C = pocketNode->getPosition() - cueBallNode->getPosition();
            
            Ogre::Degree AtoB = A.angleBetween(B);//alignment
            
            if(AtoB > Ogre::Degree(Ogre::Real(100))) {
                Ogre::Real proximity = B.length();
                Ogre::Real chance = AtoB.valueDegrees() / proximity;
                
                if(chance > bestChance){
                    bestChance = chance;
                    bestPocket = curPocket;
                    bestBall = curBall;
                    
                    std::cout << "Chose a Best" << std::endl;
                }
            }
        }
        
        chosenPocket = bestPocket;
        chosenBall = bestBall;
        
        cueToChosen = chosenBall->getNode()->getPosition() 
                    - game->cueBall->getNode()->getPosition();
    }
    
    decided = true;
    
    std::cout << chosenBall->getGraphics()->name << std::endl;
}

float AIPlayer::closestChoice (const Ogre::Vector3& x, 
        const Ogre::Vector3& y, const Ogre::Vector3& z)
{
    Ogre::Degree a1 = cueToChosen.angleBetween(x);
    Ogre::Degree a2 = cueToChosen.angleBetween(y);
    Ogre::Degree a3 = cueToChosen.angleBetween(z);
    
    if (a1 <= a2 && a1 <= a3)
        return -1;
    else if (a2 <= a1 && a2 <= a3)
        return 0;
    else if (a3 <= a1 && a3 <= a2)
        return 1;
}

bool AIPlayer::giveGamePlayerInput(float& csd, float& csrx, float& csry, bool& hitBall)
{
//    Pocket* chosenPocket;
//    Ball* chosenBall;
    if (!decided)
        decideShot();
//    cueStickDelta = 75f;
    
    Ogre::Vector3 stickDir   = game->cueStick->getNode()->getOrientation()
                             * Ogre::Vector3::NEGATIVE_UNIT_Z;
    
    Ogre::Vector3 stickYAxis = game->cueStick->getNode()->getOrientation() 
                             * Ogre::Vector3::UNIT_Y;
    
    Ogre::Vector3 stickXAxis = game->cueStick->getNode()->getOrientation()
                             * Ogre::Vector3::UNIT_X;
    
    Ogre::Vector3 x1 = Ogre::Quaternion(Ogre::Degree(-1), stickYAxis) * stickDir;
    Ogre::Vector3 x2 = Ogre::Quaternion(Ogre::Degree( 0), stickYAxis) * stickDir;
    Ogre::Vector3 x3 = Ogre::Quaternion(Ogre::Degree( 1), stickYAxis) * stickDir;
    
    csrx = closestChoice(x1, x2, x3);    
    
    Ogre::Vector3 y1 = Ogre::Quaternion(Ogre::Degree(-1), stickXAxis) * stickDir;
    Ogre::Vector3 y2 = Ogre::Quaternion(Ogre::Degree( 0), stickXAxis) * stickDir;
    Ogre::Vector3 y3 = Ogre::Quaternion(Ogre::Degree( 1), stickXAxis) * stickDir;
    
    csry = closestChoice(y1, y2, y3);
    
    Player::giveGamePlayerInput(csd, csrx, csry, hitBall);
}
