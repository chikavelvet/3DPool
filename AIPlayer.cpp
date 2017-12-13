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
#include <OgreMath.h>
#include <OgreRay.h>

#include "AIPlayer.h"
#include "ThreeDPool.h"
#include "GraphicsComponent.h"

#include "Ball.h"
#include "Pocket.h"
#include "Stick.h"
#include <random>

#include <algorithm>

const float AIPlayer::ROT_DELTA_START = 0.01;
const float AIPlayer::ROT_DELTA_MIN = 0.0000001;
const int AIPlayer::NO_ROT_COUNT_THRESHOLD = 5;

const int AIPlayer::EASY_DIFFICULTY_OFFSET = 2;
const int AIPlayer::MEDIUM_DIFFICULTY_OFFSET = 1;
const int AIPlayer::HARD_DIFFICULTY_OFFSET = 0.5;

const float AIPlayer::EASY_DIFFICULTY_PERFECT_PERCENTAGE   = 0.1;
const float AIPlayer::MEDIUM_DIFFICULTY_PERFECT_PERCENTAGE = 0.3;
const float AIPlayer::HARD_DIFFICULTY_PERFECT_PERCENTAGE   = 0.5;

const int AIPlayer::ROTATIONS_MAX = 30000;

AIPlayer::AIPlayer(ThreeDPool* _game, int _difficulty) :
        game(_game),
        decided(false),
        rotDelta(ROT_DELTA_START),
        chargeDelta(0.01f),
        noRotCount(0),
        rotatingStick(true),
        decidedChargeGoal(false),
        chargeGoal(50.0f),
        difficulty(_difficulty),
        rotations(0)
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

std::vector<Ogre::Ray> AIPlayer::makeRays(Ogre::Vector3 centerOrigin, Ogre::Vector3 direction){
    std::vector<Ogre::Ray> rays;
    Ogre::Vector3 xAxis = direction * Ogre::Vector3::UNIT_X;
    Ogre::Vector3 yAxis = direction * Ogre::Vector3::UNIT_Y;

    float radius = 5.0f;
    for(int angle = 0; angle < 360; angle += 45){
        Ogre::Vector3 xOffset = Ogre::Math::Cos(Ogre::Degree(angle)) * radius * xAxis;
        Ogre::Vector3 yOffset = Ogre::Math::Sin(Ogre::Degree(angle)) * radius * yAxis;
        rays.push_back(Ogre::Ray(centerOrigin + xOffset + yOffset, direction));
    }
    return rays;
}

bool AIPlayer::noBallsBlocking(Ogre::Vector3 cueBallDest, Ball* candidateBall, Pocket* candidatePocket){
    
    /*Variables we will need in our intersection tests*/
    Ogre::SceneNode* cueBallNode = game->cueBall->getNode();
    std::vector<Ball*> allBalls = std::vector<Ball*>(game->solidBalls);   allBalls.insert(allBalls.end(), game->stripedBalls.begin(), game->stripedBalls.end());
    allBalls.erase(std::remove(allBalls.begin(), allBalls.end(), candidateBall), allBalls.end());
    // allBalls.remove(candidateBall);
    float ballRadius = 5.0f;
    
    /*First conduct checks along line from cueball to destination*/
    Ogre::Vector3 origin(cueBallNode->getPosition());
    Ogre::Vector3 dest(cueBallDest);
    Ogre::Vector3 direction(dest - origin);
    std::vector<Ogre::Ray> rays(makeRays(origin, direction)); //9 rays: 8 rays around the cue ball, 1 in the center

    Ogre::Sphere destSphere(dest, 0.1f);
    std::pair<bool,Ogre::Real> intersection = Ogre::Ray(origin, direction).intersects(destSphere);
    assert(intersection.first);
    Ogre::Real tStop = intersection.second; //figure out where along the ray the raycasting should stop

    for(std::vector<Ball*>::iterator ballIt = allBalls.begin(); ballIt != allBalls.end(); ++ballIt) {
        Ball* curBall = *ballIt;
        Ogre::Sphere ballSphere(curBall->getNode()->getPosition(), ballRadius); //a sphere representation of the ball
        for(Ogre::Ray curRay: rays){
            intersection = curRay.intersects(ballSphere);
            if(intersection.first){
                if(intersection.second < tStop){//the intersection only counts if it happens before dest
                    return false;
                }
            }
        }
    }

    /*Next conduct checks along line from targetball to pocket*/
    origin = Ogre::Vector3(candidateBall->getNode()->getPosition());
    dest = Ogre::Vector3(candidatePocket->getNode()->getPosition());
    direction = Ogre::Vector3(dest-origin);
    rays = std::vector<Ogre::Ray>(makeRays(origin, direction));  //9 rays: 8 rays around the cue ball, 1 in the center

    destSphere = Ogre::Sphere(dest, 0.1f);
    intersection = Ogre::Ray(origin, direction).intersects(destSphere);
    assert(intersection.first);
    tStop = intersection.second; //figure out where along the ray the ray casting should stop

    for(std::vector<Ball*>::iterator ballIt = allBalls.begin(); ballIt != allBalls.end(); ++ballIt) {
        Ball* curBall = *ballIt;
        Ogre::Sphere ballSphere(curBall->getNode()->getPosition(), ballRadius); //a sphere representation of the ball
        for(Ogre::Ray curRay: rays){
            intersection = curRay.intersects(ballSphere);
            if(intersection.first){
                if(intersection.second < tStop){//the intersection only counts if it happens before dest
                    return false;
                }
            }
        }
    }


    return true;
}

