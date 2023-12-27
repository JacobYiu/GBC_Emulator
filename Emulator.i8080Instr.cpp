#include "Emulator.h"
#include "utils.h"
#include <iostream>


//--------------------------------------------------------------------------------------------------------
//                                    LOAD and WRITE 8/16 BIT INSTRUCTIONS
//--------------------------------------------------------------------------------------------------------
void Emulator::CPU_8BIT_LOAD(BYTE &reg)
{
    BYTE newData = readByte(program_Counter);
    program_Counter ++;
    reg = newData;
}

void Emulator::CPU_REG2REG_LOAD(BYTE &reg1, BYTE &reg2)
{
    reg1 = reg2;
}

void Emulator::CPU_REG_LOAD_ROM(BYTE &reg, WORD addrInReg)
{
    BYTE newData = addrInReg;
    reg = readByte(addrInReg);
}

void Emulator::CPU_16BIT_LOAD(WORD &regWord)
{
    WORD newData = readWord();
    program_Counter += 2;
    regWord = newData;
}

//--------------------------------------------------------------------------------------------------------
//                                    Increase/Decrease 8/16 Bit
//--------------------------------------------------------------------------------------------------------
void Emulator::CPU_16BIT_DEC(WORD &regWord)
{
    regWord --;
}

void Emulator::CPU_16BIT_INC(WORD &regWord)
{
    regWord ++;
}

//We cant reset the whole register because C is unaffected. We should not change the value of C at all
void Emulator::CPU_8BIT_INC(BYTE &reg1)
{
    BYTE preVal = reg1;   
    reg1 += 1;
    
    /*
    ----------------
        set flags
    ----------------
    */
    //Set/Reset Zero flag
    reg_AF.lo = (reg1 == 0)? SetBit(reg_AF.lo, FLAG_Z) : (ResetBit(reg_AF.lo, FLAG_Z));
   
    //Reset Negative Flag 
    reg_AF.lo = ResetBit(reg_AF.lo, FLAG_N);

    //Set/Reset Half Carry flag
    reg_AF.lo = (((preVal & 0xF) + 0x1)> 0xF)? SetBit(reg_AF.lo, FLAG_H) : (ResetBit(reg_AF.lo, FLAG_H));

    //Do not set the carry flag. 
}

//Contains Address and need to get the address and increment the value inside that address by 1 
void Emulator::CPU_8BIT_MEMORY_INC(WORD regWord)
{
    BYTE addrVal = readByte(regWord);
    BYTE preVal = addrVal;
    addrVal ++;
    writeByte(regWord, addrVal);

    reg_AF.lo = (addrVal == 0)? SetBit(reg_AF.lo, FLAG_Z) : (ResetBit(reg_AF.lo, FLAG_Z));
   
   //Reset Negative Flag 
    reg_AF.lo = ResetBit(reg_AF.lo, FLAG_N);

    //Set/Reset Half Carry flag
    reg_AF.lo = (((preVal & 0xF) + 0x1) > 0xF)? SetBit(reg_AF.lo, FLAG_H) : (ResetBit(reg_AF.lo, FLAG_H));

    //Do not set the carry flag. 
}

//We cant reset the whole register because C is unaffected. We should not change the value of C at all
void Emulator::CPU_8BIT_DEC(BYTE &reg1)
{
    BYTE preVal = reg1;
    reg1 -= 1;
    
    /*
    ----------------
        set flags
    ----------------
    */
   //Set/Reset Zero flag
    reg_AF.lo = (reg1 == 0)? SetBit(reg_AF.lo, FLAG_Z) : (ResetBit(reg_AF.lo, FLAG_Z));
   
   //Reset Negative Flag 
    reg_AF.lo = SetBit(reg_AF.lo, FLAG_N);

    //Set/Reset Half Carry flag
    reg_AF.lo = ((preVal & 0xF) - 1 < 0)? SetBit(reg_AF.lo, FLAG_H) : (ResetBit(reg_AF.lo, FLAG_H));

    //Do not set the carry flag. 
}

