#include "Gameboy.h"

int main(char argc, char* argv[])
{
    Gameboy* gameboy = Gameboy::createGameBoyInstance();

    gameboy -> startGameboySimulation();

    delete gameboy;

    return 0;
}
