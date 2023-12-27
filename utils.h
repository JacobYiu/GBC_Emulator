#pragma once
#ifndef UTILS_H
#define UTILS_H

typedef unsigned char BYTE;         // 0 to 256
typedef char SIGNED_BYTE;           // -128 to 127
typedef unsigned short WORD;        // 0 to 65535
typedef signed short SIGNED_WORD;   // -32768 to 32767

#include <sstream>
#include <iomanip>

static bool TestBit(BYTE data, BYTE bitToTest)
{
    if (((data >> bitToTest) & 1) == 1) // Corrected condition
    {
        return true;
    }
    return false;
}

static BYTE SetBit(BYTE data, BYTE bitToSet)
{
    BYTE bitSet = (0b1 << bitToSet);
    data |= bitSet;
    return data;
}

static BYTE ResetBit(BYTE data, BYTE bitToReset) // Added return type
{
    //If value is 0 then leave as it is. If 1, reset
    if(TestBit(data, bitToReset) == 1)
    {
        data = data ^ (0b1 << bitToReset); // XOR the bit so that it becomes 0
    }
    return data;
}

static BYTE BitGetVal(BYTE byteToCheck, int bitPos)
{
    if(TestBit(byteToCheck, bitPos))
    {
        return 0b1;
    }

    else
    {
        return 0b0;
    }
}

static void printHex(std::string varName, WORD value)
{
    std::cout << varName << " : " << std::hex << std::uppercase << static_cast<int>(value) << std::endl;
}

static std::string convertHex(std::string regName, WORD regVal, bool fourByte)
{
    std::ostringstream oss;
    if(!fourByte)
    {
        oss << regName << std::setw(2) << std::setfill('0') << std::hex << std::uppercase << static_cast<int>(regVal); 
    }

    else
    {
        oss << regName << std::setw(4) << std::setfill('0') << std::hex << std::uppercase << static_cast<int>(regVal);
    }

    return oss.str();
}

#endif