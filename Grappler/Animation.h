//
//  Animation.h
//  Climber
//
//  Created by Ethan Coeytaux on 8/10/15.
//  Copyright (c) 2015 Science Class Games. All rights reserved.
//

#ifndef __Climber__Animation__
#define __Climber__Animation__

#include "Coordinate.h"

class SDL_Renderer;
class Camera;
class SpriteSheet;

class Animation {
public:
    Animation(SpriteSheet ** sheet, int frameStartX, int frameStartY, int numberOfFrames, int FPS, bool looping = true);
    virtual ~Animation();
    
    void update();
    void draw(Camera * camera, Coordinate coor, double angle = 0, bool flipHorizontal = false, bool flipVertical = false) const;
    
    void reset();
    
    bool isLoopComplete();
    
private:
    SpriteSheet ** sheet;
    int frameStartX, frameStartY, numberOfFrames, currentFrame, FPS, frameCountdown;
    bool looping, loopComplete;
};

#endif /* defined(__Climber__Animation__) */
