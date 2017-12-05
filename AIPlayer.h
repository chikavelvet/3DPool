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
    
    static const int NO_ROT_COUNT_THRESHOLD;
    static const float ROT_DELTA_MIN;
    static const float ROT_DELTA_START;

    bool decideShot();
    float guessStickRotation(const Ogre::Vector3& x, const Ogre::Vector3& y, const Ogre::Vector3& z);
    float guessStickCharge();
    void calculateXYRotation();
    void calculateStickChargeGoal();
    
public:
    AIPlayer(ThreeDPool* _game);
    AIPlayer(const AIPlayer& orig);
    virtual ~AIPlayer();
    
    virtual bool frameUpdate(const Ogre::FrameEvent& evt) {}
    virtual bool giveGamePlayerInput(float& csd, float& csrx, float& csry, bool& hitBall);
    virtual bool endCurrentTurn(void);


private:

};

#endif /* AIPLAYER_H */

