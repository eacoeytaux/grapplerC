//
//  Circle.cpp
//  Grappler
//
//  Created by Ethan Coeytaux on 8/23/15.
//  Copyright (c) 2015 Firework Gamelabs. All rights reserved.
//

#include "Circle.h"

#include "Constants.h"
#include "Camera.h"

Circle::Circle(Coordinate center, double radius) : center(center), radius(radius) { }

Circle::Circle(double x, double y, double radius) : center(x, y), radius(radius) { }

Circle::~Circle() { }

Coordinate Circle::getCenter() const {
    return center;
}

double Circle::getRadius() const {
    return radius;
}

double Circle::getCircumference() const {
    return radius * M_PI * 2;
}

double Circle::convertToRadians(double distance) const {
    return fabs(distance / radius);
}

double Circle::convertToDistance(double radians) const {
    return fabs(radians * radius);
}

bool Circle::inCircle(double x, double y) const {
    return (constants::findDistance(center.getX(), center.getY(), x, y) <= radius);
}

bool Circle::inCircle(Coordinate coor) const {
    return inCircle(coor.getX(), coor.getY());
}

bool Circle::doesIntersect(Circle otherCircle, Coordinate * intersectionCoor1, Coordinate * intersectionCoor2) const {
    double distance = constants::findDistance(center, otherCircle.center);
    double otherCircleAngle = constants::findAngle(center, otherCircle.center);
    
    if (distance > radius + otherCircle.radius) {
        return false;
    } else if (distance == radius + otherCircle.radius) {
        if (intersectionCoor1 != nullptr)
            *intersectionCoor1 = center + Vector(cos(otherCircleAngle) * radius, sin(otherCircleAngle) * radius);
        return true;
    }
    
    double x = (pow(distance, 2) - pow(otherCircle.radius, 2) + pow(radius, 2)) / (distance * 2);
    double triangleAngle = acos(x / radius);
    
    if (intersectionCoor1 != nullptr) {
        double angle = otherCircleAngle + triangleAngle;
        *intersectionCoor1 = center + Vector(cos(angle) * radius, sin(angle) * radius);
    }
    if (intersectionCoor2 != nullptr) {
        double angle = otherCircleAngle - triangleAngle;
        *intersectionCoor2 = center + Vector(cos(angle) * radius, sin(angle) * radius);
    }
    
    return true;
}

bool Circle::doesIntersect(Line line, Coordinate * intersectionCoor1, Coordinate * intersectionCoor2) const {
    Coordinate closestCoor = line.closestCoordinate(center, true);
    double distanceFromCenter = constants::findDistance(center, closestCoor);
    
    Coordinate coor1(0, 0), coor2(0, 0);
    
    if ((distanceFromCenter > radius) || (inCircle(line.getCoor1()) && inCircle(line.getCoor2()))) {
        return false;
    } else if (distanceFromCenter < 0.0001) {
        double angle = line.getAngle();
        
        coor1 = center + Vector(cos(angle) * radius, sin(angle) * radius);
        coor2 = center - Vector(cos(angle) * radius, sin(angle) * radius);
    } else {
        double triangleAngle = acos(distanceFromCenter / radius);
        double closestCoorAngle = constants::findAngle(center, closestCoor);
        
        //TODO there should be a more efficient way to do this, this way just required less thought...
        
        double angle1 = closestCoorAngle + triangleAngle;
        coor1 = center + Vector(cos(angle1) * radius, sin(angle1) * radius);
        
        double angle2 = closestCoorAngle - triangleAngle;
        coor2 = center + Vector(cos(angle2) * radius, sin(angle2) * radius);
    }
    
    bool coor1InLine = line.inLine(coor1);
    bool coor2InLine = line.inLine(coor2);
    
    if (coor1InLine) {
        if (coor2InLine) { //two intersection points
            double xDistance1 = fabs(line.getCoor1().getX() - coor1.getX());
            double xDistance2 = fabs(line.getCoor1().getX() - coor2.getX());
            
            bool coor1Closest = (xDistance1 < xDistance2);
            
            if (intersectionCoor1 != nullptr)
                *intersectionCoor1 = coor1Closest ? coor1 : coor2;
            if (intersectionCoor2 != nullptr)
                *intersectionCoor2 = coor1Closest ? coor2 : coor1;
            return true;
        } else {
            if (intersectionCoor1 != nullptr)
                *intersectionCoor1 = coor1;
            return true;
        }
    } else if (coor2InLine) {
        if (intersectionCoor1 != nullptr)
            *intersectionCoor1 = coor2;
        return true;
    } else {
        return false;
    }
}