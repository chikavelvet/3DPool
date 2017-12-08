/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   AIPlayer.h
 * Author: treyr3
 *
 * Created on November 15, 2017, 5:19 PM
 */

#ifndef AIPLAYER_H
#define AIPLAYER_H

#include "Player.h"
#include "OgreVector3.h"

#define CALC_OFFSET(x) x

class ThreeDPool;
class Ball;
class Pocket;

class AIPlayer : public Player {
protected:
    ThreeDPool* game;
    Pocket* chosenPocket;
    Ball* chosenBall;
    Ogre::Vector3 cueToDest;

    bool decided;
    bool decidedChargeGoal;
    bool rotatingStick;
    int noRotCount;
    float rotDelta;
    float chargeDelta;
    float chargeGoal;


    int difficulty;
    int maxDifficultyOffset;
    float perfectPercentage;

    static const int NO_ROT_COUNT_THRESHOLD;
    static const float ROT_DELTA_MIN;
    static const float ROT_DELTA_START;

    static const int EASY_DIFFICULTY_OFFSET;
    static const int MEDIUM_DIFFICULTY_OFFSET;
    static const int HARD_DIFFICULTY_OFFSET;
    
    static const float EASY_DIFFICULTY_PERFECT_PERCENTAGE;
    static const float MEDIUM_DIFFICULTY_PERFECT_PERCENTAGE;
    static const float HARD_DIFFICULTY_PERFECT_PERCENTAGE;

    bool decideShot();
    float guessStickRotation(const Ogre::Vector3& x, const Ogre::Vector3& y, const Ogre::Vector3& z);
    float guessStickCharge();
    void calculateXYRotation();
    void applyDifficulty();
    bool calculateStickChargeGoal();
    float randNum();
    
public:
    AIPlayer(ThreeDPool* _game, int _difficulty);
    AIPlayer(const AIPlayer& orig);
    virtual ~AIPlayer();
    
    virtual bool frameUpdate(const Ogre::FrameEvent& evt) {}
    virtual bool giveGamePlayerInput(float& csd, float& csrx, float& csry, bool& hitBall);
    virtual bool endCurrentTurn(void);


private:

};

#endif /* AIPLAYER_H */

