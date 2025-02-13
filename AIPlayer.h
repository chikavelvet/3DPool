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

    int rotations;

    int difficulty;
    int baseDifficulty;
    int adaptiveBoundaries[2];
    int maxDifficultyOffset;
    float perfectPercentage;
    
    static const int ROTATIONS_MAX;

    static const int NO_ROT_COUNT_THRESHOLD;
    static const float ROT_DELTA_MIN;
    static const float ROT_DELTA_START;

    static const int EASY_DIFFICULTY_OFFSET;
    static const int MEDIUM_DIFFICULTY_OFFSET;
    static const int HARD_DIFFICULTY_OFFSET;
    
    static const float EASY_DIFFICULTY_PERFECT_PERCENTAGE;
    static const float MEDIUM_DIFFICULTY_PERFECT_PERCENTAGE;
    static const float HARD_DIFFICULTY_PERFECT_PERCENTAGE;

    static const int LOWER_ADAPTIVE_BOUNDARY;
    static const int EASY_ADAPTIVE_BOUNDARIES  [2];
    static const int MEDIUM_ADAPTIVE_BOUNDARIES[2];
    static const int HARD_ADAPTIVE_BOUNDARIES  [2];
    static const int HIGHER_ADAPTIVE_BOUNDARY;
    

    bool decideShot();
    bool adaptive;

    float guessStickRotation(const Ogre::Vector3& x, const Ogre::Vector3& y, const Ogre::Vector3& z);
    float guessStickCharge();
    void calculateXYRotation();
    void applyDifficulty(Ogre::Vector3& dest);
    bool calculateStickChargeGoal();
    float randNum();
    std::vector<Ogre::Ray> makeRays(Ogre::Vector3 centerOrigin, Ogre::Vector3 direction);
    bool noBallsBlocking(Ogre::Vector3 cueBallDest, Ball* candidateBall, Pocket* candidatePocket);

public:
    AIPlayer(ThreeDPool* _game, int _difficulty, bool _adaptive = true);
    AIPlayer(const AIPlayer& orig);
    virtual ~AIPlayer();
    
    virtual bool frameUpdate(const Ogre::FrameEvent& evt) {}
    virtual bool giveGamePlayerInput(float& csd, float& csrx, float& csry, bool& hitBall, bool& _LMBDown);
    virtual bool endCurrentTurn(void);


private:

};

#endif /* AIPLAYER_H */

