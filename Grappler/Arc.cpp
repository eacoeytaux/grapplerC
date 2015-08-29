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

Arc::Arc(Circle circle, double angle, double arcRadians) : circle(circle), startAngle(angle), endAngle(angle + arcRadians), arcRadians(arcRadians), start(circle.getCenter() + Vector(cos(angle) * circle.getRadius(), sin(angle) * circle.getRadius())), end(circle.getCenter() + Vector(cos(angle + arcRadians) * circle.getRadius(), sin(angle + arcRadians) * circle.getRadius())) { }

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

double Arc::getArcRadians() const {
    return arcRadians;
}

bool Arc::angleWithinArc(double angle) const {
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

bool Arc::doesIntersect(Line line, Coordinate * intersectionCoor1, Coordinate * intersectionCoor2) const {
    //double startAngle = this->startAngle;
    //double endAngle = this->endAngle;
    
    //if (arcRadians < 0)
        //constants::swap<double>(&startAngle, &endAngle);
    
    Coordinate * tempIntersectionCoor1 = new Coordinate(0, 0);
    Coordinate * tempIntersectionCoor2 = new Coordinate(0, 0);
    
    if (circle.doesIntersect(line, tempIntersectionCoor1, tempIntersectionCoor2)) {
        bool coor1Intersects = false, coor2Intersects = false;
        
        if (angleWithinArc(constants::findAngle(circle.getCenter(), *tempIntersectionCoor1))) {
            if (intersectionCoor1 != nullptr)
                *intersectionCoor1 = *tempIntersectionCoor1;
            
            coor1Intersects = true;
        }
        
        if (angleWithinArc(constants::findAngle(circle.getCenter(), *tempIntersectionCoor2))) {
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
