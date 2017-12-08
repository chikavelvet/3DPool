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
#include <random>

#include <algorithm>

const float AIPlayer::ROT_DELTA_START = 0.05;
const float AIPlayer::ROT_DELTA_MIN = 0.00001;
const int AIPlayer::NO_ROT_COUNT_THRESHOLD = 5;

const int AIPlayer::EASY_DIFFICULTY_OFFSET = 8;
const int AIPlayer::MEDIUM_DIFFICULTY_OFFSET = 4;
const int AIPlayer::HARD_DIFFICULTY_OFFSET = 2;

const float AIPlayer::EASY_DIFFICULTY_PERFECT_PERCENTAGE   = 0.1;
const float AIPlayer::MEDIUM_DIFFICULTY_PERFECT_PERCENTAGE = 0.3;
const float AIPlayer::HARD_DIFFICULTY_PERFECT_PERCENTAGE   = 0.5;

AIPlayer::AIPlayer(ThreeDPool* _game, int _difficulty) :
        game(_game),
        decided(false),
        rotDelta(ROT_DELTA_START),
        chargeDelta(0.01f),
        noRotCount(0),
        rotatingStick(true),
        decidedChargeGoal(false),
        chargeGoal(50.0f),
        difficulty(_difficulty)

{
    assert(difficulty == 2 || difficulty == 1 || difficulty == 0); //Difficulty must be easy, medium, or hard
    switch (difficulty) {
        case 2:
            maxDifficultyOffset = HARD_DIFFICULTY_OFFSET;
            perfectPercentage = HARD_DIFFICULTY_PERFECT_PERCENTAGE;
            break;
        case 1:
            maxDifficultyOffset = MEDIUM_DIFFICULTY_OFFSET;
            perfectPercentage = MEDIUM_DIFFICULTY_PERFECT_PERCENTAGE;
            break;
        case 0:
            maxDifficultyOffset = EASY_DIFFICULTY_OFFSET;
            perfectPercentage = EASY_DIFFICULTY_PERFECT_PERCENTAGE;
            break;
    }
}

AIPlayer::AIPlayer(const AIPlayer& orig) {
}

AIPlayer::~AIPlayer() {
}

