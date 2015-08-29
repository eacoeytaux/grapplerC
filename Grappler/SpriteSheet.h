//
//  SpriteSheet.h
//  Climber
//
//  Created by Ethan Coeytaux on 8/10/15.
//  Copyright (c) 2015 Science Class Games. All rights reserved.
//

#ifndef __Climber__SpriteSheet__
#define __Climber__SpriteSheet__

#include <string>

#include "Coordinate.h"
#include "Image.h"

class SDL_Texture;
class SDL_Renderer;

class Camera;

class SpriteSheet {
public:
    SpriteSheet(bool * failureFlag, SDL_Renderer * renderer, std::string filePath, int widthCount, int heightCount, int scale = 1);
    virtual ~SpriteSheet();
    
    SDL_Texture * getTexture() const;
    int getImageWidth() const;
    int getImageHeight() const;
    int getWidthCount() const;
    int getHeightCount() const;
    int getScale() const;
    
    Image toImage(bool * failureFlag) const;
    
private:
    SDL_Texture * texture;
    
    int imageWidth = 0;
    int imageHeight = 0;
    int widthCount = 0;
    int heightCount = 0;
    int scale = 1;
};

#endif /* defined(__Climber__SpriteSheet__) */