bool AIPlayer::decideShot()
{
//    std::cout << "started deciding shot" << std::endl;

    Ogre::SceneNode* cueBallNode = game->cueBall->getNode();
    
    std::vector<Pocket*> pockets = game->pockets;//pockets
    std::vector<Ball*> ourBalls, oppBalls;
    
    if(game->ballsAssignedToPlayers){
        ourBalls = targetSolids ? game->solidBalls : game->stripedBalls;
        oppBalls = targetSolids ? game->stripedBalls : game->solidBalls;
    }
    else {
        ourBalls = std::vector<Ball*>(game->solidBalls);
        ourBalls.insert(ourBalls.end(), game->stripedBalls.begin(), game->stripedBalls.end());
        oppBalls = std::vector<Ball*>();    
    }
    
    if (game->activePlayerReadyToHitEightBall())
        ourBalls.push_back(game->eightBall);

    Pocket* bestClearPocket_150 = NULL;
    Ball* bestClearBall_150 = NULL;
    double bestClearDist_150 = 1500.0;
    
    Pocket* bestClearPocket_110 = NULL;
    Ball* bestClearBall_110 = NULL;
    double bestClearDist_110 = 1500.0;
    
    Pocket* bestClearPocket_lower = NULL;
    Ball* bestClearBall_lower = NULL;
    double bestClearDist_lower = 1500.0;

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

                if(proximity < bestDist_150 || proximity < bestClearDist_150){
                    Ogre::Vector3 direction = curPocket->getNode()->getPosition() - curBall->getNode()->getPosition(); 
                    direction.normalise(); float radius = 5.0f;
                    Ogre::Vector3 dest = curBall->getNode()->getPosition() - (direction * radius * 2.0f);
                    bool isClearShot = noBallsBlocking(dest, curBall, curPocket);

                    if(isClearShot)
                    {
                        if(proximity < bestClearDist_150){
                            bestClearDist_150 = proximity;
                            bestClearPocket_150 = curPocket;
                            bestClearBall_150 = curBall;
                        }
                    }
                    else{                        
                        if(proximity < bestDist_150){
                            bestDist_150 = proximity;
                            bestPocket_150 = curPocket;
                            bestBall_150 = curBall;
                        }
                    }

                }
            }
            else if (AtoB >= Ogre::Degree(110)) {

                if(proximity < bestDist_110 || proximity < bestClearDist_110){
                    Ogre::Vector3 direction = curPocket->getNode()->getPosition() - curBall->getNode()->getPosition(); 
                    direction.normalise(); float radius = 5.0f;
                    Ogre::Vector3 dest = curBall->getNode()->getPosition() - (direction * radius * 2.0f);
                    bool isClearShot = noBallsBlocking(dest, curBall, curPocket);

                    if(isClearShot)
                    {
                        if(proximity < bestClearDist_110){
                            bestClearDist_110 = proximity;
                            bestClearPocket_110 = curPocket;
                            bestClearBall_110 = curBall;
                        }
                    }
                    else{                        
                        if(proximity < bestDist_110){
                            bestDist_110 = proximity;
                            bestPocket_110 = curPocket;
                            bestBall_110 = curBall;
                        }
                    }

                }
            }
            else { // all other shots

                if(proximity < bestDist_lower || proximity < bestClearDist_lower){
                    Ogre::Vector3 direction = curPocket->getNode()->getPosition() - curBall->getNode()->getPosition(); 
                    direction.normalise(); float radius = 5.0f;
                    Ogre::Vector3 dest = curBall->getNode()->getPosition() - (direction * radius * 2.0f);
                    bool isClearShot = noBallsBlocking(dest, curBall, curPocket);

                    if(isClearShot)
                    {
                        if(proximity < bestClearDist_lower){
                            bestClearDist_lower = proximity;
                            bestClearPocket_lower = curPocket;
                            bestClearBall_lower = curBall;
                        }
                    }
                    else{                        
                        if(proximity < bestDist_lower){
                            bestDist_lower = proximity;
                            bestPocket_lower = curPocket;
                            bestBall_lower = curBall;
                        }
                    }

                }
            }
        }
    }

    Pocket* bestClearPocket = bestClearPocket_150 != NULL ? bestClearPocket_150 
                                          : (bestClearPocket_110 != NULL ? bestClearPocket_110 
                                                                    : bestClearPocket_lower);
    Ball* bestClearBall = bestClearBall_150 != NULL ? bestClearBall_150 
                                      : (bestClearBall_110 != NULL ? bestClearBall_110 
                                                              : bestClearBall_lower);
    Pocket* bestNonClearPocket = bestPocket_150 != NULL ? bestPocket_150 
                                          : (bestPocket_110 != NULL ? bestPocket_110 
                                                                    : bestPocket_lower);
    Ball* bestNonClearBall = bestBall_150 != NULL ? bestBall_150 
                                      : (bestBall_110 != NULL ? bestBall_110 
                                                              : bestBall_lower);

    if(bestClearBall == NULL || bestClearPocket == NULL){
        chosenPocket = bestNonClearPocket;
        chosenBall = bestNonClearBall;
    } else{
        chosenPocket = bestClearPocket;
        chosenBall = bestClearBall;        
    }

    // std::cout << "BALL CHOSEN: " << chosenBall;

    Ogre::Vector3 direction = chosenPocket->getNode()->getPosition() - chosenBall->getNode()->getPosition(); 
    direction.normalise();
    float radius = 5.0f;
    Ogre::Vector3 dest = chosenBall->getNode()->getPosition() - (direction * radius * 2.0f);
    float distance(Ogre::Vector3((dest-cueBallNode->getPosition()) + (chosenPocket->getNode()->getPosition() - chosenBall->getNode()->getPosition())).length());

    // std::cout << "DISTANCE: " << distance << std::endl;

    applyDifficulty(dest); //applies random offsets to dest in the x, y, and z directions

    std::cout << "DEST: " << dest << std::endl;


    cueToDest = dest - game->cueBall->getNode()->getPosition();

    Ogre::Vector3 chosenToCue(game->cueBall->getNode()->getPosition() - chosenBall->getNode()->getPosition());
    Ogre::Vector3 chosenBallToPocket(chosenPocket->getNode()->getPosition() - chosenBall->getNode()->getPosition());

    // std::cout << "Angle Between best ball and best pocket: " << 
    //         Ogre::Degree(chosenToCue.angleBetween(chosenBallToPocket)) << std::endl;

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

    if(noRotCount >= NO_ROT_COUNT_THRESHOLD || rotations > ROTATIONS_MAX) {
        rotatingStick = false;
    }

    if(cueStickRotationX == 0 && cueStickRotationY == 0) {
        ++noRotCount;
        rotDelta = std::max(ROT_DELTA_MIN, rotDelta/10.0f);
    }
    else {
        noRotCount = 0;
    }
    ++rotations;
}

