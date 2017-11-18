 /*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ManualPlayer.h
 * Author: treyr3
 *
 * Created on November 15, 2017, 5:19 PM
 */

#ifndef MANUALPLAYER_H
#define MANUALPLAYER_H

#include <ois/OISKeyboard.h>
#include <ois/OISMouse.h>
#include <ois/OISInputManager.h>

#include "Player.h"

class ManualPlayer : public Player {
protected:
    virtual bool keyPressed(const OIS::KeyEvent &arg) {}
    virtual bool keyReleased(const OIS::KeyEvent &arg) {}
    virtual bool mouseMoved(const OIS::MouseEvent &me);
    virtual bool mousePressed(const OIS::MouseEvent &me, OIS::MouseButtonID id) {}
    virtual bool mouseReleased(const OIS::MouseEvent &me, OIS::MouseButtonID id);
    
public:
    ManualPlayer();
    ManualPlayer(const ManualPlayer& orig);
    virtual ~ManualPlayer();
    
    virtual bool frameUpdate(const Ogre::FrameEvent& evt);
private:

};

#endif /* MANUALPLAYER_H */

