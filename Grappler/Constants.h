//
//  Constants.h
//  Climber
//
//  Created by Ethan Coeytaux on 6/27/15.
//  Copyright (c) 2015 Science Class Games. All rights reserved.
//

#ifndef Climber_Constants_h
#define Climber_Constants_h

#include <math.h>

#include "Color.h"
#include "Coordinate.h"
#include "Vector.h"

#define DEBUG_MODE false

/* declare game constants */

namespace constants {
    
    /* declare commonly used numbers */
    
    const float rootTwo = sqrt(2);
    
    /* declare drag coefficients */
    const float airResistance = 0.99;
    const float frictionNorm = 0.75;
    const float rotationFrictionNorm = 0.95;
    
    /* declare force vectors */
    const static Vector gravity(0, 0.25);
    
    /* declare global functions */
    template<typename T>
    static void swap(T * t1, T * t2) {
        T temp = *t1;
        *t1 = *t2;
        *t2 = temp;
    }
    
#pragma GCC diagnostic ignored "-Wunused" //these functions ARE being used...
    static double findDistance(double x1, double y1, double x2 = 0, double y2 = 0) {
        double dx = x1 - x2;
        double dy = y1 - y2;
        
        if (dx == 0)
            return fabs(dy);
        else if (dy == 0)
            return fabs(dx);
        
        return sqrt(pow(dx, 2) + pow(dy, 2));
    }
    
    inline static double findDistance(Coordinate coor1, Coordinate coor2 = Coordinate(0, 0)) {
        return findDistance(coor1.getX(), coor1.getY(), coor2.getX(), coor2.getY());
    }
    
    inline static double findAngle(double x1, double y1, double x2, double y2) {
        return atan2(y2 - y1, x2 - x1);
    }
    
    static double findAngle(Coordinate coor1, Coordinate coor2) {
        return findAngle(coor1.getX(), coor1.getY(), coor2.getX(), coor2.getY());
    }
    
    static double findAngleDifference(double angle1, double angle2) {
        if ((angle1 > 0) ^ (angle2 > 0)) {
            double angleDifference = fabs(angle1) + fabs(angle2);
            if (angleDifference > M_PI)
                return ((M_PI * 2) - angleDifference) * ((angle1 > 0) ? 1 : -1);
            else
                return angleDifference * ((angle1 > 0) ? -1 : 1);
        } else {
            return angle2 - angle1;
        }
    }
#pragma GCC diagnostic warning "-Wunused"
    
    /* commonly used colors */
    const static Color * white = new Color(255, 255, 255);
    const static Color * black = new Color(0, 0, 0);
    const static Color * red = new Color(255, 0, 0);
    const static Color * green = new Color(0, 255, 0);
    const static Color * blue = new Color(0, 0, 255);
    const static Color * yellow = new Color(255, 255, 0);
    const static Color * magenta = new Color(255, 0, 255);
    const static Color * cyan = new Color(0, 255, 255);
};

#endif
