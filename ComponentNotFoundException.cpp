/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ComponentNotFoundException.cpp
 * Author: treyr3
 * 
 * Created on October 28, 2017, 2:39 PM
 */

#include "ComponentNotFoundException.h"

ComponentNotFoundException::ComponentNotFoundException(const std::string& __arg) :
    std::runtime_error(__arg)
{
}

ComponentNotFoundException::ComponentNotFoundException() :
    std::runtime_error("")
{
}