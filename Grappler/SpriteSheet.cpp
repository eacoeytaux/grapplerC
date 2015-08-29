//
//  SpriteSheet.cpp
//  Climber
//
//  Created by Ethan Coeytaux on 8/10/15.
//  Copyright (c) 2015 Science Class Games. All rights reserved.
//

#include "SpriteSheet.h"

#include "SDL2_image/SDL_image.h"

#include "Constants.h"
#include "Camera.h"

SpriteSheet::SpriteSheet(bool * failureFlag, SDL_Renderer * renderer, std::string filePath, int widthCount, int heightCount, int scale) : widthCount(widthCount), heightCount(heightCount), scale(scale) {
    SDL_Surface * loadedSurface = IMG_Load(filePath.c_str());
    
    if (loadedSurface == nullptr) {
        printf("SpriteSheet Error: Unable to load image %s: SDL_image Error: %s\n", filePath.c_str(), IMG_GetError());
        
        if (failureFlag != nullptr)
            *failureFlag = true;
    } else {
        texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if (texture == nullptr) {
            printf("SpriteSheet Error: Unable to create texture from %s: SDL Error: %s\n", filePath.c_str(), SDL_GetError());
            
            if (failureFlag != nullptr)
                *failureFlag = true;
        }
        SDL_FreeSurface(loadedSurface);
        
        if (SDL_QueryTexture(texture, nullptr, nullptr, &imageWidth, &imageHeight) < 0) {
            printf("SpriteSheet Error: %s\n", SDL_GetError());
        } else {
            imageWidth *= scale;
            imageHeight *= scale;
        }
    }
}

SpriteSheet::~SpriteSheet() {
    SDL_DestroyTexture(texture);
    texture = nullptr;
}

SDL_Texture * SpriteSheet::getTexture() const {
    return texture;
}

int SpriteSheet::getImageWidth() const {
    return imageWidth;
}

int SpriteSheet::getImageHeight() const {
    return imageHeight;
}

int SpriteSheet::getWidthCount() const {
    return widthCount;
}

int SpriteSheet::getHeightCount() const {
    return heightCount;
}

int SpriteSheet::getScale() const {
    return scale;
}

Image SpriteSheet::toImage(bool * failureFlag) const {
    return Image(failureFlag, texture, scale);
}