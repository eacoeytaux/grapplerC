//
//  Arc.h
//  Grappler
//
//  Created by Ethan Coeytaux on 8/29/15.
//  Copyright (c) 2015 Firework Gamelabs. All rights reserved.
//

#ifndef __Grappler__Arc__
#define __Grappler__Arc__

#include "Circle.h"

class Arc {
public:
    Arc(Circle circle, double angle, double arcRadians);
    virtual ~Arc();
    
    Circle getCircle() const;
    Coordinate getStart() const;
    Coordinate getEnd() const;
    double getStartAngle() const;
    double getEndAngle() const;
    double getArcRadians() const;
    double getArcLength() const;
    
    bool angleInArc(double angle) const;
    bool inArc(Coordinate coor) const;
    bool inArc(double x, double y) const;
    
    bool doesIntersect(Line line, Coordinate * intersectionCoor1 = nullptr, Coordinate * intersectionCoor2 = nullptr) const;
    //bool doesIntersect(Circle circle, Coordinate * intersectionCoor1, Coordinate * intersectionCoor2) const;
    //bool doesIntersect(Arc otherArc, Coordinate * intersectionCoor1, Coordinate * intersectionCoor2) const;
    
    //double
    
private:
    Circle circle;
    Coordinate start;
    Coordinate end;
    double startAngle, endAngle, arcRadians, arcLength;
};

#endif /* defined(__Grappler__Arc__) */
