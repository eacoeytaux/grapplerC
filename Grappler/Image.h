//
//  Image.h
//  Climber
//
//  Created by Ethan Coeytaux on 7/13/15.
//  Copyright (c) 2015 Science Class Games. All rights reserved.
//

#ifndef Climber_Image_h
#define Climber_Image_h

#include <string>

#include "Coordinate.h"

class SDL_Texture;
class SDL_Renderer;

class Camera;

class Image {
public:
    Image(bool * failureFlag, SDL_Renderer * renderer, std::string filePath, int scale = 1);
    Image(bool * failureFlag, SDL_Texture * texture, int scale = 1);
    virtual ~Image();
    
    SDL_Texture * getTexture() const;
    int getWidth() const;
    int getHeight() const;
    int getScale() const;
    
private:
    SDL_Texture * texture;
    
    int width = 0;
    int height = 0;
    int scale = 1;
};

#endif
