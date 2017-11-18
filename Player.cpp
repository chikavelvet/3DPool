/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Player.cpp
 * Author: treyr3
 * 
 * Created on November 15, 2017, 5:16 PM
 */

#include "Player.h"

Player::Player() :
    cueStickDelta(0),
    cueStickRotationX(0),
    cueStickRotationY(0),
    hitBall(false)
{
}

Player::Player(const Player& orig) {
}

Player::~Player() {
}

bool Player::giveGamePlayerInput(float& csd, float& csrx, float& csry, bool& hb) {
    csd = cueStickDelta;
    csrx = cueStickRotationX;
    csry = cueStickRotationY;
    hb = hitBall;
    cueStickDelta = 0;
    cueStickRotationX = 0;
    cueStickRotationY = 0;
    hitBall = false;
    return true;
}