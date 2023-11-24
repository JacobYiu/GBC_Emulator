#pragma once
#ifndef UTILS_H
#define UTILS_H

typedef unsigned char BYTE;         // 0 to 256
typedef char SIGNED_BYTE;           // -128 to 127
typedef unsigned short WORD;        // 0 to 65535
typedef signed short SIGNED_WORD;   // -32768 to 32767

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
    BYTE bitSet = (1 << bitToSet);
    data |= bitSet;
    return data;
}

static BYTE ResetBit(BYTE data, BYTE bitToReset) // Added return type
{
    data = data ^ (0b1 << bitToReset); // XOR the bit so that it becomes 0
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

#endif