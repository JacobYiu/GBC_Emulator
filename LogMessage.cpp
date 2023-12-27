#include <iostream>
#include <fstream>
#include "LogMessage.h"

LogMessage* LogMessage::LogMessageInstancePtr = nullptr;


LogMessage::LogMessage()
{

}

LogMessage::~LogMessage()
{

}

LogMessage* LogMessage::createLogMessageInstance()
{
    if(LogMessageInstancePtr == nullptr)
    {
        LogMessageInstancePtr = new LogMessage();
        return LogMessageInstancePtr;
    }

    else
    {
        return LogMessageInstancePtr;
    }
}

LogMessage* LogMessage::getLogMsgInstance()
{
    return LogMessageInstancePtr;
}

void LogMessage::writeToLog(std::string msg)
{
    std::ofstream logFile("error.log");

    if(!logFile.is_open())
    {
        std::cerr << "Error opening or creating the file" << std::endl;
        return;
    }

    logFile << "\n------------------------------------------------------------------------\n";
    logFile << msg;

    logFile.close();
}