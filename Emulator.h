#pragma once
#ifndef EMULATOR_H
#define EMULATOR_H

//Libraries
#include <iostream>
#include <fstream>
#include <cstring>

//Definitions
#define MaxCartridgeMemory 0x200000
#define MaxInternalMemory 0x10000 //Equivalent to a word
#define MaxExternalRAM    0x8000
#define x_ScreenResolution 160
#define y_ScreenResolution 144

//Flags
#define FLAG_Z 7
#define FLAG_N 6
#define FLAG_H 5
#define FLAG_C 4
//The values represents the bit numbers. It does not start from 0 because of endianess

//Timer Components & their addresses
#define DividerTimerAddr 0xFF04 //Divider Timer
#define TIMA 0xFF05 //Timer
#define TMA 0xFF06  //Timer Modulator, the value here is used when timer overflows
#define TMC 0xFF07  //Timer Controller, Holds a bit value on how much to increment the timer per second. e.g.(4096Hz) For every 1 second, increment the timer 4096 times

//Interrupt Components & their addresses
#define IERAddr 0xFFFF //Interrupt Enabled Register: Used to check if  specific interrupt can be service if the bit is enabled.
#define IRRAddr 0xFF0F //Interrupt Request Register: Used to signal that a specific interrupt is currently being serviced. If the bit is enabled, an interrupt is currently being serviced

//LCD Components & their addresses 
#define LCDCtrlRegAddr 0xFF40
#define LCDStatusAddr 0xFF41 //Stores the status of the LCD & interrupt bits for specific LCD functions
//These interrupt bit is meant only for LCD interrupts and unlike the other interrupts it is not set by the emulator, it is set by the game
//More in the source code
#define scanlineAddr 0xFF44 //Current Scanline number. This address stores the what scanline is currently being written(0-153)
//NOT to be confused with scanlineCounter which tracks WHEN to move on to the next scanline
#define scanlineSpclAddr 0xFF45 //Do something(special effect or smth) when 0xFF45 is equal to 0xFF44, Then coincidence flag is set 

//Graphics Components & their addresses 
#define viewingAreaXPosAddr 0xFF42 //Y Position of the BACKGROUND to start drawing the viewing area from
#define viewingAreaYPosAddr 0xFF42 //X Position of the BACKGROUND to start drawing the viewing area from
#define windowXPosAddr 0xFF4A      //The Y Position of the VIEWING AREA to start drawing the window from
#define windowYPosAddr 0xFF4B     //The X Position - 7 of the VIEWING AREA to start drawing the window from
#define BGPalettesAddr 0xFF47      //BG_Color_Palettes
#define SpritePallete1Addr 0xFF48  //SPRITE_Color_Palettes_1
#define SpritePallete2Addr 0xFF49  //SPRITE_Color_Palettes_2
#define SpriteAttributeAddr 0xFE00 //Sprite Attributes

//Joypad Components & their addresses
#define joypadAddr 0xFF00          //Joypad 


//DMA Components & their addresses
#define DMAAddr 0xFF46



//CPU clock speed
#define CPUCLOCKSPEED 4194304

typedef unsigned char BYTE;         //0 to 256
typedef char SIGNED_BYTE;           //-128 to 127
typedef unsigned short WORD;        //-32768 to 32767
typedef signed short SIGNED_WORD;   //0 to 65536

//Memory Mapping
/*
0000-3FFF 16KB ROM Bank 00 (in cartridge, fixed at bank 00)             - ROM in the Cartridge. Contains initial 16KB ROM
4000-7FFF 16KB ROM Bank 01..NN (in cartridge, switchable bank number)   - Switchable ROM banks in the Cartridge in case a ROM bank is full
8000-9FFF 8KB Video RAM (VRAM) (switchable bank 0-1 in CGB Mode)        - RAM specifically for storing graphical data and tiles 
A000-BFFF 8KB External RAM (in cartridge, switchable bank, if any)      - RAM in Cartridge, Non-Volatile. One RAM Bank is about roughly 2KB
C000-CFFF 4KB Work RAM Bank 0 (WRAM)                                    - RAM in the emulator, Volatile
D000-DFFF 4KB Work RAM Bank 1 (WRAM) (switchable bank 1-7 in CGB Mode)  - Contains switchable RAM banks in case a RAM bank is full
E000-FDFF Same as C000-DDFF (ECHO) (typically not used)                 - 
FE00-FE9F Sprite Attribute Table (OAM)                                  - Used for sprites 
FEA0-FEFF Not Usable                                                    - 
FF00-FF7F I/O Ports                                                     - Used for communication with input devices and other hardware
FFFF Interrupt Enable Register                                          -
FF80-FFFE High RAM (HRAM)                                               - Used for storing hardware 
*/

/*
Interrupt Mapping
*/

