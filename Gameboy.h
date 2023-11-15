#pragma once
#ifndef GAMEBOY_H
#define GAMEBOY_H

class Gameboy{
    private:
        static Gameboy* gameboyInstancePtr;
        int windowWidth = 640;
        int windowHeight = 480;
        Gameboy()
        {

        }

    public:
        Gameboy(const Gameboy &_gameboyInstancePtr) = delete;
        bool initSDL();
        bool initGL();
        void handleInput();

        static Gameboy* createGameBoyInstance();
};


#endif