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

class AIPlayer : public Player {
public:
    AIPlayer();
    AIPlayer(const AIPlayer& orig);
    virtual ~AIPlayer();
    
    virtual bool frameUpdate(const Ogre::FrameEvent& evt) {}
private:

};

#endif /* AIPLAYER_H */