//Contains Address and need to get the address and decrement the value inside that address by 1 
void Emulator::CPU_8BIT_MEMORY_DEC(WORD regWord)
{
    BYTE addrVal = readByte(regWord);
    BYTE preVal = addrVal;
    addrVal --;
    writeByte(regWord, addrVal);

    /*
    ----------------
        set flags
    ----------------
    */

   //Set Zero Flag
    reg_AF.lo = (addrVal == 0)? SetBit(reg_AF.lo, FLAG_Z) : (ResetBit(reg_AF.lo, FLAG_Z));
   
   //Reset Negative Flag 
    reg_AF.lo = SetBit(reg_AF.lo, FLAG_N);

    //Set/Reset Half Carry flag
    reg_AF.lo = ((preVal & 0xF) == 0)? SetBit(reg_AF.lo, FLAG_H) : (ResetBit(reg_AF.lo, FLAG_H));

    //Do not set the carry flag. 
}

//--------------------------------------------------------------------------------------------------------
//                                    Arithmethic 8/16 Bit
//--------------------------------------------------------------------------------------------------------
void Emulator::CPU_8BIT_ADD(BYTE &reg1, BYTE regToAdd, bool useImmediateValue, bool carryFlag)
{
    BYTE valueToAdd = 0;
    WORD finalVal = 0;

    if(useImmediateValue)
    {
        valueToAdd = readByte(program_Counter);
        program_Counter ++;
    }

    else
    {
        valueToAdd = regToAdd;
    }
    
    BYTE prevValueToAdd = valueToAdd;
    BYTE carry = 0;

    if(carryFlag)
    {
        if(TestBit(reg_AF.lo, FLAG_C) == true)
        {
            carry = 1;
            valueToAdd += 1;
        }
    }

    finalVal = reg1 + valueToAdd;    

    /*
    ----------------
        set flags
    ----------------
    */

    // set Z flag
    reg_AF.lo = ((finalVal & 0xFF) == 0)? SetBit(reg_AF.lo, FLAG_Z) : ResetBit(reg_AF.lo, FLAG_Z);  

    // set N flag
    reg_AF.lo = ResetBit(reg_AF.lo, FLAG_N);

    //set Carry flag
    WORD valForCarry = (reg1 & 0xFF) + (prevValueToAdd & 0xFF) + carry;

    reg_AF.lo = (valForCarry > 0xFF) ? SetBit(reg_AF.lo, FLAG_C): ResetBit(reg_AF.lo, FLAG_C) ;
    
    

    //set Half carry flag

    if((reg1 & 0xF) + (prevValueToAdd & 0xF) + carry > 0xF)
    {
        reg_AF.lo = SetBit(reg_AF.lo, FLAG_H);
    }

    else
    {
        reg_AF.lo = ResetBit(reg_AF.lo, FLAG_H);
    }
    

    reg1 = (BYTE)finalVal;
}

void Emulator::CPU_8BIT_SUB(BYTE &reg1, BYTE regToSub, bool useImmediateValue, bool carryFlag)
{
    BYTE valueToSub = 0;
    BYTE finalVal = 0;

    if(useImmediateValue)
    {
        valueToSub = readByte(program_Counter);
        program_Counter++;
    }

    else
    {
        valueToSub = regToSub;
    }

    BYTE prevValueToSub = valueToSub;
    BYTE carry = 0;

    if(carryFlag)
    {
        if(TestBit(reg_AF.lo, FLAG_C) == true)
        {
            carry = 1;
            valueToSub += 1;
        }
    }

    finalVal = reg1 - valueToSub;

    /*
    ----------------
        set flags
    ----------------
    */

    reg_AF.lo = 0;

    //set Z flag 
    if(finalVal == 0)
    {
        reg_AF.lo = SetBit(reg_AF.lo, FLAG_Z);
    }

    //set Negative flag
    reg_AF.lo = SetBit(reg_AF.lo, FLAG_N);

    //set Carry flag if the 8th bit is not set
    
    if(reg1 < (prevValueToSub + carry)) 
    {
        reg_AF.lo = SetBit(reg_AF.lo, FLAG_C);
    }

    //set Half carry flag if the 5th bit is set
    //If you think about it. A carry only needs to happen if the value is less than the valueToSub

    if((reg1 & 0xF) < ((prevValueToSub & 0xF) + carry))
    {
        reg_AF.lo = SetBit(reg_AF.lo, FLAG_H);
    }

    reg1 = finalVal;
}

