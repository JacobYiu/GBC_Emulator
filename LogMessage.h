#pragma once
#ifndef LOGMESSAGE_H
#define LOGMESSAGE_H
#include "Gameboy.h"


class LogMessage{
    private:
        static LogMessage* LogMessageInstancePtr;
        LogMessage();


    public:
        LogMessage(const Gameboy &_LogMessageInstancePtr) = delete;
        ~LogMessage();
        static LogMessage* createLogMessageInstance();
        static LogMessage* getLogMsgInstance();
        static void writeToLog(char* msg);
};

#endif