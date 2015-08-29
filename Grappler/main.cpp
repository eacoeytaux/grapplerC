//
//  main.cpp
//  Climber
//
//  Created by Ethan Coeytaux on 6/27/15.
//  Copyright (c) 2015 Science Class Games. All rights reserved.
//

#ifdef __APPLE__
#include "SDL2/SDL.h"
#include "SDL2/SDL_joystick.h"
//#include "SDL2_mixer/SDL_mixer.h"
#endif

#include <unistd.h>

#include "Constants.h"
#include "Camera.h"
#include "Coordinate.h"
#include "AbsLevel.h"

#define FPS 60

#define AUTO_LOCK_CURSOR false

const long targetTime = 1000 / FPS;
int totalCycles = 0;

bool running = true;
bool paused = false;
unsigned int cycles = 0;

const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 640;
const int SCREEN_CENTER_X = SCREEN_WIDTH / 2;
const int SCREEN_CENTER_Y = SCREEN_HEIGHT / 2;

bool mouseTrapped = false;
Coordinate mouseCoor(0, 0);

SDL_Window  * window = nullptr;
Uint32 windowID;
bool fullscreen = false;

SDL_Renderer * renderer = nullptr;

SDL_Joystick * controller = nullptr;
int xDir = 0;
int yDir = 0;
double joystickAngle = 0;

const int CONTROLLER_DEAD_ZONE = 0;

AbsLevel * currentLevel;

bool init() {
    //initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
        printf("Main Error: %s\n", SDL_GetError());
        return false;
    } else {
        //create window
        //window = SDL_CreateWindow("Climber", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        window = SDL_CreateWindow("Climber", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == nullptr) {
            printf("Main Error: %s\n", SDL_GetError());
            return false;
        } else {
            windowID = SDL_GetWindowID(window);
            
            if (AUTO_LOCK_CURSOR)
                mouseTrapped = (SDL_SetRelativeMouseMode(SDL_TRUE) == 0);
            
            //init renderer
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if (renderer == nullptr) {
                printf("Main Error: %s\n", SDL_GetError());
                return false;
            } else {
                SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            }
            
            //init SDL2_mixer for audio
            //if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
            //    printf("Main Error: %s\n", Mix_GetError());
            //    return false;
            //}
            
            //if using controller, init controller
            SDL_JoystickEventState(SDL_ENABLE);
            controller = SDL_JoystickOpen(0);
            if (controller == nullptr) {
                printf("Controller not loaded\n");
            } else {
                printf("Controller loaded! Controller name: %s\n", SDL_JoystickName(controller));
            }
            
            return true;
        }
    }
}

void exit() {
    //exit mixer
    //Mix_Quit();
    
    //close controller
    SDL_JoystickClose(controller);
    controller = nullptr;
    
    //close window and renderer
    SDL_DestroyRenderer(renderer);
    renderer = nullptr;
    SDL_DestroyWindow(window);
    window = nullptr;
    SDL_Quit();
}

void updateLogic(float dt) {
    dt /= 1000.f;
    currentLevel->update(mouseCoor, dt);
    cycles++;
}

void updateImage(Camera * camera) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    currentLevel->draw(camera);
    if (mouseTrapped) {
        camera->drawRectangle(mouseCoor.getX(), mouseCoor.getY(), 3, 3, constants::white, true);
        camera->drawRectangle(mouseCoor.getX(), mouseCoor.getY(), 7, 7, constants::white, false);
    }
}

