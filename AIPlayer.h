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
    Ogre::Vector3 cueToChosen;

    bool decided;
    float rotDelta;

    void decideShot();
    
public:
    AIPlayer(ThreeDPool* _game);
    AIPlayer(const AIPlayer& orig);
    virtual ~AIPlayer();
    
    virtual bool frameUpdate(const Ogre::FrameEvent& evt) {}
    virtual bool giveGamePlayerInput(float& csd, float& csrx, float& csry, bool& hitBall);
private:
    float closestChoice(const Ogre::Vector3& x, const Ogre::Vector3& y, const Ogre::Vector3& z);
    void calculateXYRotation();

};

#endif /* AIPLAYER_H */

