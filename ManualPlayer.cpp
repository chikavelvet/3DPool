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

ManualPlayer::ManualPlayer(Ogre::RenderWindow* _mWindow) :
    mWindow(_mWindow)
{
    setUpInputListener();
}

ManualPlayer::ManualPlayer(const ManualPlayer& orig) {
}

ManualPlayer::~ManualPlayer() {
}

void ManualPlayer::setUpInputListener() {       
    OIS::ParamList pl;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;
 
    mWindow->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
    
    mInputManager = OIS::InputManager::createInputSystem( pl );
    
    mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
    mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));
    
    mMouse->setEventCallback(this);
    mKeyboard->setEventCallback(this);
}

bool ManualPlayer::frameUpdate(const Ogre::FrameEvent& evt) {
    mKeyboard->capture();
    mMouse->capture();
}
