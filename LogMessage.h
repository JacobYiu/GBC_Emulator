#pragma once
#ifndef LOGMESSAGE_H
#define LOGMESSAGE_H


class LogMessage{
    private:
        static LogMessage* LogMessageInstancePtr;
        LogMessage();


    public:
        LogMessage(const LogMessage &_LogMessageInstancePtr) = delete;
        ~LogMessage();
        static LogMessage* createLogMessageInstance();
        static LogMessage* getLogMsgInstance();
        static void writeToLog(char* msg);
};

#endif