void Emulator::CPU_8BIT_AND(BYTE &reg1, BYTE reg2, bool useImmediateValue)
{
    BYTE valueToAnd = 0;
    if(useImmediateValue)
    {
        valueToAnd = readByte(program_Counter);
        program_Counter++;
    }

    else
    {
        valueToAnd = reg2;
    }

    reg1 = reg1 & valueToAnd;

    /*
    ----------------
        set flags
    ----------------
    */

    reg_AF.lo = 0;

    reg_AF.lo = SetBit(reg_AF.lo, FLAG_H);

    if(reg1 == 0)
    {
        reg_AF.lo = SetBit(reg_AF.lo, FLAG_Z);
    }
}

void Emulator::CPU_8BIT_OR(BYTE &reg1, BYTE reg2, bool useImmediateValue)
{
    BYTE valueToOr = 0;
    if(useImmediateValue)
    {
        valueToOr = readByte(program_Counter);
        program_Counter++;
    }

    else
    {
        valueToOr = reg2;
    }

    reg1 = reg1 | valueToOr;

    /*
    ----------------
        set flags
    ----------------
    */

    reg_AF.lo = 0;

    if(reg1 == 0)
    {
        reg_AF.lo = SetBit(reg_AF.lo, FLAG_Z);
    }
}

void Emulator::CPU_8BIT_XOR(BYTE &reg1, BYTE reg2, bool useImmediateValue)
{
    BYTE valueToXor = 0;
    if(useImmediateValue)
    {
        valueToXor = readByte(program_Counter);
        program_Counter++;
    }

    else
    {
        valueToXor = reg2;
    }

    reg1 = reg1 ^ valueToXor;

    /*
    ----------------
        set flags
    ----------------
    */
    
    reg_AF.lo = 0;

    if(reg1 == 0)
    {
        reg_AF.lo = SetBit(reg_AF.lo, FLAG_Z);
    }
}

//compare reg1 to reg2
void Emulator::CPU_8BIT_COMPARE(BYTE reg1, BYTE reg2, bool useImmediateValue)
{
    BYTE valueToCmp = 0;
    BYTE finalVal = 0;

    if(useImmediateValue)
    {
        valueToCmp = readByte(program_Counter);
        program_Counter++;
    }

    else
    {
        valueToCmp = reg2;
    }

    finalVal = reg1 - valueToCmp;

    /*
    ----------------
        set flags
    ----------------
    */

    reg_AF.lo = 0;

    //set Z flag 
    if(finalVal == 0)
    {
        reg_AF.lo = SetBit(reg_AF.lo, FLAG_Z);
    }

    //set Negative flag
    reg_AF.lo = SetBit(reg_AF.lo, FLAG_N);

    //set Carry flag if the 16th bit is not set
    
    if(reg1 < valueToCmp) 
    {
        reg_AF.lo = SetBit(reg_AF.lo, FLAG_C);
    }

    //set Half carry flag if the 5th bit is set
    if((reg1 & 0xF) < (valueToCmp & 0xF))
    {
        reg_AF.lo = SetBit(reg_AF.lo, FLAG_H);
    }
}


