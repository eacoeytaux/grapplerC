//
//  Vector.h
//  Climber
//
//  Created by Ethan Coeytaux on 6/28/15.
//  Copyright (c) 2015 Science Class Games. All rights reserved.
//

#ifndef __Climber__Vector__
#define __Climber__Vector__

#include "Line.h"
#include "LinkedList.h"

class Vector {
public:
    Vector(double dx, double dy, const Coordinate * origin = nullptr);
    Vector(Coordinate coor1, Coordinate coor2);
    Vector(Line line);
    virtual ~Vector();
    
    double getDx() const;
    void setDx(double dx);
    double getDy() const;
    void setDy(double dy);
    bool isDxPositive();
    bool isDyPositive();
    void setDxDy(double dx, double dy);
    void setDxDy(Vector vec);
    bool getOrigin(Coordinate * coor) const;
    void setOrigin(Coordinate * coor);
    bool getDestination(Coordinate * coor) const;
    bool setDestination(Coordinate destination);
    double getMagnitude() const;
    
    void setAngle(double angle);
    void rotate(double angle);
    void flip();
    void flipHorizontal();
    void flipVertical();
    
    bool subtractAngle(double angle); //TODO delete????
    //bool subtractAngles(LinkedList<double> angleDifferences);
    
    Line toLine() const;
    Line toLine(const Coordinate coor, bool subtracted = false) const;
    
    void draw(Camera * camera, const Color * color = nullptr, int magnitude = 1) const;
    void draw(Coordinate origin, Camera * camera, const Color * color = nullptr, int magnitude = 1) const;
    
    std::string print();
    
    Vector operator+(const Vector & vec) const;
    Vector & operator+=(const Vector & vec);
    Vector operator-(const Vector & vec) const;
    Vector & operator-=(const Vector & vec);
    Vector operator*(const float & f) const;
    Vector & operator*=(const float & f);
    Vector operator/(const float & f) const;
    Vector & operator/=(const float & f);
    
private:
    double dx, dy;
    bool dxPositive, dyPositive;
    const Coordinate * origin;
    
    mutable double magnitude;
    mutable bool magnitudeSet;
};

//these are declared here because declaring them inside Coordinate.h requires including Vector.h which causes circular inclusion
inline Coordinate operator+(const Coordinate & coor, const Vector & vec) {
    return Coordinate(coor.getX() + vec.getDx(), coor.getY() + vec.getDy());
}

inline Coordinate & operator+=(Coordinate & coor, const Vector & vec) {
    coor.setX(coor.getX() + vec.getDx());
    coor.setY(coor.getY() + vec.getDy());
    return coor;
}

inline Coordinate operator-(const Coordinate & coor, const Vector & vec) {
    return Coordinate(coor.getX() - vec.getDx(), coor.getY() - vec.getDy());
}

inline Coordinate & operator-=(Coordinate & coor, const Vector & vec) {
    coor.setX(coor.getX() - vec.getDx());
    coor.setY(coor.getY() - vec.getDy());
    return coor;
}

inline Line operator+(const Line & line, const Vector & vec) {
    return Line(line.getCoor1() + vec, line.getCoor1() + vec);
}

inline Line operator-(const Line & line, const Vector & vec) {
    return Line(line.getCoor1() - vec, line.getCoor1() - vec);
}

#endif /* defined(__Climber__Vector__) */
