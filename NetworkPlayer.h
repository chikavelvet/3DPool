/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   NetworkPlayer.h
 * Author: treyr3
 *
 * Created on November 15, 2017, 5:18 PM
 */

#ifndef NETWORKPLAYER_H
#define NETWORKPLAYER_H

#include "Player.h"

class NetworkPlayer : public Player {
public:
    NetworkPlayer();
    NetworkPlayer(const NetworkPlayer& orig);
    virtual ~NetworkPlayer();
    
    virtual bool frameUpdate(const Ogre::FrameEvent& evt) {};
private:

};

#endif /* NETWORKPLAYER_H */