void Emulator::CPU_16BIT_ADD(WORD &reg1, WORD regToAdd)
{
    WORD finalVal = reg1 + regToAdd;

    /*
    ----------------
        set flags
    ----------------
    */
    //This time we do not set the whole flag to zero because the Z flag cannot be unaffected
    //Reset N Flag
    reg_AF.lo = ResetBit(reg_AF.lo, FLAG_N);

    //Set C Flag
    if((reg1 + regToAdd) > 0xFFFF)
    {
        reg_AF.lo = SetBit(reg_AF.lo, FLAG_C);
    }  

    else
    {
        reg_AF.lo = ResetBit(reg_AF.lo, FLAG_C);
    }

    //Set H Flag
    if((reg1 & 0xFFF) + (regToAdd & 0xFFF)  > 0xFFF)
    {
        reg_AF.lo = SetBit(reg_AF.lo, FLAG_H);
    }

    else
    {
        reg_AF.lo = ResetBit(reg_AF.lo, FLAG_H);
    }

    reg1 = finalVal;
}

//--------------------------------------------------------------------------------------------------------
//                                    Branch INSTRUCTIONS
//--------------------------------------------------------------------------------------------------------
void Emulator::CPU_JUMP_TO_N(bool useConditions, int flag, bool conditionToSatisfy)
{
    WORD addrToJump = readWord();
    
    program_Counter += 2;

    if(!useConditions)
    {   
        program_Counter = addrToJump;
        return;
    }

    /*
    ----------------
        set flags
    ----------------
    */

    if(TestBit(reg_AF.lo, flag) == conditionToSatisfy)
    {
        program_Counter = addrToJump;
    }
}

void Emulator::CPU_JUMP_BY_N(bool useConditions, int flag, bool conditionToSatisfy)
{
    SIGNED_BYTE addrToJumpBy = readByte(program_Counter);
    // printHex("addr to Jump by in 2s Complement is ", addrToJumpBy);
    program_Counter ++;
    if(!useConditions)
    {   
        program_Counter += addrToJumpBy;
        return;
    }

    /*
    ----------------
        set flags
    ----------------
    */
    if(TestBit(reg_AF.lo, flag) == conditionToSatisfy)
    {
        program_Counter += addrToJumpBy;
    }
}

void Emulator::CPU_CALL(bool useConditions, int flag, bool conditionToSatisfy)
{
    WORD addrToJumpTo = readWord();
    program_Counter += 2;

    if(!useConditions)
    {
        PushWordToStack(program_Counter);
        program_Counter = addrToJumpTo;
        return;
    }

    /*
    ----------------
        set flags
    ----------------
    */

    if(TestBit(reg_AF.lo, flag) == conditionToSatisfy)
    {
        PushWordToStack(program_Counter);
        program_Counter = addrToJumpTo;
    }
}

void Emulator::CPU_RETURN(bool useConditions, int flag, bool conditionToSatisfy)
{
    if(!useConditions)
    {
        WORD addrToJumpBackTo = PopWordOffStack();
        program_Counter = addrToJumpBackTo;
        return;
    }

    /*
    ----------------
        set flags
    ----------------
    */

    if(TestBit(reg_AF.lo, flag) == conditionToSatisfy)
    {
        WORD addrToJumpBackTo = PopWordOffStack();
        program_Counter = addrToJumpBackTo;
    }
}


//--------------------------------------------------------------------------------------------------------
//                                    RESTART CPU
//--------------------------------------------------------------------------------------------------------
void Emulator::CPU_RESTART(BYTE addrToJumpTo)
{
    PushWordToStack(program_Counter);
    program_Counter = addrToJumpTo;
}


