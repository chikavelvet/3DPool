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
public:
    Player();
    Player(const Player& orig);
    virtual ~Player();    

    virtual bool keyPressed(const OIS::KeyEvent &arg) {}
    virtual bool keyReleased(const OIS::KeyEvent &arg) {}
    virtual bool mouseMoved(const OIS::MouseEvent &arg) {}
    virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {}
    virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {}
    
    virtual bool frameUpdate(const Ogre::FrameEvent& evt) = 0;
private:

};

#endif /* PLAYER_H */

