//
//  Rope.h
//  Climber
//
//  Created by Ethan Coeytaux on 7/12/15.
//  Copyright (c) 2015 Science Class Games. All rights reserved.
//

#ifndef __Climber__Rope__
#define __Climber__Rope__

#include "Constants.h"
#include "Coordinate.h"

class Player;

class Rope {
    friend class Player;
    
public:
    Rope(Player * parent, Coordinate * root, double length = 0);
    virtual ~Rope();
    
    Coordinate getOrigin() const;
    Coordinate getRoot() const;
    double getLength() const;
    void setLength(double length);
    bool isTaught() const;
    Coordinate getTaughtPoint() const;
    void updateParabola();
    
    void update();
    void draw(Camera * camera, const Color * color = nullptr) const;
    void drawParabola(Camera * camera, const Color * color = nullptr) const;
    
    static double calculateParabolaPoint(double x, double a, double b, double c);
    static bool calculateParabola(double * a, double * b, double * c, Coordinate coor1, Coordinate coor2, double length);
    static bool calculateParabola(double * a, double * b, double * c, double x1, double y1, double x2, double y2, double length);
    //calculates arc length of parabola a(x^2) + bx + c between x1 and x2
    static double calculateArcLength(double a, double b, double c, double x1, double y1, double x2, double y2);
    //calculates arc length from vertex to coordinate given base (dx) and height (dy)
    static double calculateArcLength(double b, double h);
    
private:
    const static double k;
    
    Player * parent;
    double length;
    Coordinate * origin, * root;
    
    Vector tension = Vector(0, 0);
    bool taught = false;
    mutable Coordinate taughtPoint; //TODO this may not be accurate
    
    double a = 0, b = 0, c = 0;
};

#endif /* defined(__Climber__Rope__) */
