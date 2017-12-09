/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Player.h
 * Author: treyr3
 *
 * Created on November 15, 2017, 5:16 PM
 */

#ifndef PLAYER_H
#define PLAYER_H

#include <OgreFrameListener.h>

#include <ois/OISKeyboard.h>
#include <ois/OISMouse.h>
#include <ois/OISInputManager.h>

class Player {
protected:
    friend class ThreeDPool;
    float cueStickDelta;
    float cueStickRotationX;
    float cueStickRotationY;
    bool hitBall;
    bool LMBDown;
    bool targetRedBall;
    int strokes;

public:
    Player();
    Player(const Player& orig);
    virtual ~Player();    
    
    void setRedBall(bool isRed) { targetRedBall = isRed; }

    virtual bool keyPressed(const OIS::KeyEvent &arg) {}
    virtual bool keyReleased(const OIS::KeyEvent &arg) {}
    virtual bool mouseMoved(const OIS::MouseEvent &me) {}
    virtual bool mousePressed(const OIS::MouseEvent &me, OIS::MouseButtonID id) {}
    virtual bool mouseReleased(const OIS::MouseEvent &me, OIS::MouseButtonID id) {}
    
    virtual bool frameUpdate(const Ogre::FrameEvent& evt) = 0;
    virtual bool giveGamePlayerInput(float& csd, float& csrx, float& csry, bool& hitBall, bool& _LMBDown);
    virtual bool endCurrentTurn(void);
    
private:

};

#endif /* PLAYER_H */

