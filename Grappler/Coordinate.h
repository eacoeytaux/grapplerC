//
//  Coordinate.h
//  Climber
//
//  Created by Ethan Coeytaux on 6/28/15.
//  Copyright (c) 2015 Science Class Games. All rights reserved.
//

#ifndef __Climber__Coordinate__
#define __Climber__Coordinate__

#include <iostream>

class Coordinate {
public:
    Coordinate(double x, double y) : x(x), y(y) { }
    
    virtual ~Coordinate() { }
    
    double getX() const {
        return x;
    }
    
    void setX(double x) {
        this->x = x;
    }
    
    void moveX(double dx) {
        x += dx;
    }
    
    double getY() const {
        return y;
    }
    
    void setY(double y) {
        this->y = y;
    }
    
    void moveY(double dy) {
        y += dy;
    }
    
    void setXY(double x, double y) {
        this->x = x;
        this->y = y;
    }
    
    void moveXY(double dx, double dy) {
        x += dx;
        y += dy;
    }
    
    std::string toString() const {
        return "Coor(" + std::to_string((int)x) + "," + std::to_string((int)y) + ")";
    }
    
private:
    double x, y;
};

#endif /* defined(__Climber__Coordinate__) */
