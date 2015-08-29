//
//  Image.cpp
//  Climber
//
//  Created by Ethan Coeytaux on 8/1/15.
//  Copyright (c) 2015 Science Class Games. All rights reserved.
//

#include "Image.h"

#include "SDL2_image/SDL_image.h"

#include "Constants.h"
#include "Camera.h"

Image::Image(bool * failureFlag, SDL_Renderer * renderer, std::string filePath, int scale) {
    this->scale = scale;
    
    SDL_Surface * loadedSurface = IMG_Load(filePath.c_str());
    
    if (loadedSurface == nullptr) {
        printf("Image Error: Unable to load image %s: SDL_image Error: %s\n", filePath.c_str(), IMG_GetError());
        
        if (failureFlag != nullptr)
            *failureFlag = true;
    } else {
        texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if (texture == nullptr) {
            printf("Image Error: Unable to create texture from %s: SDL Error: %s\n", filePath.c_str(), SDL_GetError());
            
            if (failureFlag != nullptr)
                *failureFlag = true;
        }
        SDL_FreeSurface(loadedSurface);
        
        if (SDL_QueryTexture(texture, nullptr, nullptr, &width, &height) < 0) {
            printf("Image Error: %s\n", SDL_GetError());
            if (failureFlag != nullptr)
                *failureFlag = true;
        } else {
            width *= scale;
            height *= scale;
        }
    }
}

Image::Image(bool * failureFlag, SDL_Texture * texture, int scale) : texture(texture), scale(scale) {
    if (SDL_QueryTexture(texture, nullptr, nullptr, &width, &height) < 0) {
        printf("Image Error: %s\n", SDL_GetError());
        if (failureFlag != nullptr)
            *failureFlag = true;
    } else {
        width *= scale;
        height *= scale;
    }
}

Image::~Image() {
    SDL_DestroyTexture(texture);
    texture = nullptr;
}

SDL_Texture * Image::getTexture() const {
    return texture;
}

int Image::getWidth() const {
    return width;
}

int Image::getHeight() const {
    return height;
}

int Image::getScale() const {
    return scale;
}