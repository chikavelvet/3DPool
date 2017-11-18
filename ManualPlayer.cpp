/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ManualPlayer.cpp
 * Author: treyr3
 * 
 * Created on November 15, 2017, 5:19 PM
 */

#include "ManualPlayer.h"
#include <OgreRenderWindow.h>

ManualPlayer::ManualPlayer()
{
}

ManualPlayer::ManualPlayer(const ManualPlayer& orig) {
}

ManualPlayer::~ManualPlayer() {
}

bool ManualPlayer::frameUpdate(const Ogre::FrameEvent& evt) {

}

bool ManualPlayer::mouseMoved(const OIS::MouseEvent &me) {
    if(me.state.buttonDown(OIS::MB_Left))
    {
        cueStickDelta = me.state.Y.rel * 0.05;
    }
    else{
        cueStickRotationX = 0.01 * me.state.X.rel;
        cueStickRotationY = 0.01 * me.state.Y.rel;
    }
    return true;
}

bool ManualPlayer::mouseReleased(const OIS::MouseEvent &me, OIS::MouseButtonID id) {
    if(id==OIS::MB_Left)
        hitBall = true;
}

