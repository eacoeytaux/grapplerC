 //
//  Vector.cpp
//  Climber
//
//  Created by Ethan Coeytaux on 7/30/15.
//  Copyright (c) 2015 Science Class Games. All rights reserved.
//

#include "Vector.h"

#include <math.h>

#include "Constants.h"
#include "Camera.h"
#include "Line.h"


Vector::Vector(double dx, double dy, const Coordinate * origin) : origin(origin), magnitude(0), magnitudeSet(false) {
    setDx(dx);
    setDy(dy);
}

Vector::Vector(Coordinate coor1, Coordinate coor2) : origin(nullptr), magnitude(0), magnitudeSet(false) {
    setDx(coor2.getX() - coor1.getX());
    setDy(coor2.getY() - coor1.getY());
}

Vector::Vector(Line line) : origin(nullptr), magnitude(0), magnitudeSet(false) {
    setDx(line.getCoor2().getX() - line.getCoor1().getX());
    setDy(line.getCoor2().getY() - line.getCoor1().getY());
}

Vector::~Vector() { }

double Vector::getDx() const {
    return dx;
}

void Vector::setDx(double dx) {
    this->dx = dx;
    dxPositive = dx > 0;
    magnitudeSet = angleSet = false;
}

double Vector::getDy() const {
    return dy;
}

void Vector::setDy(double dy) {
    this->dy = dy;
    dyPositive = dy > 0;
    magnitudeSet = angleSet = false;
}

bool Vector::isDxPositive() {
    return dxPositive;
}

bool Vector::isDyPositive() {
    return dyPositive;
}

void Vector::setDxDy(double dx, double dy) {
    setDx(dx);
    setDy(dy);
}

void Vector::setDxDy(Vector vec) {
    setDxDy(vec.getDx(), vec.getDy());
}

Coordinate Vector::getOrigin() const {
    return *origin;
}

void Vector::setOrigin(Coordinate * coor) {
    origin = coor;
}

Coordinate Vector::getDestination() const {
    return *origin + *this;
}

bool Vector::setDestination(Coordinate destination) {
    if (origin == nullptr)
        return false;
    
    setDx(destination.getX() - origin->getX());
    setDy(destination.getY() - origin->getY());
    
    return true;
}

double Vector::getMagnitude() const {
    if (!magnitudeSet) {
        magnitude = sqrt(pow(dx, 2) + pow(dy, 2));
        magnitudeSet = true;
    }
    return magnitude;
}

double Vector::getAngle() const {
    if (!angleSet) {
        angle = atan2(dy, dx);
        angleSet = true;
    }
    return angle;
}

void Vector::rotateToAngle(double angle) {
    double magnitude = getMagnitude();
    setDx(cos(angle) * magnitude);
    setDy(sin(angle) * magnitude);
    this->magnitude = magnitude;
    magnitudeSet = true;
}

void Vector::rotate(double angle) {
    rotateToAngle(getAngle() + angle);
}

void Vector::flip() {
    dx *= -1;
    dy *= -1;
    dxPositive = !dxPositive;
    dyPositive = !dyPositive;
}

void Vector::flipHorizontal() {
    dx *= -1;
    dxPositive = !dxPositive;
}

void Vector::flipVertical() {
    dy *= -1;
    dyPositive = !dyPositive;
}

/*bool Vector::subtractAngle(double angle) {
    if (angle == -M_PI_2) { //TODO is this worth it?
        if (dy >= 0) {
            dy = 0;
            return true;
        } else {
            return false;
        }
    } else if (angle == 0) {
        if (dx >= 0) {
            dx = 0;
            return true;
        } else {
            return false;
        }
    } else if (angle == M_PI_2) {
        if (dy <= 0) {
            dy = 0;
            return true;
        } else {
            return false;
        }
    } else if (angle == M_PI) {
        if (dx <= 0) {
            dx = 0;
            return true;
        } else {
            return false;
        }
    }
    
    double vectorAngle = atan2(dy, dx);
    double angleDifference = constants::findAngleDifference(vectorAngle, angle);//angle - vectorAngle;//(vectorAngle * (((angle > 0) ^ (vectorAngle > 0)) ? 1 : -1));
    double absAngleDifference = fabs(angleDifference);
    
    if (fabs(absAngleDifference - M_PI) < 0.0001) { //account for rounding errors
        dx = 0;
        dy = 0;
        magnitude = 0;
        magnitudeSet = true;
        return true;
    }
    
    //if (absAngleDifference == M_PI_2)
    //    return true;
    //else
    if (absAngleDifference < (M_PI_2 - 0.0001)) //angle would not affect vector
        return false;
    
    double newAngle = vectorAngle - (angleDifference - M_PI_2); //angle of new vector relative to cartesian plane
    double newMagnitude = cos(angleDifference - M_PI_2) * getMagnitude();
    
    setDx(cos(newAngle) * newMagnitude);
    setDy(sin(newAngle) * newMagnitude);
    magnitude = newMagnitude;
    magnitudeSet = true;
    
    return true;
}*/

