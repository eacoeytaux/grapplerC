//
//  Rope.cpp
//  Climber
//
//  Created by Ethan Coeytaux on 7/12/15.
//  Copyright (c) 2015 Science Class Games. All rights reserved.
//

#include "Rope.h"

#include "Camera.h"
#include "Player.h"

const double Rope::k = 1;

Rope::Rope(Player * parent, Coordinate * root, double length) : parent(parent), origin(&(parent->center)), root(root), length(length), taughtPoint(*root) { }

Rope::~Rope() { }

Coordinate Rope::getOrigin() const {
    return *origin;
}

Coordinate Rope::getRoot() const {
    return *root;
}

bool Rope::isTaught() const {
    return taught;
}

Coordinate Rope::getTaughtPoint() const {
    double angle = constants::findAngle(*origin, *root);
    
    taughtPoint = *root + (Vector(cos(angle) * length, sin(angle) * length) * -1);
    return taughtPoint;
}

double Rope::getLength() const {
    return length;
}

void Rope::setLength(double length) {
    this->length = length;
}

void Rope::updateParabola() {
    taught = !calculateParabola(&a, &b, &c, *root, *origin, length);
}

void Rope::update() {
    if (taught) {
        double distance = constants::findDistance(*origin, *root);
        double angle = constants::findAngle(*origin, *root);
        
        taughtPoint = *root + (Vector(cos(angle) * length, sin(angle) * length) * -1);
        
        //double FcMagnitude = pow(parent->vel.getMagnitude(), 2) / length;
        //Vector Fc = Vector(cos(angle) * FcMagnitude, sin(angle) * FcMagnitude);
        
        //Vector Tg(cos(angle) * constants::gravity.getMagnitude(), sin(angle) * constants::gravity.getMagnitude());
        
        tension = Vector(*origin, taughtPoint);//parent->force * -1;// - constants::gravity);
        //tension.subtractAngle(angle + M_PI_2);
        //tension *= -(distance - length) * k;
        
        //parent->addForce(tension);
        //if (pull)
            //parent->addForce(tension);//Vector(cos(angle) * (distance - length), sin(angle) * (distance - length)));
        
        //printf("tension dx: %f, dy: %f\n", tension.getDx(), tension.getDy());
        
        //parent->setVelocity(Vector(0, 0));
        
        //parent->addVelocity(Vector(*origin, *root + Vector(cos(angle) * length, sin(angle) * length)));// * (distance - length));
    }
}

void Rope::draw(Camera * camera, const Color * color) const {
    drawParabola(camera);
    
    if (taught) {
        camera->drawRectangle(taughtPoint, 3, 3, constants::white);
        //tension.draw(*origin, camera, constants::white);
    }
}

void Rope::drawParabola(Camera * camera, const Color * color) const {
    Coordinate origin = *(this->origin);
    Coordinate root = *(this->root);
    
    if (origin.getX() > root.getX())
        constants::swap<Coordinate>(&origin, &root);
    
    if (color == nullptr)
        color = (taught ? new Color(255, 153, 153) : constants::white);
    
    int newY, lastY = calculateParabolaPoint(origin.getX(), a, b, c);
    for (int x = origin.getX() + 1; x < root.getX(); x++) {
        newY = calculateParabolaPoint(x, a, b, c);
        camera->drawLine(x - 1, lastY, x, newY, color);
        lastY = newY;
    }
    
    camera->drawLine(root.getX() - 1, lastY, root.getX(), root.getY(), color);
    
    /*int lastX = calculateParabolaPoint(leftBound, a, b, c);
    for (int i = leftBound + 1; i <= rightBound; i++) {
        int newX = calculateParabolaPoint(i, a, b, c);
        camera->drawLine(i - 1, lastX, i, newX, taught ? new Color(255, 153, 153) : constants::white);
        lastX = newX;
    }*/
}

double Rope::calculateParabolaPoint(double x, double a, double b, double c) {
    return (a * pow(x, 2)) + (b * x) + c;
}

bool Rope::calculateParabola(double * a, double * b, double * c, Coordinate coor1, Coordinate coor2, double length) {
    return calculateParabola(a, b, c, coor1.getX(), coor1.getY(), coor2.getX(), coor2.getY(), length);
}

bool Rope::calculateParabola(double * a, double * b, double * c, double x1, double y1, double x2, double y2, double length) {
    if (constants::findDistance(x1, y1, x2, y2) > (length - 0.0001)) {
        //printf("Rope Warning: distance between coordinates is greater than length of rope\n");
        
        //set a, b, c to straight line
        *a = 0;
        *b = (y2 - y1) / (x2 - x1);
        *c = y1 - (*b * x1);
        
        return false;
    }
    
    if (x1 > x2) { //switches coordinates if coor1 is further right than coor2
        double tempx = x1;
        double tempy = y1;
        x1 = x2;
        y1 = y2;
        x2 = tempx;
        y2 = tempy;
    }
    
    double dx = x2 - x1;
    double dy = y2 - y1;
    
    //calculates b and c of f(x)
    double bf = (dy - (pow(x2, 2) - pow(x1, 2))) / dx;
    double cf = y1 - (pow(x1, 2) + (bf * x1));
    
    //calculates b and c of g(x)
    double bg = (dy + (pow(x2, 2) - pow(x1, 2))) / dx;
    double cg = y1 - (-pow(x1, 2) + (bg * x1));
    
    //all parabolas containing coor1 and coor2 have equation ht(x) = t*f(x) + (1-t)*g(x)
    double t = 0.5; //t = 0.5 is straight line, all values lower have parabola opening down
    double dt = 100; //rate at which t changes when approximating
    
    //calculates t value with 6 digits of precision
    for (int i = 0; i < 10; i++) { //TODO can this be made more efficient?
        while ((calculateArcLength(-1 + (2 * t), (bg * (1 - t)) + (bf * t), (cg * (1 - t)) + (cf * t), x1, y1, x2, y2) - length) < 0) {
            t -= dt;
        }
        
        t += dt; //undoes last change which pushes t over threshold
        dt /= 10;
    }
    
    *a = -1 + (2 * t);
    *b = (bg * (1 - t)) + (bf * t);
    *c = (cg * (1 - t)) + (cf * t);
    
    return true;
}

double Rope::calculateArcLength(double a, double b, double c, double x1, double y1, double x2, double y2) {
    if (a == 0)
        return constants::findDistance(x1, y1, x2, y2);
    
    if (x1 > x2) { //switches coordinates if coor1 is further right than coor2 (should already be done)
        double tempx = x1;
        double tempy = y1;
        x1 = x2;
        y1 = y2;
        x2 = tempx;
        y2 = tempy;
    }
    
    //calculates vertex
    double vertexX = -b / (2 * a);
    double vertexY = (a * pow(vertexX, 2)) + (b * vertexX) + c;
    
    double arc1 = calculateArcLength(x1 - vertexX, y1 - vertexY) * ((x1 < vertexX) ? 1 : -1);
    double arc2 = calculateArcLength(x2 - vertexX, y2 - vertexY) * ((x2 > vertexX) ? 1 : -1);
    
    return arc1 + arc2;
}

double Rope::calculateArcLength(double b, double h) {
    b = fabs(b);
    h = fabs(h);
    
    double twoH = 2 * h;
    double bSquared = pow(b, 2);
    double root = sqrt(pow(twoH, 2) + bSquared);
    
    return (root + ((bSquared / twoH) * log((twoH + root) / b))) / 2;
}