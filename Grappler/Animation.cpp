//
//  Animation.cpp
//  Climber
//
//  Created by Ethan Coeytaux on 8/10/15.
//  Copyright (c) 2015 Science Class Games. All rights reserved.
//

#include "Animation.h"

#include "Camera.h"
#include "SpriteSheet.h"

Animation::Animation(SpriteSheet ** sheet, int frameStartX, int frameStartY, int numberOfFrames, int FPS, bool looping) : sheet(sheet), frameStartX(frameStartX), frameStartY(frameStartY), numberOfFrames(numberOfFrames), currentFrame(0), FPS(FPS), frameCountdown(FPS), looping(looping), loopComplete(false) { }

Animation::~Animation() { }

void Animation::update() {
    if (numberOfFrames == 1)
        return;
    
    if (!looping && loopComplete)
        return;
    
    frameCountdown--;
    if (frameCountdown <= 0) {
        frameCountdown = FPS;
        
        currentFrame++;
        if (currentFrame >= numberOfFrames) {
            currentFrame = 0;
            if (!looping)
                loopComplete = true;
        }
    }
}

void Animation::draw(Camera * camera, Coordinate coor, double angle, bool flipHorizontal, bool flipVertical) const {
    int frame = frameStartX + frameStartY + currentFrame;
    camera->drawImageFromSpriteSheet(*sheet, frame % (*sheet)->getWidthCount(), frame / (*sheet)->getWidthCount(), coor, angle, flipHorizontal, flipVertical);
}

void Animation::reset() {
    currentFrame = 0;
    frameCountdown = FPS;
    loopComplete = false;
}

bool Animation::isLoopComplete() {
    return loopComplete;
}