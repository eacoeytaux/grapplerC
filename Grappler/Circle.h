//
//  Circle.h
//  Grappler
//
//  Created by Ethan Coeytaux on 8/23/15.
//  Copyright (c) 2015 Firework Gamelabs. All rights reserved.
//

#ifndef __Grappler__Circle__
#define __Grappler__Circle__

#include "Coordinate.h"
#include "Line.h"

class Camera;
class Color;

class Circle {
public:
    Circle(Coordinate center, double radius);
    Circle(double x, double y, double radius);
    virtual ~Circle();
    
    Coordinate getCenter() const;
    double getRadius() const;
    double getCircumference() const;
    
    bool inCircle(double x, double y) const;
    bool inCircle(Coordinate coor) const;
    bool doesIntersect(Circle otherCircle, Coordinate * intersectionCoor1 = nullptr, Coordinate * intersectionCoor2 = nullptr) const;
    bool doesIntersect(Line line, Coordinate * intersectionCoor1 = nullptr, Coordinate * intersectionCoor2 = nullptr) const;
    
private:
    Coordinate center;
    double radius;
};

#endif /* defined(__Grappler__Circle__) */