//--------------------------------------------------------------------------------------------------------
//                                    Decimal Adjust
//--------------------------------------------------------------------------------------------------------
void Emulator::CPU_DAA()             
{
    BYTE reg = reg_AF.hi;

    WORD correction = TestBit(reg_AF.lo, FLAG_C) ? 0x60 : 0x00;

    if (TestBit(reg_AF.lo, FLAG_H) || (!TestBit(reg_AF.lo, FLAG_N) && ((reg & 0x0F) > 9))) {
        correction |= 0x06;
    }

    if (TestBit(reg_AF.lo, FLAG_C) || (!TestBit(reg_AF.lo, FLAG_N) && (reg > 0x99))) {
        correction |= 0x60;
    }

    if (TestBit(reg_AF.lo, FLAG_N)) {
        reg = static_cast<BYTE>(reg - correction);
    } else {
        reg = static_cast<BYTE>(reg + correction);
    }

    //Checks if 6th bit is set
    //0x60 = 0b1100 000 << 2 = 0b110 000 000
    //0b110 000 000
    //-----&-------
    //0b100 000 000
    //-------------
    //0b100 000 000
    //If there is a correct of 0x60. You definitely need to set carry flag because it will be greater than
    if (((correction << 2) & 0x100) != 0) {
        reg_AF.lo = SetBit(reg_AF.lo, FLAG_C);
    }

    reg_AF.hi = static_cast<BYTE>(reg);          
    

    reg_AF.lo = ResetBit(reg_AF.lo, FLAG_H);

    if(reg_AF.hi == 0) 
    {
        reg_AF.lo = SetBit(reg_AF.lo, FLAG_Z);
    }

    else
    {
        reg_AF.lo = ResetBit(reg_AF.lo, FLAG_Z);
    }     

}


//--------------------------------------------------------------------------------------------------------
//                                    Rotate Instructions Register
//--------------------------------------------------------------------------------------------------------

//Rotate Left
//MSB into carry flag
//MSB to LSB
void Emulator::CPU_RLC(BYTE &reg, bool isRegA)
{
    BYTE bef = reg;
    reg = reg << 1;

    //Move carry flag to the LSB
    if(TestBit(bef, 7) == 1)
    {
        reg = SetBit(reg, 0);
    }

    /*
    ----------------
        set flags
    ----------------
    */

    reg_AF.lo = 0;

    //Check the Carry flag before rotation happens
    if(TestBit(bef, 7) == 1)
    {
        reg_AF.lo = SetBit(reg_AF.lo, FLAG_C);
    }
    
    //Check Zero flag
    if(!isRegA)
    {
        if(reg == 0)
        {
            reg_AF.lo = SetBit(reg_AF.lo, FLAG_Z);
        }
    }
}

//Rotate Left through carry
//MSB into carry flag
//carry flag to LSB
void Emulator::CPU_RL(BYTE &reg, bool isRegA)
{
    BYTE bef = reg;
    reg = reg << 1;
    

    //Check the untouched reg and Move the MSB to the LSB
    if(TestBit(reg_AF.lo, FLAG_C) == 1)
    {
        reg = SetBit(reg, 0);
    }

    /*
    ----------------
        set flags
    ----------------
    */

    reg_AF.lo = 0;

    //Check the Carry flag
    if(TestBit(bef, 7) == 1)
    {
        reg_AF.lo = SetBit(reg_AF.lo, FLAG_C);
    }
    
    //Check Zero falg
    if(!isRegA)
    {
        if(reg == 0)
        {
            reg_AF.lo = SetBit(reg_AF.lo, FLAG_Z);
        }
    }
}

//Rotate Right
//LSB into carry flag
//LSB to MSB
void Emulator::CPU_RRC(BYTE &reg, bool isRegA)
{
    BYTE bef = reg;
    reg = reg >> 1;

    //Move carry flag to the MSB
    if(TestBit(bef, 0) == 1)
    {
        reg = SetBit(reg, 7);
    }

    /*
    ----------------
        set flags
    ----------------
    */

    reg_AF.lo = 0;

    //Check the Carry flag
    if(TestBit(bef, 0) == 1)
    {
        reg_AF.lo = SetBit(reg_AF.lo, FLAG_C);
    }
    
    //Check Zero flag
    if(!isRegA)
    {
        if(reg == 0)
        {
            reg_AF.lo = SetBit(reg_AF.lo, FLAG_Z);
        }
    }
}

