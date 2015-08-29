//
//  Line.h
//  Climber
//
//  Created by Ethan Coeytaux on 6/28/15.
//  Copyright (c) 2015 Science Class Games. All rights reserved.
//

#ifndef __Climber__Line__
#define __Climber__Line__

#include "Coordinate.h"

class Camera;
class Color;

class Line {
public:
    Line(Coordinate coor1, Coordinate coor2);
    Line(double x1, double y1, double x2, double y2);
    ~Line();
    
    void draw(Camera * camera, const Color * color = nullptr) const;
    
    double getAngle() const;
    Coordinate getCoor1() const;
    Coordinate getCoor2() const;
    Coordinate getCoorLow() const;
    Coordinate getCoorHigh() const;
    double getXAdjustment() const;
    double getYAdjustment() const;
    
    double getPerpendicularDx(double distance) const;
    double getPerpendicularDy(double distance) const;
    Line getPerpendicularLine(double distance) const;
    
    bool inLineX(double x) const;
    bool inLineY(double y) const;
    bool inLine(double x, double y) const;
    bool inLine(Coordinate coor) const;
    bool doesIntersect(Line otherLine, Coordinate * intersectionCoor = nullptr) const;
    
    Coordinate closestCoordinate(Coordinate coor, bool ignoreBounds = false) const;
    
private:
    void init(double x1, double y1, double x2, double y2);
    
    double angle; //in radians
    double x1, y1, x2, y2;
    double m, b;
    bool vertical, horizontal;
    
    double xAdjustment, yAdjustment;
    
    double xLow, yLow, xHigh, yHigh; //NOTE: yLow is not necessarily lower than yHigh, as yLow corresponds to xLow and yHigh corresponds to xHigh
};

#endif /* defined(__Climber__Line__) */
