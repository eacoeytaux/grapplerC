//
//  Arc.cpp
//  Grappler
//
//  Created by Ethan Coeytaux on 8/29/15.
//  Copyright (c) 2015 Firework Gamelabs. All rights reserved.
//

#include "Arc.h"

#include <math.h>

#include "Constants.h"

Arc::Arc(Circle circle, double startAngle, double arcRadians) : circle(circle), startAngle(startAngle), endAngle(startAngle + arcRadians), arcRadians(arcRadians), arcLength(arcRadians * circle.getRadius()), start(circle.getCenter() + Vector(cos(startAngle) * circle.getRadius(), sin(startAngle) * circle.getRadius())), end(circle.getCenter() + Vector(cos(startAngle + arcRadians) * circle.getRadius(), sin(startAngle + arcRadians) * circle.getRadius())), positiveDir(arcRadians >= 0) { }

Arc::~Arc() { }

Circle Arc::getCircle() const {
    return circle;
}

Coordinate Arc::getStart() const {
    return start;
}

Coordinate Arc::getEnd() const {
    return end;
}

double Arc::getStartAngle() const {
    return startAngle;
}

double Arc::getEndAngle() const {
    double endAngle = this->endAngle;
    
    while (endAngle > M_PI)
        endAngle -= (M_PI * 2);
    while (endAngle < -M_PI)
        endAngle += (M_PI * 2);
    
    return endAngle;
}

double Arc::getArcRadians() const {
    return arcRadians;
}

double Arc::getArcLength() const {
    return arcLength;
}

bool Arc::isPositiveDirection() const {
    return positiveDir;
}

double Arc::convertToRadians(double distance) const {
    return circle.convertToRadians(distance);
}

double Arc::convertToDistance(double radians) const {
    return circle.convertToDistance(radians);
}

bool Arc::angleInArc(double angle) const {
    double startAngle = this->startAngle;
    double endAngle = this->endAngle;
    
    if (arcRadians < 0)
        constants::swap<double>(&startAngle, &endAngle);
    
    if (endAngle > M_PI) {
        return ((angle >= startAngle) && (angle <= (endAngle - (M_PI * 2))));
    } else if (endAngle < -M_PI) {
        return ((angle <= startAngle) && (angle >= (endAngle + (M_PI * 2))));
    } else {
        return ((angle >= startAngle) && (angle <= endAngle));
    }
}

bool Arc::inArc(Coordinate coor) const {
    return inArc(coor.getX(), coor.getY());
}

bool Arc::inArc(double x, double y) const {
    return (circle.inCircle(x, y) && angleInArc(constants::findAngle(circle.getCenter().getX(), circle.getCenter().getY(), x, y)));
}

bool Arc::doesIntersect(Line line, Coordinate * intersectionCoor1, Coordinate * intersectionCoor2) const {
    Coordinate * tempIntersectionCoor1 = new Coordinate(0, 0);
    Coordinate * tempIntersectionCoor2 = new Coordinate(0, 0);
    
    if (circle.doesIntersect(line, tempIntersectionCoor1, tempIntersectionCoor2)) {
        bool coor1Intersects = false, coor2Intersects = false;
        
        if (angleInArc(constants::findAngle(circle.getCenter(), *tempIntersectionCoor1))) {
            if (intersectionCoor1 != nullptr)
                *intersectionCoor1 = *tempIntersectionCoor1;
            
            coor1Intersects = true;
        }
        
        if (angleInArc(constants::findAngle(circle.getCenter(), *tempIntersectionCoor2))) {
            if (coor1Intersects) {
                if (intersectionCoor2 != nullptr)
                    *intersectionCoor2 = *tempIntersectionCoor2;
            } else {
                if (intersectionCoor1 != nullptr)
                    *intersectionCoor1 = *tempIntersectionCoor2;
            }
            
            coor2Intersects = true;
        }
        
        if (coor1Intersects && coor2Intersects) {
            double xDistance1 = fabs(line.getCoor1().getX() - intersectionCoor1->getX());
            double xDistance2 = fabs(line.getCoor1().getX() - intersectionCoor2->getX());
            
            if (xDistance1 > xDistance2) {
                constants::swap<Coordinate *>(&intersectionCoor1, &intersectionCoor2);
            }
            
            return true;
        }
        
        return (coor1Intersects || coor2Intersects);
    }
    
    return false;
}