int main(int argc, const char * argv[]) {
    if (!init()) {
        printf("Main Error: failed to init window, all is lost...\n");
        return 0;
    }
    
    printf("FPS set to %i (target time: %li ms)\n", FPS, targetTime);
    
    //init mouseCoor
    int x;
    int y;
    SDL_GetMouseState(&x, &y);
    mouseCoor.setXY(x, y);
    
    srand(time_t(nullptr));
    
    //setup variables
    running = true;
    unsigned int start = 0, end = 0, elapsed = 0, accumulator = 0;
    
    Camera camera(renderer, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_CENTER_X, SCREEN_CENTER_Y);
    
    currentLevel = new AbsLevel(renderer);
    
    while (running) {
        start = SDL_GetTicks();
        elapsed = start - end;
        end = start;
        
        accumulator += elapsed;
        
        //while (accumulator >= targetTime) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT: {
                    running = false;
                    break;
                }
                    
                case SDL_MOUSEMOTION: {
                    int mouseX = event.motion.x;
                    int mouseY = event.motion.y;
                    
                    camera.xUnadjust(&mouseX);
                    camera.yUnadjust(&mouseY);
                    
                    mouseCoor.setXY(mouseX, mouseY);
                    
                    //camera.setCenter(event.motion.x, event.motion.y);
                    break;
                }
                    
                case SDL_KEYDOWN: {
                    SDL_Keycode keyPressed = event.key.keysym.sym;
                    
                    switch (keyPressed) {
                        case SDLK_p: {
                            paused = !paused;
                            break;
                        }
                            
                        case SDLK_r: {
                            totalCycles = 0;
                            start = SDL_GetTicks();
                            currentLevel = new AbsLevel(renderer);
                            break;
                        }
                            
                        case SDLK_t: {
                            if (mouseTrapped) {
                                mouseTrapped = !(SDL_SetRelativeMouseMode(SDL_FALSE) == 0);
                            } else {
                                mouseTrapped = (SDL_SetRelativeMouseMode(SDL_TRUE) == 0);
                                if (mouseTrapped)
                                    mouseCoor.setXY(SCREEN_CENTER_X, SCREEN_CENTER_Y);
                            }
                            break;
                        }
                            
                        case SDLK_y: {
                            SDL_SetWindowFullscreen(window, fullscreen ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP);
                            fullscreen = !fullscreen;
                            break;
                        }
                            
                        case SDLK_b: {
                            Map::flipShowBumpers();
                            break;
                        }
                            
                        case SDLK_LEFT:
                        case SDLK_a: {
                            if (paused)
                                break;
                            
                            (currentLevel->getPlayer())->setMovingLeft(true);
                            break;
                        }
                            
                        case SDLK_RIGHT:
                        case SDLK_d: {
                            if (paused)
                                break;
                            
                            (currentLevel->getPlayer())->setMovingRight(true);
                            break;
                        }
                            
                        case SDLK_SPACE: {
                            if (paused)
                                break;
                            
                            (currentLevel->getPlayer())->setJumping(true);
                            break;
                        }
                    }
                    
                    break;
                }
                    
                case SDL_KEYUP: {
                    SDL_Keycode keyPressed = event.key.keysym.sym;
                    
                    switch (keyPressed) {
                        case SDLK_LEFT:
                        case SDLK_a: {
                            if (paused)
                                break;
                            
                            (currentLevel->getPlayer())->setMovingLeft(false);
                            break;
                        }
                            
                        case SDLK_RIGHT:
                        case SDLK_d: {
                            if (paused)
                                break;
                            
                            (currentLevel->getPlayer())->setMovingRight(false);
                            break;
                        }
                    }
                    
                    break;
                }
                    
                case SDL_JOYBALLMOTION: {
                    int x = 5;
                    x++;
                    break;
                }
                    
                case SDL_JOYAXISMOTION: {
                    if (event.jaxis.axis == 0) {
                        xDir = event.jaxis.value;
                        if (event.jaxis.value < -CONTROLLER_DEAD_ZONE) {
                            xDir = -1;
                        } else if (event.jaxis.value > CONTROLLER_DEAD_ZONE) {
                            printf("Value: %i\n", event.jaxis.value);
                            xDir = 1;
                        } else {
                            xDir = 0;
                        }
                    } else if (event.jaxis.axis == 1) {
                        yDir = event.jaxis.value;
                        if (event.jaxis.value < -CONTROLLER_DEAD_ZONE) {
                            yDir = -1;
                        } else if (event.jaxis.value > CONTROLLER_DEAD_ZONE) {
                            yDir = 1;
                        } else {
                            yDir = 0;
                        }
                    }
                    
                    joystickAngle = atan2((double)yDir, (double)xDir);
                    
                    printf("%f\n", joystickAngle);
                    
                    break;
                }
            }
        }
        
        if (!running)
            break;
        
        double magnitude = constants::findDistance(0, 0, xDir, yDir) / 4096;
        camera.moveCenter(cos(joystickAngle) * magnitude, sin(joystickAngle) * magnitude);
        
        if (!paused)
            updateLogic(targetTime);
        
        accumulator -= targetTime;
        //}
        
        updateImage(&camera);
        SDL_RenderPresent(renderer);
        
        //end = SDL_GetTicks();
        //long elapsed = end - start;
        //long wait = (targetTime - elapsed);
        //if (wait >= 0) {
        //    usleep((useconds_t)(wait * 1000));
        //    //printf("Hit deadline with %ld ms to spare!\n", wait);
        //} else {
        //    printf("Missed deadline by %ld ms\n", -wait);
        //}
    }
    
    exit();
    
    return 0;
}