//Rotate Right
//LSB into carry flag
//carry flag into MSB
void Emulator::CPU_RR(BYTE &reg, bool isRegA)
{
    BYTE bef = reg;
    reg = reg >> 1;
    

    //Check the untouched reg and Move the LSB to the MSB
    if(TestBit(reg_AF.lo, FLAG_C) == 1)
    {
        reg = SetBit(reg, 7);
    }

    /*
    ----------------
        set flags
    ----------------
    */

    reg_AF.lo = 0;

    //Check if Carry flag should be is set
    if(TestBit(bef, 0) == 1)
    {
        reg_AF.lo = SetBit(reg_AF.lo, FLAG_C);
    }
    
    //Check Zero falg
    if(!isRegA)
    {
        if(reg == 0)
        {
            reg_AF.lo = SetBit(reg_AF.lo, FLAG_Z);
        }    
    }
}   


//Rotate Left
//MSB into carry flag
//LSB set to 0
void Emulator::CPU_SLA(BYTE &reg)
{
    BYTE bef = reg;
    reg = reg << 1;

    /*
    ----------------
        set flags
    ----------------
    */

    reg_AF.lo = 0;

    //Check the Carry flag
    if(TestBit(bef, 7) == 1)
    {
        reg_AF.lo = SetBit(reg_AF.lo, FLAG_C);
    }
    
    //Check Zero flag
    if(reg == 0)
    {
        reg_AF.lo = SetBit(reg_AF.lo, FLAG_Z);
    }
}

//Rotate Right
//LSB into carry flag
//MSB Does not Change(Set to the 6th bit Basically)
void Emulator::CPU_SRA(BYTE &reg)
{
    BYTE bef = reg;
    reg = reg >> 1;
    

    //Check the untouched reg and get previous 7th bit
    if(BitGetVal(bef, 7) == 1)
    {
        reg = SetBit(reg, 7);
    }

    /*
    ----------------
        set flags
    ----------------
    */

    reg_AF.lo = 0;

    //Check if Carry flag is set
    if(TestBit(bef, 0) == 1)
    {
        reg_AF.lo = SetBit(reg_AF.lo, FLAG_C);
    }
    
    //Check Zero falg
    if(reg == 0)
    {
        reg_AF.lo = SetBit(reg_AF.lo, FLAG_Z);
    }    
}

//Rotate Right
//LSB into carry flag
//MSB Set to 0
void Emulator::CPU_SRL(BYTE &reg)
{
    BYTE bef = reg;
    reg = reg >> 1;

    /*
    ----------------
        set flags
    ----------------
    */

    reg_AF.lo = 0;

    //Check if Carry flag is set
    if(TestBit(bef, 0) == 1)
    {
        reg_AF.lo = SetBit(reg_AF.lo, FLAG_C);
    }
    
    //Check Zero falg
    if(reg == 0)
    {
        reg_AF.lo = SetBit(reg_AF.lo, FLAG_Z);
    }    
}


//--------------------------------------------------------------------------------------------------------
//                                    Rotate Instructions Memory
//--------------------------------------------------------------------------------------------------------

//Rotate Left the data stored in regWord
//MSB into carry flag
//Carry flag to LSB
//Write Back to address in regWord
void Emulator::CPU_RL_MEMORY(WORD addrInReg)
{
    BYTE data = readByte(addrInReg);
    BYTE bef = data;
    data = data << 1;
    
    //Check carry flag
    if(TestBit(reg_AF.lo, 4) == 1)
    {
        data = SetBit(data, 0);
    }

    writeByte(addrInReg, data);

    /*
    ----------------
        set flags
    ----------------
    */

    reg_AF.lo = 0;

    //Check the Carry flag
    if(TestBit(bef, 7) == 1)
    {
        reg_AF.lo = SetBit(reg_AF.lo, FLAG_C);
    }
    
    //Check Zero falg
    if(data == 0)
    {
        reg_AF.lo = SetBit(reg_AF.lo, FLAG_Z);
    }
}

