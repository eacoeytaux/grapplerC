//
//  Camera.cpp
//  Climber
//
//  Created by Ethan Coeytaux on 6/28/15.
//  Copyright (c) 2015 Science Class Games. All rights reserved.
//

#include "Camera.h"

#include "Color.h"
#include "Image.h"
#include "Vector.h"
#include "SpriteSheet.h"

Camera::Camera(SDL_Renderer * renderer, int width, int height, int centerX, int centerY) : renderer(renderer), halfWidth(width / 2), halfHeight(height / 2), centerX(centerX), centerY(centerY) {
    for (int i = 0; i < CIRCLE_PERCISION - 1; i++) {
        circleXCoordinates[i] = cos((M_PI * 2) * ((float)i / (float)(CIRCLE_PERCISION - 1)));
        circleYCoordinates[i] = sin((M_PI * 2) * ((float)i / (float)(CIRCLE_PERCISION - 1)));
    }
    circleXCoordinates[CIRCLE_PERCISION - 1] = circleXCoordinates[0];
    circleYCoordinates[CIRCLE_PERCISION - 1] = circleYCoordinates[0];
}

Camera::~Camera() { }

void Camera::setCenter(int x, int y) {
    centerX = x;
    centerY = y;
}

void Camera::moveCenter(int dx, int dy) {
    centerX += dx;
    centerY += dy;
}

void Camera::xAdjust(int * x) const {
    *x -= centerX - halfWidth;
}

void Camera::xUnadjust(int * x) const {
    *x += centerX - halfWidth;
}

void Camera::yAdjust(int * y) const {
    *y -= centerY - halfHeight;
}

void Camera::yUnadjust(int * y) const {
    *y += centerY - halfHeight;
}

void Camera::drawRectangle(Coordinate center, int width, int height, const Color * color, bool fill) {
    drawRectangle(center.getX(), center.getY(), width, height, color, fill);
}

void Camera::drawRectangle(int x, int y, int width, int height, const Color * color, bool fill) {
    xAdjust(&x);
    yAdjust(&y);
    
    SDL_Rect rect;
    rect.x = x - (width / 2);
    rect.y = y - (height / 2);
    rect.w = width;
    rect.h = height;
    
    if (color != nullptr)
        SDL_SetRenderDrawColor(renderer, color->getRGBA(0), color->getRGBA(1), color->getRGBA(2), color->getRGBA(3));
    if (fill)
        SDL_RenderFillRect(renderer, &rect);
    else
        SDL_RenderDrawRect(renderer, &rect);
}

void Camera::drawLine(Line line, const Color * color) {
    drawLine(line.getCoor1(), line.getCoor2(), color);
}

void Camera::drawLine(Coordinate coor1, Coordinate coor2, const Color * color) {
    drawLine(coor1.getX(), coor1.getY(), coor2.getX(), coor2.getY(), color);
}

void Camera::drawLine(int x1, int y1, int x2, int y2, const Color * color) {
    xAdjust(&x1);
    yAdjust(&y1);
    xAdjust(&x2);
    yAdjust(&y2);
    
    if (color != nullptr)
        SDL_SetRenderDrawColor(renderer, color->getRGBA(0), color->getRGBA(1), color->getRGBA(2), color->getRGBA(3));
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

void Camera::drawCircle(Circle circle, const Color * color) {
    drawCircle(circle.getCenter(), circle.getRadius(), color);
}

void Camera::drawCircle(Coordinate center, float radius, const Color * color) {
    Coordinate lastCoor = center + Vector(radius, 0);
    for (int i = 1; i < CIRCLE_PERCISION; i++) {
        Coordinate nextCoor = center + Vector(radius * circleXCoordinates[i], radius * circleYCoordinates[i]);
        drawLine(lastCoor, nextCoor, color);
        lastCoor = nextCoor;
    }
}

void Camera::drawCircle(int x, int y, float radius, const Color * color) {
    drawCircle(Coordinate(x, y), radius, color);
}

void Camera::drawArc(Arc arc, const Color * color) {
    bool negative = (arc.getArcRadians() < 0);
    
    int percision = (int)((float)CIRCLE_PERCISION * (arc.getArcRadians() / (M_PI * 2)));
    if (percision < 2)
        percision = 2;
    
    double startAngle = arc.getStartAngle();
    Coordinate lastCoor = arc.getStart();
    for (int i = 1; i < percision; i++) {
        Coordinate nextCoor = arc.getCircle().getCenter() + Vector(arc.getCircle().getRadius() * (cos(startAngle + (((M_PI * 2) * ((float)(i / (float)(CIRCLE_PERCISION - 1))))) * (negative ? -1 : 1))), arc.getCircle().getRadius() * (sin(startAngle + (((M_PI * 2) * ((float)(i / (float)(CIRCLE_PERCISION - 1)))) * (negative ? -1 : 1)))));
        drawLine(lastCoor, nextCoor, color);
        lastCoor = nextCoor;
    }
    
    //drawCircle(arc.getCircle(), color);
    //drawLine(Line(arc.getCircle().getCenter(), arc.getStart()), color);
    //drawLine(Line(arc.getCircle().getCenter(), arc.getEnd()), color);
}

void Camera::drawImage(const Image * image, Coordinate coor, double angle, bool flipHorizontal, bool flipVertical) {
    int width = image->getWidth();
    int height = image->getHeight();
    
    SDL_Rect dstRect;
    dstRect.x = coor.getX() - width / 2;
    dstRect.y = coor.getY() - height / 2;
    dstRect.w = width;
    dstRect.h = height;
    
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    if (flipHorizontal || flipVertical) {
        if (!flipVertical)
            flip = SDL_FLIP_HORIZONTAL;
        else if (!flipHorizontal)
            flip = SDL_FLIP_VERTICAL;
        else
            flip = (SDL_RendererFlip)(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
    }
    
    //SDL_Point point = {static_cast<int>(coor.getX()), static_cast<int>(coor.getY())};
    
    SDL_RenderCopyEx(renderer, image->getTexture(), nullptr, &dstRect, angle, nullptr, flip);
}

//TODO this seems really messy...
void Camera::drawImageFromSpriteSheet(const SpriteSheet * sheet, int x, int y, Coordinate coor, double angle, bool flipHorizontal, bool flipVertical) {
    int width = sheet->getImageWidth();
    int height = sheet->getImageHeight();
    int widthCount = sheet->getWidthCount();
    int heightCount = sheet->getHeightCount();
    int scale = sheet->getScale();
    
    SDL_Rect srcRect;
    srcRect.x = x * widthCount * scale * 2;
    srcRect.y = y * heightCount * scale * 2;
    srcRect.w = width / widthCount / scale;
    srcRect.h = height / heightCount / scale;
    
    SDL_Rect dstRect;
    dstRect.x = coor.getX() - (width / widthCount / 2);
    dstRect.y = coor.getY() - (height / heightCount / 2);
    dstRect.w = width / widthCount;
    dstRect.h = height / heightCount;
    
    angle /= M_PI;
    angle *= 180;
    if (flipHorizontal)
        angle *= -1;
    
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    if (flipHorizontal || flipVertical) {
        if (!flipVertical)
            flip = SDL_FLIP_HORIZONTAL;
        else if (!flipHorizontal)
            flip = SDL_FLIP_VERTICAL;
        else
            flip = (SDL_RendererFlip)(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
    }
    
    //SDL_Point point = {static_cast<int>(coor.getX()), static_cast<int>(coor.getY())};
    
    SDL_RenderCopyEx(renderer, sheet->getTexture(), &srcRect, &dstRect, angle, nullptr, flip);
}