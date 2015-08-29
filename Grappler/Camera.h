//
//  Camera.h
//  Climber
//
//  Created by Ethan Coeytaux on 6/28/15.
//  Copyright (c) 2015 Science Class Games. All rights reserved.
//

//The entire point of the Camera class is that no class should have to deal directly with the graphics framework, and should the framework be changed only Camera.cpp should need tweaking

#ifndef __Climber__Camera__
#define __Climber__Camera__

#ifdef __APPLE__
#include "SDL2/SDL.h"
#endif

#include "Coordinate.h"
#include "Line.h"
#include "Circle.h"
#include "Arc.h"

#define CIRCLE_PERCISION 100

class Color;
class Image;
class SpriteSheet;

class Camera {
public:
    Camera(SDL_Renderer * renderer, int width, int height, int centerX = 0, int centerY = 0);
    virtual ~Camera();
    
    void setCenter(int x, int y);
    void moveCenter(int dx, int dy);
    
    void xAdjust(int * x) const;
    void xUnadjust(int * x) const;
    void yAdjust(int * y) const;
    void yUnadjust(int * y) const;
    
    void drawRectangle(Coordinate center, int width, int height, const Color * color = nullptr, bool fill = true);
    void drawRectangle(int x, int y, int width, int height, const Color * color = nullptr, bool fill = true);
    void drawLine(Line line, const Color * color = nullptr);
    void drawLine(Coordinate coor1, Coordinate coor2, const Color * color = nullptr);
    void drawLine(int x1, int y1, int x2, int y2, const Color * color = nullptr);
    void drawCircle(Circle circle, const Color * color = nullptr);
    void drawCircle(Coordinate center, float radius, const Color * color = nullptr);
    void drawCircle(int x, int y, float radius, const Color * color = nullptr);
    void drawArc(Arc arc, const Color * color = nullptr);
    
    void drawImage(const Image * image, Coordinate coor, double angle = 0, bool flipHorizontal = false, bool flipVertical = false);
    void drawImageFromSpriteSheet(const SpriteSheet * sheet, int x, int y, Coordinate coor, double angle = 0, bool flipHorizontal = false, bool flipVertical = false);
    
private:
    SDL_Renderer * renderer = nullptr;
    
    //int width, height;
    int halfWidth, halfHeight;
    
    int centerX, centerY;
    
    float circleXCoordinates[CIRCLE_PERCISION + 1];
    float circleYCoordinates[CIRCLE_PERCISION + 1];
};

#endif /* defined(__Climber__Camera__) */
