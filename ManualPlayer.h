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

#include "Player.h"

class ManualPlayer : public Player {
public:
    ManualPlayer();
    ManualPlayer(const ManualPlayer& orig);
    virtual ~ManualPlayer();
private:

};

#endif /* MANUALPLAYER_H */