//Rotate Left the data stored in addrInReg
//MSB into carry flag
//MSB flag to LSB
//Write Back to address in addrInReg
void Emulator::CPU_RLC_MEMORY(WORD addrInReg)
{
    BYTE data = readByte(addrInReg);
    BYTE bef = data;
    data = data << 1;
    

    //Check the untouched data and Move the MSB to the LSB
    if(BitGetVal(bef, 7) == 1)
    {
        data = SetBit(data, 0);
    }

    writeByte(addrInReg, data);

    /*
    ----------------
        set flags
    ----------------
    */

    reg_AF.lo = 0;

    //Check the Carry flag
    if(TestBit(bef, 7) == 1)
    {
        reg_AF.lo = SetBit(reg_AF.lo, FLAG_C);
    }
    
    //Check Zero flag
    if(data == 0)
    {
        reg_AF.lo = SetBit(reg_AF.lo, FLAG_Z);
    }    
}

//Rotate Right the data stored in addrInReg
//LSB into carry flag
//Carry flag to MSB
//Write Back to address in addrInReg
void Emulator::CPU_RR_MEMORY(WORD addrInReg)
{
    BYTE data = readByte(addrInReg);
    BYTE bef = data;
    data = data >> 1;

    //Move carry flag to the MSB
    if(TestBit(reg_AF.lo, 4) == 1)
    {
        data = SetBit(data, 7);
    }

    writeByte(addrInReg, data);

    /*
    ----------------
        set flags
    ----------------
    */

    reg_AF.lo = 0;

    //Check the Carry flag
    if(TestBit(bef, 0) == 1)
    {
        reg_AF.lo = SetBit(reg_AF.lo, FLAG_C);
    }
    
    //Check Zero flag
    if(data == 0)
    {
        reg_AF.lo = SetBit(reg_AF.lo, FLAG_Z);
    }
}

//Rotate Right the data stored in addrInReg
//LSB into carry flag
//LSB to MSB
//Write Back to address in addrInReg
void Emulator::CPU_RRC_MEMORY(WORD addrInReg)
{
    BYTE data = readByte(addrInReg);
    BYTE bef = data;
    data = data >> 1;
    

    //Check the untouched reg and Move the LSB to the MSB
    if(BitGetVal(bef, 0) == 1)
    {
        data = SetBit(data, 7);
    }

    writeByte(addrInReg, data);

    /*
    ----------------
        set flags
    ----------------
    */

    reg_AF.lo = 0;

    //Check if Carry flag is set
    if(TestBit(bef, 0) == 1)
    {
        reg_AF.lo = SetBit(reg_AF.lo, FLAG_C);
    }
    
    //Check Zero flag
    if(data == 0)
    {
        reg_AF.lo = SetBit(reg_AF.lo, FLAG_Z);
    }
}

//Rotate Left the data stored in addrInReg
//MSB into carry flag
//LSB set to 0
//Write Back to address in addrInReg
void Emulator::CPU_SLA_MEMORY(WORD addrInReg)
{
    BYTE data = readByte(addrInReg);
    BYTE bef = data;
    data = data << 1;

    writeByte(addrInReg, data);

    /*
    ----------------
        set flags
    ----------------
    */

    reg_AF.lo = 0;

    //Check the Carry flag
    if(TestBit(bef, 7) == 1)
    {
        reg_AF.lo = SetBit(reg_AF.lo, FLAG_C);
    }
    
    //Check Zero flag
    if(data == 0)
    {
        reg_AF.lo = SetBit(reg_AF.lo, FLAG_Z);
    }
}

//Rotate Right the data stored in addrInReg
//LSB into carry flag
//MSB does not change(Set to 7th bit before rotation occurs)
//Write Back to address in addrInReg
void Emulator::CPU_SRA_MEMORY(WORD addrInReg)
{
    BYTE data = readByte(addrInReg);
    BYTE bef = data;
    data = data >> 1;
    

    //Check the untouched reg and set MSB to the 7th bit of the previous value
    if(BitGetVal(bef, 7) == 1)
    {
        data = SetBit(data, 7);
    }

    writeByte(addrInReg, data);

    /*
    ----------------
        set flags
    ----------------
    */

    reg_AF.lo = 0;

    //Check if Carry flag is set
    if(TestBit(bef, 0) == 1)
    {
        reg_AF.lo = SetBit(reg_AF.lo, FLAG_C);
    }
    
    //Check Zero flag
    if(data == 0)
    {
        reg_AF.lo = SetBit(reg_AF.lo, FLAG_Z);
    }
}

