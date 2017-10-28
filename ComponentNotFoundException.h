/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ComponentNotFoundException.h
 * Author: treyr3
 *
 * Created on October 28, 2017, 2:39 PM
 */

#ifndef COMPONENTNOTFOUNDEXCEPTION_H
#define COMPONENTNOTFOUNDEXCEPTION_H

#include <stdexcept>

class ComponentNotFoundException : public std::runtime_error {
public:
    ComponentNotFoundException(const std::string& __arg);
    ComponentNotFoundException();

};

#endif /* COMPONENTNOTFOUNDEXCEPTION_H */

