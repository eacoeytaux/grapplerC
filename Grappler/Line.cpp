//
//  Line.cpp
//  Climber
//
//  Created by Ethan Coeytaux on 6/28/15.
//  Copyright (c) 2015 Science Class Games. All rights reserved.
//

#include "Line.h"

#include <math.h>

#include "Camera.h"
#include "Circle.h"

Line::Line(Coordinate coor1, Coordinate coor2) {
    init(coor1.getX(), coor1.getY(), coor2.getX(), coor2.getY());
}

Line::Line(double x1, double y1, double x2, double y2) {
    init(x1, y1, x2, y2);
}

Line::~Line() { }

void Line::init(double x1, double y1, double x2, double y2) {
    this->x1 = x1;
    this->y1 = y1;
    this->x2 = x2;
    this->y2 = y2;
    
    if (x1 <= x2) {
        xLow = x1;
        xHigh = x2;
        if ((x1 == x2) && (y1 > y2)) { //if is vertical
            yLow = y2;
            yHigh = y1;
        } else {
            yLow = y1;
            yHigh = y2;
        }
    } else {
        xLow = x2;
        yLow = y2;
        xHigh = x1;
        yHigh = y1;
    }
    
    if (xLow != xHigh) {
        if (yLow != yHigh) {
            vertical = false;
            horizontal = false;
            double dx = (xHigh - xLow);
            double dy = (yHigh - yLow);
            m = dy / dx;
            b = yLow - (m * xLow);
            angle = atan2(dy, dx);
            xAdjustment = cos(angle);
            yAdjustment = sin(angle);
        } else {
            vertical = false;
            horizontal = true;
            m = 0;
            b = yLow;
            angle = 0;
            xAdjustment = 1;
            yAdjustment = 0;
        }
    } else {
        vertical = true;
        horizontal = false;
        m = 0;
        b = 0;
        angle = M_PI / 2;
        xAdjustment = 0;
        yAdjustment = 1;
    }
}

void Line::draw(Camera * camera, const Color * color) const {
    camera->drawLine(x1, y1, x2, y2, color);
}

double Line::getAngle() const {
    return angle;
}

Coordinate Line::getCoor1() const {
    return Coordinate(x1, y1);
}

Coordinate Line::getCoor2() const {
    return Coordinate(x2, y2);
}

Coordinate Line::getCoorLow() const {
    return Coordinate(xLow, yLow);
}

Coordinate Line::getCoorHigh() const {
    return Coordinate(xHigh, yHigh);
}

double Line::getXAdjustment() const {
    return xAdjustment;
}

double Line::getYAdjustment() const {
    return yAdjustment;
}

double Line::getPerpendicularDx(double distance) const {
    return distance * (vertical ? 1 : yAdjustment);
}

double Line::getPerpendicularDy(double distance) const {
    return -distance * (vertical ? 1 : xAdjustment);
}

Line Line::getPerpendicularLine(double distance) const {
    double distanceX = getPerpendicularDx(distance);
    double distanceY = getPerpendicularDy(distance);
    return Line(x1 + distanceX, y1 + distanceY, x2 + distanceX, y2 + distanceY);
}

bool Line::inLineX(double x) const {
    if (vertical)
        return (x == xLow);
    else
        return (x >= xLow) && (x <= xHigh);
}

bool Line::inLineY(double y) const {
    if (horizontal)
        return (y == yLow);
    else if (vertical)
        return (y >= yLow) && (y <= yHigh);
    else
        return (m >= 0) ? ((y >= yLow) && (y <= yHigh)) : ((y <= yLow) && (y >= yHigh));
}

bool Line::inLine(double x, double y) const {
    //if (((m * x) + b) != y) return false; //checks to see if y = f(x) //TODO fix: rounding errors?
    return inLineX(x) && inLineY(y);
}

bool Line::inLine(Coordinate coor) const {
    return inLineX(coor.getX()) && inLineY(coor.getY());
}

bool Line::doesIntersect(Line otherLine, Coordinate * intersectionCoor) const {
    double xIntersect;
    double yIntersect;
    
    if (vertical) {
        xIntersect = xLow;
        yIntersect = (otherLine.m * xLow) + otherLine.b;
    } else if (otherLine.vertical) {
        xIntersect = otherLine.xLow;
        yIntersect = (m * otherLine.xLow) + b;
    } else if (horizontal) {
        xIntersect = (otherLine.vertical ? otherLine.xLow : (yLow - otherLine.b) / otherLine.m);
        yIntersect = yLow;
    } else if (otherLine.horizontal) {
        xIntersect = (vertical ? xLow : (otherLine.yLow - b) / m);
        yIntersect = otherLine.yLow;
    } else {
        if (m == otherLine.m) {
            if (b == otherLine.b) {
                //if (intersectionCoor != nullptr)
                    //intersectionCoor->setXY(0, b);
                return true; //TODO technically is true...
            }
        }
        xIntersect = (otherLine.b - b) / (m - otherLine.m);
        yIntersect = (m * xIntersect) + b;
    }
    
    if (inLine(xIntersect, yIntersect) && otherLine.inLine(xIntersect, yIntersect)) {
        if (intersectionCoor != nullptr)
            intersectionCoor->setXY(xIntersect, yIntersect);
        return true;
    } else {
        return false;
    }
}

Coordinate Line::closestCoordinate(Coordinate coor, bool ignoreBounds) const {
    if (vertical) {
        if (ignoreBounds || inLineY(coor.getY()))
            return Coordinate(xLow, coor.getY());
        else
            return (coor.getY() > yHigh) ? getCoorHigh() : getCoorLow();
    } else if (horizontal) {
        if (ignoreBounds || inLineX(coor.getX()))
            return Coordinate(coor.getX(), y1);
        else
            return (coor.getX() > xHigh) ? getCoorHigh() : getCoorLow();
    } else {
        double otherM = -1 / m;
        double otherB = coor.getY() - (otherM * coor.getX());
        
        double x = (b - otherB) / (otherM - m);
        double y = (m * x) + b;
        
        if (ignoreBounds || inLine(x, y))
            return Coordinate(x, y);
        else
            return ((x > xHigh) ? getCoorHigh() : getCoorLow());
    }
}