//Rotate Right the data stored in addrInReg
//LSB into carry flag
//MSB set to 0
//Write Back to address in addrInReg
void Emulator::CPU_SRL_MEMORY(WORD addrInReg)
{
    BYTE data = readByte(addrInReg);
    BYTE bef = data;
    data = data >> 1;

    writeByte(addrInReg, data);

    /*
    ----------------
        set flags
    ----------------
    */

    reg_AF.lo = 0;

    //Check if Carry flag is set
    if(TestBit(bef, 0) == 1)
    {
        reg_AF.lo = SetBit(reg_AF.lo, FLAG_C);
    }
    
    //Check Zero falg
    if(data == 0)
    {
        reg_AF.lo = SetBit(reg_AF.lo, FLAG_Z);
    }
}

//--------------------------------------------------------------------------------------------------------
//                                    Swap Nibbles
//--------------------------------------------------------------------------------------------------------

void Emulator::CPU_SWAP_NIBBLES(BYTE &reg)
{
    BYTE upperNibble = (reg & 0xF0) >> 4;
    BYTE lowerNibble = (reg & 0xF) << 4;

    reg = lowerNibble | upperNibble;

    /*
    ----------------
        set flags
    ----------------
    */

   reg_AF.lo = 0;

   if(reg == 0)
   {
       reg_AF.lo = SetBit(reg_AF.lo, FLAG_Z);
   }
}

void Emulator::CPU_SWAP_NIBBLES_MEMORY(WORD addrInReg)
{
    BYTE data = readByte(addrInReg);
    BYTE upperNibble = (data & 0xF0) >> 4;
    BYTE lowerNibble = (data & 0xF) << 4;

    data = lowerNibble | upperNibble;

    writeByte(addrInReg, data);

    /*
    ----------------
        set flags
    ----------------
    */

   reg_AF.lo = 0;

   if(data == 0)
   {
       reg_AF.lo = SetBit(reg_AF.lo, FLAG_Z);
   }
}


//--------------------------------------------------------------------------------------------------------
//                                    Check Bit Functions
//--------------------------------------------------------------------------------------------------------

void Emulator::CPU_TEST_BIT(BYTE reg, BYTE bitToTest)
{
    /*
    ----------------
        set flags
    ----------------
    */

    if(TestBit(reg, bitToTest) == 0)
    {
        reg_AF.lo = SetBit(reg_AF.lo, FLAG_Z);
    }    

    else
    {
        reg_AF.lo = ResetBit(reg_AF.lo, FLAG_Z);
    }

    reg_AF.lo = ResetBit(reg_AF.lo, FLAG_N);
    reg_AF.lo = SetBit(reg_AF.lo, FLAG_H);
}

void Emulator::CPU_RESET_BIT(BYTE &reg, BYTE bitToReset)
{
    reg = ResetBit(reg, bitToReset);
}

void Emulator::CPU_RESET_BIT_MEMORY(WORD addrInReg, BYTE bitToReset)
{
    BYTE data = readByte(addrInReg);
    data = ResetBit(data, bitToReset);
    writeByte(addrInReg, data);
}

void Emulator::CPU_SET_BIT(BYTE &reg, BYTE bitToSet)
{
    reg = SetBit(reg, bitToSet);
}

void Emulator::CPU_SET_BIT_MEMORY(WORD addrInReg, BYTE bitToSet)
{
    BYTE data = readByte(addrInReg);
    data = SetBit(data, bitToSet);
    writeByte(addrInReg, data);
}