class Emulator
{
public:   
    //Load game
    bool loadCartridge(std::string GameName);
   
private:

    enum COLOR {WHITE, LIGHT_GRAY, DARK_GRAY, BLACK};
    //Register
    union Register
    {
        WORD reg;
        struct 
        {
            BYTE lo; //control Register bits 0 to 7; Register A,B,D,H from bits 0 to 7
            BYTE hi; //control Register btis 8 to 15 ; Register F,C,E,L from bits 8 to 15
        };
        //It is lo first then hi because of endianness
    };

    Register reg_AF; //Not used often because it contains A(accumulator) & F(flag register)
    Register reg_BC;
    Register reg_DE;
    Register reg_HL;
    Register stack_Pointer;

    BYTE Cartridge_Memory[MaxCartridgeMemory]; //Maximum capacity of the Cartridge.
    BYTE screen_Display[x_ScreenResolution][y_ScreenResolution][3]; //[x-axis][y-axis][RGB color]
    BYTE internal_Memory[MaxInternalMemory];   //Maximmum capacity of internal memory of the emulator
    //To set middle of screent to color red which is 0xFF00000 
    /*
    screenDisplay[x_screenResolution/2][y_screenResolution/2][0] = 0xFF;
    screenDisplay[x_screenResolution/2][y_screenResolution/2][1] = 0;
    screenDisplay[x_screenResolution/2][y_screenResolution/2][2] = 0;
    */

    WORD program_Counter;

    //Set whether or not a switchable bank is used or not. In this case, We only need 2
    //Switchable banks are used in case the game file is too big for the current ROM (0x8000)
    bool MBC1_set;
    bool MBC2_set;

    BYTE current_ROM_BANK; //Current ROM banking used
    BYTE external_RAM_BANK_Memory[MaxExternalRAM];
    BYTE current_EXT_RAM_BANK; //Current RAM banking used

    bool enableRAMBanking;
    bool enableROMBanking;

    //Used to update the timer in memory 0xFF05. Increments down and will only update timer every x clock cycles. 
    //Timer in 0xFF05 (I believe) is used to have interrupts at given interval
    int timerCounter;
    //Used to update divider timer in memory 0xFF04. Increments up and will only update timer every 256 clock cycles.  
    //Divider timer in 0xFF04 (I believe) is used to I have no idea
    int dividerCounter; 
    //The 2 counters specified above is not used for synchronizing graphics
    int frequency;

    //Used to track scanline. Takes 456 clock cycles to draw one scanline and move on to the next
    int scanlineCounter;

    //This returns a BYTE of keys associated with each key
    /*
        SDLK_a : key = 4
        SDLK_s : key = 5
        SDLK_RETURN : key = 7
        SDLK_SPACE : key = 6
        SDLK_RIGHT : key = 0
        SDLK_LEFT : key = 1
        SDLK_UP : key = 2
        SDLK_DOWN : key = 3 
    */
    BYTE joypadKeyState;

    bool masterInterrupt;
    

    /*
    ------------------------
    -------FUNCTIONS--------
    ------------------------
    */
    
    //General Function
    void PushWordToStack(WORD dataToPush);
    bool restartCPU();
    bool writeMemory(WORD address, BYTE data);
    BYTE readMemory(WORD address) const; //Never modify memory hence, const. Safety check

    //Handle RAMROM Bank Changes
    void set_ROM_BANKING_CONTROLLER();
    void HandleBanking(WORD address, BYTE data);
    void DoRamBankEnable(WORD address, BYTE data);
    void changeLoROMBank(BYTE data);
    void changeHiROMBank(BYTE data);
    void changeRAMBank(BYTE data);
    void changeROMRAMMode(BYTE data);

    //Handle DMA
    void DMATransfer(BYTE data);

    //Handle Timing 
    void updateTimers(int cycles);
    bool isClockEnabled() const;
    BYTE getClockFreq() const;
    void setClockFreq();
    void divideRegisterCycle(int cycles);

    //Handle Interrupt
    void RequestInterrupt(int id);                 //Request for an interrupt to happen by setting IER
    void DoInterrupts();                           //Polls every x seconds if there is an interrupt
    void ServiceInterrupt(BYTE interruptBit);      //Service the interrupt

    //Handle LCD
    void updateGraphics(int cycles);
    void setLCDStatus();
    bool IsLCDEnabled() const;
  
    //Handle Graphics & bit of LCD(Specifically LCD control register)
    void drawScanline();
    void renderTiles();
    void renderSprites();
    COLOR GetColor(int colorNum, WORD pallete_Address) const;

    //Handle Joypad
    void keyPressed(int key);
    void keyReleased(int key);
    BYTE GetJoypadState() const;

    //Handle Opcodes
    int ExecuteNextOpcode();
    int ExecuteOpcode(BYTE opcode);
    
    //Synchronize
    void Update();
};

#endif