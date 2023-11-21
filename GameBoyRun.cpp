#include "Gameboy.h"

int main(char argc, char* argv[])
{
    LogMessage* logMsg = LogMessage::createLogMessageInstance();
    Gameboy* gameboy = Gameboy::createGameBoyInstance();

    gameboy -> startGameboySimulation();

    delete gameboy;
    delete logMsg;
    
    return 0;
}
