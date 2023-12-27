#pragma once
#ifndef GAMEBOY_H
#define GAMEBOY_H

#include <SDL2/SDL.h>   /* All SDL App's need this */
#include <SDL2/SDL_opengl.h>
#include "Emulator.h"

class Gameboy{
    private:
        static Gameboy* GameboyInstancePtr;
        static Emulator* EmulatorInstancePtr;
        //Constructor when creating the Gameboy
        Gameboy();

    public:
        static SDL_Window* gWindow;
        //Used to prevent copying
        Gameboy(const Gameboy &_GameboyInstancePtr) = delete;
        //Used when deleting the gameboy instance
        ~Gameboy();
        static Gameboy* createGameBoyInstance();
        static Gameboy* getGameBoyInstance();
        void startGameboySimulation();
        void debugScreenDisplay();
        void renderGame();
        void checkFPS();
        bool initSDL();
        bool initGL();
        void handleInput(SDL_Event &event);
        void setKeyPressed(int keyPressed);
        void setKeyReleased(int keyReleased);
        void renderGameboy();

};


#endif