bool Vector::subtractAngle(double angle) {
    double vectorAngle = atan2(dy, dx);
    
    double angleDifference = constants::findAngleDifference(vectorAngle, angle);
    if ((fabs(angleDifference) - M_PI_2) < -0.0001)
        return false;
    
    angleDifference += (angleDifference > 0) ? -M_PI_2 : M_PI_2;
    
    double newAngle = vectorAngle + angleDifference; //angle of new vector relative to cartesian plane
    double newMagnitude = fabs(cos(angleDifference) * getMagnitude());
    
    setDx(cos(newAngle) * newMagnitude);
    setDy(sin(newAngle) * newMagnitude);
    magnitude = newMagnitude;
    magnitudeSet = true;
    
    return true;
}

Line Vector::toLine() const {
    if (origin == nullptr)
        return toLine(Coordinate(0, 0));
    else
        return toLine(*origin);
}

Line Vector::toLine(const Coordinate coor, bool subtracted) const {
    return Line(coor.getX(), coor.getY(), coor.getX() + (subtracted ? -dx : dx), coor.getY() + (subtracted ? -dy : dy));
}

void Vector::draw(Camera * camera, const Color * color, int magnitude) const {
    if (origin != nullptr)
        draw(*origin, camera, color, magnitude);
    else
        printf("Vector Warning: attempted to draw vector with no origin\n");
}

void Vector::draw(Coordinate origin, Camera * camera, const Color * color, int magnitude) const {
    camera->drawRectangle(origin.getX(), origin.getY(), 3, 3, color);
    
    if ((fabs(dx) + fabs(dy)) < 0.0001) //account for rounding errors
        return;
    
    Coordinate destination(origin.getX() + (dx * magnitude), origin.getY() + (dy * magnitude));
    camera->drawLine(origin.getX(), origin.getY(), destination.getX(), destination.getY(), color);
    
    double angle = atan2(dy, dx);
    double piMinusAngle = M_PI - angle;// * ((angle < 0) ? -1 : 1));
    camera->drawLine(destination.getX(), destination.getY(), destination.getX() - (sin(piMinusAngle - M_PI_4) * 5), destination.getY() - (cos(piMinusAngle - M_PI_4) * 5), color);
    camera->drawLine(destination.getX(), destination.getY(), destination.getX() + (sin(piMinusAngle + M_PI_4) * 5), destination.getY() + (cos(piMinusAngle + M_PI_4) * 5), color);
}

std::string Vector::print() {
    return "Vector(" + std::to_string(dx) + "," + std::to_string(dy) + ((origin == nullptr) ? "" : ("," + origin->toString())) + ")";
}

Vector Vector::operator+(const Vector & vec) const {
    return Vector(dx + vec.getDx(), dy + vec.getDy(), origin);
}

Vector & Vector::operator+=(const Vector & vec) {
    setDx(dx + vec.dx);
    setDy(dy + vec.dy);
    return *this;
}

Vector Vector::operator-(const Vector & vec) const {
    return Vector(dx - vec.getDx(), dy - vec.getDy(), origin);
}

Vector & Vector::operator-=(const Vector & vec) {
    setDx(dx - vec.dx);
    setDy(dy - vec.dy);
    return *this;
}

Vector Vector::operator*(const float & f) const {
    return Vector(dx * f, dy * f, origin);
}

Vector & Vector::operator*=(const float & f) {
    setDx(dx * f);
    setDy(dy * f);
    return *this;
}

Vector Vector::operator/(const float & f) const {
    return Vector(dx / f, dy / f, origin);
}

Vector & Vector::operator/=(const float & f) {
    setDx(dx / f);
    setDy(dy / f);
    return *this;
}
