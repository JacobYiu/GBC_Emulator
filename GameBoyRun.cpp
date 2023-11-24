#include "Gameboy.h"
#include "LogMessage.h"

int main(int argc, char* argv[])
{
    LogMessage* logMsg = LogMessage::createLogMessageInstance();
    Gameboy* gameboy = Gameboy::createGameBoyInstance();

    gameboy -> startGameboySimulation();

    delete gameboy;
    delete logMsg;
    
    return 0;
}
