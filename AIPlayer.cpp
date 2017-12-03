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

#include <algorithm>

//const float AIPlayer::ROT_DELTA_START = 0.03;
const float AIPlayer::ROT_DELTA_START = 0.003;
const float AIPlayer::ROT_DELTA_MIN = 0.0001;

AIPlayer::AIPlayer(ThreeDPool* _game) :
        game(_game),
        decided(false),
        rotDelta(ROT_DELTA_START),
        chargeDelta(0.01f),
        rotatingStick(true)
{
}

AIPlayer::AIPlayer(const AIPlayer& orig) {
}

AIPlayer::~AIPlayer() {
}

bool AIPlayer::decideShot()
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


        /*DOESN'T PICK A BALL UNTIL ALL BALLS HAVE STOPPED MOVING!*/
        if(curBall->getBody()->getLinearVelocity().length() > 0.0f){
            decided = false;
            return false;
        }

//        std::cout << curBall << std::endl;
        if(!curBall->getGraphics()->geom->isVisible())
            continue;

        Ogre::SceneNode* ballNode = curBall->getNode();
        
        for(std::vector<Pocket*>::iterator pocketIt = pockets.begin(); pocketIt != pockets.end(); ++pocketIt) {
            Pocket* curPocket = *pocketIt;
            Ogre::SceneNode* pocketNode = curPocket->getNode();
            
            Ogre::Vector3 ballToCueBallV = cueBallNode->getPosition() - ballNode->getPosition();
            Ogre::Vector3 ballToPocketV = pocketNode->getPosition() - ballNode->getPosition();
//            Ogre::Vector3 cueBallToPocketV = pocketNode->getPosition() - cueBallNode->getPosition();
            
            Ogre::Degree AtoB = ballToCueBallV.angleBetween(ballToPocketV);//alignment
            
            if(AtoB > Ogre::Degree(Ogre::Real(100))) {
                Ogre::Real proximity = ballToPocketV.length();
                Ogre::Real chance = AtoB.valueDegrees() / proximity;
                
                if(chance > bestChance){
                    bestChance = chance;
                    bestPocket = curPocket;
                    bestBall = curBall;                    
                }
            }
        }
        
        chosenPocket = bestPocket;
        chosenBall = bestBall;
        
        Ogre::Vector3 direction = chosenPocket->getNode()->getPosition() - chosenBall->getNode()->getPosition(); 
        direction.normalise();
        
        float radius = 5.0f;
        Ogre::Vector3 dest = chosenBall->getNode()->getPosition() - (direction * radius * 2.0f);

        cueToDest = dest - game->cueBall->getNode()->getPosition();
    }
    
    Ogre::Vector3 chosenToCue(game->cueBall->getNode()->getPosition() - chosenBall->getNode()->getPosition());
    Ogre::Vector3 chosenBallToPocket(chosenPocket->getNode()->getPosition() - chosenBall->getNode()->getPosition());

    std::cout << "Angle Between best ball and best pocket: " << 
            Ogre::Degree(chosenToCue.angleBetween(chosenBallToPocket)) << std::endl;
        
    decided = true;
    return true;
}

void AIPlayer::calculateXYRotation() {
    Ogre::Vector3 stickDir   = game->cueStick->getNode()->getOrientation()
                             * Ogre::Vector3::NEGATIVE_UNIT_Z;
       
//    std::cout << Ogre::Degree(cueToDest.angleBetween(stickDir)) << std::endl;
    
    Ogre::Vector3 stickYAxis = game->cueStick->getNode()->getOrientation() 
                             * Ogre::Vector3::UNIT_Y;
    
    Ogre::Vector3 stickXAxis = game->cueStick->getNode()->getOrientation()
                             * Ogre::Vector3::UNIT_X;
    
    Ogre::Vector3 x1 = Ogre::Quaternion(Ogre::Degree(-rotDelta), stickYAxis) * stickDir;
    Ogre::Vector3 x3 = Ogre::Quaternion(Ogre::Degree( rotDelta), stickYAxis) * stickDir;
    
    cueStickRotationX = guessStickRotation(x1, stickDir, x3);    
    
    Ogre::Vector3 y1 = Ogre::Quaternion(Ogre::Degree(-rotDelta), stickXAxis) * stickDir;
    Ogre::Vector3 y3 = Ogre::Quaternion(Ogre::Degree( rotDelta), stickXAxis) * stickDir;
    
    cueStickRotationY = guessStickRotation(y1, stickDir, y3);

//    std::cout << "Got here" << std::endl;
    if(cueStickRotationX == 0 && cueStickRotationY == 0) {
//        if (rotDelta > ROT_DELTA_MIN) {
//            rotDelta /= 4;
//        } else 
            rotatingStick = false;
    }
}

float AIPlayer::guessStickCharge (){
    if(game->cueStickTotal > 60.0f){
        cueStickDelta = -1.0f * std::min(chargeDelta, game->cueStickTotal - 60.0f);//if the difference is less than 1, do less than 1
    }
    else if(game->cueStickTotal < 60.0f){
        cueStickDelta = 1.0f * std::min(chargeDelta, 60.0f - game->cueStickTotal);//if the difference is less than 1, do less than 1
    }
    else {        
        cueStickDelta = 0.0f;
        hitBall = true;        
    }
}

float AIPlayer::guessStickRotation (const Ogre::Vector3& x, 
        const Ogre::Vector3& y, const Ogre::Vector3& z)
{
    Ogre::Degree a1 = cueToDest.angleBetween(x);
    Ogre::Degree a2 = cueToDest.angleBetween(y);
    Ogre::Degree a3 = cueToDest.angleBetween(z);
    
    if (a1 <= a2 && a1 <= a3)
        return -rotDelta;
    else if (a2 <= a1 && a2 <= a3)
        return 0;
    else if (a3 <= a1 && a3 <= a2)
        return rotDelta;
}

bool AIPlayer::giveGamePlayerInput(float& csd, float& csrx, float& csry, bool& hitBall)
{
    if (!decided)
        if (!decideShot())
            return false;
    
    if(rotatingStick){
        calculateXYRotation();
        hitBall = false;
    }
    else if(!hitBall){
        guessStickCharge();
    }
        
    if (hitBall)
        std::cout << cueToDest.normalisedCopy() << " " << game->cueStick->getNode()->getOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Z << std::endl << chosenBall->getNode()->getPosition() << " " << chosenPocket->getNode()->getPosition() << std::endl;

    Player::giveGamePlayerInput(csd, csrx, csry, hitBall);
}


bool AIPlayer::endCurrentTurn(void){
    if(!Player::endCurrentTurn())
        return false;

    rotDelta = ROT_DELTA_START;
    rotatingStick = true;
    decided = false;
}