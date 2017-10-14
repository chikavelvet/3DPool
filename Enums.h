/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Enums.h
 * Author: treyr3
 *
 * Created on October 14, 2017, 4:39 PM
 */

#ifndef ENUMS_H
#define ENUMS_H

#define BIT(x) (1<<(x))

enum objType
{
    cueBallType, ballType, roomType, pocketType, stickType
};

enum collisionType {
    COL_NOTHING = 0,
    COL_STICK   = BIT(0),
    COL_CUEBALL = BIT(1),
    COL_BALL    = BIT(2),
    COL_WALL    = BIT(3),
    COL_POCKET  = BIT(4)
};

#ifdef __cplusplus
extern "C" {
#endif




#ifdef __cplusplus
}
#endif

#endif /* ENUMS_H */