float AIPlayer::randNum(){
    float r = static_cast<float>(rand())/static_cast<float>(RAND_MAX) * maxDifficultyOffset;
    return r - (maxDifficultyOffset / 2);
    // return static_cast<float>(rand() % maxDifficultyOffset); 
}

void AIPlayer::applyDifficulty(Ogre::Vector3& dest) {
    float randPercent = static_cast<float>(rand())/static_cast<float>(RAND_MAX);
    std::cout << "Randome Percentage: " << randPercent << " vs " << perfectPercentage << std::endl;
    if(randPercent > perfectPercentage) { //Messes up some % of the time (by a random amount)
        

        Ogre::Vector3 difficultyOffset(randNum(), randNum(), randNum());
        dest += difficultyOffset;
        std::cout << "applying offset: " << difficultyOffset.x << " " << difficultyOffset.y << " " << difficultyOffset.z << std::endl;        
    } else { //Hits a perfect shot the rest of the time
        std::cout << "hit PERFECT shot";
    }
}

float AIPlayer::guessStickCharge () {
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

bool AIPlayer::giveGamePlayerInput(float& csd, float& csrx, float& csry, bool& hitBall, bool& _LMBDown)
{
    if (!decided)
        if(!decideShot())
            return false;
    
    if(rotatingStick){
        calculateXYRotation();
        hitBall = false;
        LMBDown = false;
    }

    else if(!hitBall){
        LMBDown = true;
        if(!decidedChargeGoal){
            if(!calculateStickChargeGoal())
                return false;
            decidedChargeGoal = true;
        }
        guessStickCharge();
    }
        
//    if (hitBall)
//        std::cout << cueToDest.normalisedCopy() << " " << game->cueStick->getNode()->getOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Z << std::endl << chosenBall->getNode()->getPosition() << " " << chosenPocket->getNode()->getPosition() << std::endl;

    Player::giveGamePlayerInput(csd, csrx, csry, hitBall, _LMBDown);
    return true;
}


bool AIPlayer::endCurrentTurn(void){
    if(!Player::endCurrentTurn())
        return false;

    noRotCount = 0;
    std::cout << "Rotations: " << rotations << std::endl;
    rotations = 0;
    rotDelta = ROT_DELTA_START;
    rotatingStick = true;
    decided = false;
    decidedChargeGoal = false;
}