bool AIPlayer::decideShot()
{
//    std::cout << "started deciding shot" << std::endl;

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

    Pocket* bestPocket_150 = NULL;
    Ball* bestBall_150 = NULL;
    double bestDist_150 = 1500.0;
    
    Pocket* bestPocket_110 = NULL;
    Ball* bestBall_110 = NULL;
    double bestDist_110 = 1500.0;
    
    Pocket* bestPocket_lower = NULL;
    Ball* bestBall_lower = NULL;
    double bestDist_lower = 1500.0;
    
    for(std::vector<Ball*>::iterator ballIt = ourBalls.begin(); ballIt != ourBalls.end(); ++ballIt) {
        Ball* curBall = *ballIt;

        if(curBall->getBody()->getLinearVelocity().length() > 0.0f){
            decided = false;
//            std::cout << "stopped because BALLS STILL MOVING" << std::endl;
            return false;
        }
        
        if(!curBall->getGraphics()->geom->isVisible())
            continue;

        Ogre::SceneNode* ballNode = curBall->getNode();
        
        for(std::vector<Pocket*>::iterator pocketIt = pockets.begin(); pocketIt != pockets.end(); ++pocketIt) {
            Pocket* curPocket = *pocketIt;
            Ogre::SceneNode* pocketNode = curPocket->getNode();
            
            Ogre::Vector3 ballToCueBallV = cueBallNode->getPosition() - ballNode->getPosition();
            Ogre::Vector3 ballToPocketV = pocketNode->getPosition() - ballNode->getPosition();
            
            Ogre::Degree AtoB = ballToCueBallV.angleBetween(ballToPocketV);//alignment
            Ogre::Real proximity = ballToPocketV.length();
            
            if (AtoB >= Ogre::Degree(150)) {
                if (proximity < bestDist_150) {
                    bestDist_150 = proximity;
                    bestPocket_150 = curPocket;
                    bestBall_150 = curBall;
                }
            }
            else if (AtoB >= Ogre::Degree(110)) {
                if (proximity < bestDist_110) {
                    bestDist_110 = proximity;
                    bestPocket_110 = curPocket;
                    bestBall_110 = curBall;
                }
            }
            else { // all other shots
                if (proximity < bestDist_lower) {
                    bestDist_lower = proximity;
                    bestPocket_lower = curPocket;
                    bestBall_lower = curBall;
                }
            }
        }
        
        chosenPocket = bestPocket_150 != NULL ? bestPocket_150 
                                              : (bestPocket_110 != NULL ? bestPocket_110 
                                                                        : bestPocket_lower);
        chosenBall = bestBall_150 != NULL ? bestBall_150 
                                          : (bestBall_110 != NULL ? bestBall_110 
                                                                  : bestBall_lower);
        
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


    applyDifficulty(); //applies random offsets to cueToDest in the x, y, and z directions
        
    decided = true;
    return true;
}

void AIPlayer::calculateXYRotation() {
    Ogre::Vector3 stickDir   = game->cueStick->getNode()->getOrientation()
                             * Ogre::Vector3::NEGATIVE_UNIT_Z;
       
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

    if(noRotCount >= NO_ROT_COUNT_THRESHOLD) {
        rotatingStick = false;
    }

    if(cueStickRotationX == 0 && cueStickRotationY == 0) {
        ++noRotCount;
//        std::cout << "adding it" << std::endl;
//        if(rotDelta > ROT_DELTA_MIN)
        rotDelta = std::max(ROT_DELTA_MIN, rotDelta/10.0f);
//        else {
//            applyDifficulty();
//        }
    }
    else {
        noRotCount = 0;
    }
}

float AIPlayer::randNum(){
    float r = static_cast<float>(rand())/static_cast<float>(RAND_MAX) * maxDifficultyOffset;
    return r - (maxDifficultyOffset / 2);
    // return static_cast<float>(rand() % maxDifficultyOffset); 
}

void AIPlayer::applyDifficulty() {
    float randPercent = static_cast<float>(rand())/static_cast<float>(RAND_MAX);
    std::cout << "Randome Percentage: " << randPercent << " vs " << perfectPercentage << std::endl;
    if(randPercent > perfectPercentage) { //Messes up some % of the time (by a random amount)
        Ogre::Vector3 difficultyOffset(randNum(), randNum(), randNum());
        cueToDest += difficultyOffset;
        std::cout << "applying offset: " << difficultyOffset.x << " " << difficultyOffset.y << " " << difficultyOffset.z << std::endl;        
    } else { //Hits a perfect shot the rest of the time
        std::cout << "hit PERFECT shot";
    }
}

float AIPlayer::guessStickCharge (){
    if(game->cueStickTotal > chargeGoal){
        cueStickDelta = -1.0f * std::min(chargeDelta, game->cueStickTotal - chargeGoal);//if the difference is less than 1, do less than 1
    }
    else if(game->cueStickTotal < chargeGoal){
        cueStickDelta = 1.0f * std::min(chargeDelta, chargeGoal - game->cueStickTotal);//if the difference is less than 1, do less than 1
    }
    else {        
        cueStickDelta = 0.0f;
        hitBall = true;
    }
}

bool AIPlayer::calculateStickChargeGoal(){
    std::cout << "gets here" << std::endl;
    float chargeMin = 60.0f, chargeMax = 150.0f;
    float shotDistance = cueToDest.length() + Ogre::Vector3(chosenPocket->getNode()->getPosition() - chosenBall->getNode()->getPosition()).length();
    std::cout << shotDistance << std::endl;
    chargeGoal = 120.0f * (shotDistance/1200.0f);

    /*Basically clamp*/
    chargeGoal = std::min(chargeGoal, chargeMax);
    chargeGoal = std::max(chargeGoal, chargeMin);

    std::cout << "setting charge goal to " << chargeGoal << std::endl;
    return true;
}

float AIPlayer::guessStickRotation (const Ogre::Vector3& x, 
        const Ogre::Vector3& y, const Ogre::Vector3& z)
{
    Ogre::Degree a1 = cueToDest.angleBetween(x);
    Ogre::Degree a2 = cueToDest.angleBetween(y);
    Ogre::Degree a3 = cueToDest.angleBetween(z);
    
    if (a2 <= a1 && a2 <= a3)
        return 0.0f;
    else if (a1 <= a2 && a1 <= a3)
        return -rotDelta;
    else
        return rotDelta;
}

bool AIPlayer::giveGamePlayerInput(float& csd, float& csrx, float& csry, bool& hitBall)
{
    if (!decided)
        if(!decideShot())
            return false;
    
    if(rotatingStick){
        calculateXYRotation();
        hitBall = false;
    }

    else if(!hitBall){
        if(!decidedChargeGoal){
            if(!calculateStickChargeGoal())
                return false;
            decidedChargeGoal = true;
        }
        guessStickCharge();
    }
        
//    if (hitBall)
//        std::cout << cueToDest.normalisedCopy() << " " << game->cueStick->getNode()->getOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Z << std::endl << chosenBall->getNode()->getPosition() << " " << chosenPocket->getNode()->getPosition() << std::endl;

    Player::giveGamePlayerInput(csd, csrx, csry, hitBall);
    return true;
}


bool AIPlayer::endCurrentTurn(void){
    if(!Player::endCurrentTurn())
        return false;

    noRotCount = 0;
    rotDelta = ROT_DELTA_START;
    rotatingStick = true;
    decided = false;
    decidedChargeGoal = false;
}