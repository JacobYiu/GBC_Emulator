#include "Emulator.h"
#include "utils.h"



void Emulator::PushWordToStack(WORD dataToPush)
{
    //Just to make sure you are not overwriting any data that is not part of the stack
    if(stack_Pointer.reg > 0xFF80 && stack_Pointer.reg <= 0xFFFE)
    {
        internal_Memory[stack_Pointer.reg] = dataToPush;
        stack_Pointer.reg = stack_Pointer.reg - 1; //Decrement stack pointer register
    }
}

bool Emulator::restartCPU()
{
    program_Counter = 0x100;
    reg_AF.reg = 0x01B0;
    reg_BC.reg =0x0013;
    reg_DE.reg =0x00D8;
    reg_HL.reg =0x014D;
    stack_Pointer.reg = 0xFFFE;
    internal_Memory[0xFF05] = 0x00 ;
    internal_Memory[0xFF06] = 0x00 ;
    internal_Memory[0xFF07] = 0x00 ;
    internal_Memory[0xFF10] = 0x80 ;
    internal_Memory[0xFF11] = 0xBF ;
    internal_Memory[0xFF12] = 0xF3 ;
    internal_Memory[0xFF14] = 0xBF ;
    internal_Memory[0xFF16] = 0x3F ;
    internal_Memory[0xFF17] = 0x00 ;
    internal_Memory[0xFF19] = 0xBF ;
    internal_Memory[0xFF1A] = 0x7F ;
    internal_Memory[0xFF1B] = 0xFF ;
    internal_Memory[0xFF1C] = 0x9F ;
    internal_Memory[0xFF1E] = 0xBF ;
    internal_Memory[0xFF20] = 0xFF ;
    internal_Memory[0xFF21] = 0x00 ;
    internal_Memory[0xFF22] = 0x00 ;
    internal_Memory[0xFF23] = 0xBF ;
    internal_Memory[0xFF24] = 0x77 ;
    internal_Memory[0xFF25] = 0xF3 ;
    internal_Memory[0xFF26] = 0xF1 ;
    internal_Memory[0xFF40] = 0x91 ;
    internal_Memory[0xFF42] = 0x00 ;
    internal_Memory[0xFF43] = 0x00 ;
    internal_Memory[0xFF45] = 0x00 ;
    internal_Memory[0xFF47] = 0xFC ;
    internal_Memory[0xFF48] = 0xFF ;
    internal_Memory[0xFF49] = 0xFF ;
    internal_Memory[0xFF4A] = 0x00 ;
    internal_Memory[0xFF4B] = 0x00 ;
    internal_Memory[0xFFFF] = 0x00 ; 

    current_ROM_BANK = 1; //Need to have a default ROM bank for address 4000-7FFF; 0 is already used by address 0 - 3FFF
    current_EXT_RAM_BANK = 0; //Need to have a default RAM bank for address A000-BFFF
    memset(external_RAM_BANK_Memory, 0, sizeof(external_RAM_BANK_Memory)); //Set external RAM bank to 0 

    enableRAMBanking = false;
    timerCounter = 1024; //Clockspeed/frequency(TMC). Completes 1024 clock cycles in a second 

    masterInterrupt = true;

    //Set all joypads to true. Active low. 
    BYTE joypadKeyState = 0xFF;

    scanlineCounter = 456; //Default starting value for scanline Counter
}

bool Emulator::loadCartridge(std::string GameName)
{
    memset(Cartridge_Memory, 0, sizeof(Cartridge_Memory)); //Set all arrays in Cartridge memory to 0
    std::ifstream inFile(GameName, std::ios::binary);
    if(!inFile.is_open())
    {
        std::cerr << "Failed to read game file." << std::endl;
        return false;
    }

    inFile.read(reinterpret_cast<char*>(Cartridge_Memory), MaxCartridgeMemory); //Read from Cartridge into Console Cartrdige memory.

    inFile.close();

    return true;
}

//Writing to memory
bool Emulator::writeMemory(WORD address, BYTE data)
{
    //Memory specifically for the cartridge. You cannot write to it, Only read 
    if(address <= 0x8000)
    {
        HandleBanking(address, data);
    }

    else if((address >= 0xA000) && (address < 0xC0000))
    {
        if(enableRAMBanking)
        {
            WORD newAddress = address - 0xA000;
            external_RAM_BANK_Memory[newAddress + (current_EXT_RAM_BANK * 0x2000)] = data;
        }
    }

    //Writing memory in echo = Writing memory in RAM
    else if(address >= 0xE000 && address <= 0xFDFF)
    {
        internal_Memory[address] = data;
        writeMemory((address - 0x2000), data);
    }

    //Not usable memory address
    else if(address >= 0xFEA0 && address <= 0xFEFF)
    {
        return false;
    }

    else if(address == TMC)
    {
        BYTE currentfreq = getClockFreq();
        Cartridge_Memory[TMC] = data;
        BYTE newfreq = getClockFreq();

        if(currentfreq != newfreq)
        {
            setClockFreq();
        }
    }

    //Check for divider timer
    else if(address == DividerTimerAddr)
    {
        Cartridge_Memory[0xFF04] = 0;
    }

    else if(address == scanlineAddr)
    {
        Cartridge_Memory[0xFF44] = 0;
    }

    else if(address == DMAAddr)
    {
        DMATransfer(data);
    }

    else
    {
        internal_Memory[address] = data;
    }
}

BYTE Emulator::readMemory(WORD address) const
{
    //Read from switchable ROM banks
    //Access current ROM bank in Cartridge memory 
    if(address >= 0x4000 && address <= 0x7FFF)
    {
        WORD newAddress = address - 0x4000;
        return Cartridge_Memory[newAddress + (current_ROM_BANK * 4000)];  
    }

    //Read from switchable RAM banks
    //Access current RAM banks in external_RAM_Bank 
    else if(address >= 0xA000 && address <= 0xBFFF)
    {
        WORD newAddress = address - 0x2000;
        return external_RAM_BANK_Memory[newAddress + (current_EXT_RAM_BANK * 2000)];
    }

    else if(address == joypadAddr)
    {
        return GetJoypadState();
    }

    else
    {
        return internal_Memory[address];
    }
}

//--------------------------------------------------------------------------------------------------------
//Start of ROMRAM Banking functions
//--------------------------------------------------------------------------------------------------------
void Emulator::set_ROM_BANKING_CONTROLLER()
{
    MBC1_set = false;
    MBC2_set = false;

    //If both are false, then the game is small enough to not use any ROM banking.
    //Different MBC control different ROM bankings
    //Not sure which ROM bankings are assigned though because of lacks of docs regarding this data
    switch(Cartridge_Memory[0x147])
    {
        case 1: MBC1_set = true; break;
        case 2: MBC1_set = true; break;
        case 3: MBC1_set = true; break;
        case 5: MBC2_set = true; break;
        case 6: MBC2_set = true; break;
        default: break;
    }
}

void Emulator::HandleBanking(WORD address, BYTE data)
{   
    //RAM enabling
    if(address < 0x2000)
    {
        if(MBC1_set || MBC2_set)
        {
            DoRamBankEnable(address, data);
        }
    }

    //Change ROM bank
    else if((address >= 0x2000) & (address < 0x4000))
    {
        if(MBC1_set || MBC2_set)
        {
            changeLoROMBank(data);
        }
    }

    //Change ROM or RAM bank
    else if((address >= 0x4000) & (address < 0x6000))
    {
        //There is no RAM banking when MBC2 is set, so set ram banking to 0 and by default chooses ROM banking
        if(MBC1_set)
        {
            if(enableROMBanking)
            {
                changeHiROMBank(data);
            }
            
            else
            {
                changeRAMBank(data);
            }
        }
    }

    //This conditional statement will change whether we are dealing with ROM banking or RAM banking
    else if((address >= 0x6000) & (address < 0x8000))
    {
        if(MBC1_set)
        {
            changeROMRAMMode(data);
        }
    }

    else
    {
        return;                         //In case there is a bug in the code, this code will execute outside of HandleBanking properly 
    }

}

// Used to enable ram banking
void Emulator::DoRamBankEnable(WORD address, BYTE data)
{
    if(MBC2_set)                        //If MBC2 and 4th bit is set, then return
    {
        if(TestBit(data, 4))
        {
            return;
        }
    }

    BYTE lowerNibble = data & 0x0F;     //Check the lowerNibble and check if its equal to 0xA

    if(lowerNibble == 0xA)
    {
        enableRAMBanking = true;        //If equal, able to write to RAM
    } 
    else
    {
        enableRAMBanking = false;       //If not equal, unable to write to RAM 
    }

}

//Used to change ROM banks when writing to memory address 0x2000 - 0x3FFF
void Emulator::changeLoROMBank(BYTE data)
{   
    if(MBC2_set)
    {
        current_ROM_BANK = data & 0x0F;
        if(current_ROM_BANK == 0)       //Safety check beacuse ROM 0 is already in use
        {
            current_ROM_BANK ++;
        }
        return;
    }

    BYTE lower5 = data & 0x1F; //AND bits 0-4
    current_ROM_BANK &= 0xE0;   //AND bits 5-7 
    current_ROM_BANK |= lower5;//OR bits to maintain the first 3 bits but make updates in the lower 5 bits 
    if(current_ROM_BANK == 0)
    {
        current_ROM_BANK ++;
    }

    return;

}

//Used to change ROM banks when writing to memory address 0x4000 - 0x6FFF
void Emulator::changeHiROMBank(BYTE data)
{
    BYTE upper3 = data & 0xE0; //AND bits 5-7
    current_ROM_BANK &= 0x1F;  //AND bits 0-4
    current_ROM_BANK |= upper3;//OR bits to maintain the lower 5 bits but make updates in the upper 3 bits
    if(current_ROM_BANK == 0)
    {
        current_ROM_BANK ++;
    }

    return;
}

void Emulator::changeRAMBank(BYTE data)
{
    current_EXT_RAM_BANK = data & 0x3;
}

void Emulator::changeROMRAMMode(BYTE data)
{
    //Checks bit 0. If bit 0 is set then ROM banking is disabled
    BYTE newData = data & 0b1;
    enableROMBanking = (newData == 0)?true:false;
    if(enableROMBanking)
    {
        current_EXT_RAM_BANK = 0; //If ROM banking is set, very important to set RAM banking to 0
    }
}

//--------------------------------------------------------------------------------------------------------
//Start of timer functions
/*
Memory Mapping of the timers:
    0xFF05: Timer
    0xFF06: The Timer Modulator 
    0xFF07: The Timer Controller
*/

//However since there is only 8 bits in the timer controller, then we need to adapt by using these bits for varying frequencies:
/*
00: 4096 Hz
01: 262144 Hz
10: 65536 Hz
11: 16384 Hz 
*/ 
//--------------------------------------------------------------------------------------------------------
void Emulator::updateTimers(int cycles)
{
    divideRegisterCycle(cycles);

    //Clock must be enabled to update clock
    if(isClockEnabled())
    {
        timerCounter -= cycles;

        //enough cpu clock cycles have happened to update the timer
        if(timerCounter <= 0)
        {
            //Reset timer tracer to the appropriate value
            setClockFreq();

            if(readMemory(TIMA) == 255)
            {
                writeMemory(TIMA, readMemory(TMA));
                RequestInterrupt(2);
            }

            else
            {
                writeMemory(TIMA, readMemory(TIMA) + 1);
            }
        }
    }
}

bool Emulator::isClockEnabled() const
{
    //Check bit 2 of TMC to see if clock is enabled
    if(TestBit(readMemory(TMC), 2))
    {
        return true;
    }

    return false;
}

//Used to get clock frequency
BYTE Emulator::getClockFreq() const
{
    //Only get bits 0 - 2 
    return readMemory(TMC) & 0x3;
}

//Used to set clock frequency. SHOULD only be set by the hardware and not by code
void Emulator::setClockFreq()
{
    BYTE freq = getClockFreq();
    switch(freq)
    {
        case 0: timerCounter = 1024;    //freq = 4086
        case 1: timerCounter = 16;      //freq = 261244
        case 2: timerCounter = 64;      //freq = 65536
        case 3: timerCounter = 256;     //freq = 16382
    }
}


void Emulator::divideRegisterCycle(int cycles)
{
    dividerCounter += cycles;
    if(dividerCounter >= 255)
    {
        dividerCounter = 0;
        Cartridge_Memory[0xFF04] ++; //the emulator does not allow writing directly to memory 
    }
}

//--------------------------------------------------------------------------------------------------------
//Start of Interrupt functions
/*
Memory Mapping of the interrupts:
    0xFFFF: The interrupt enabled register (IER)
    0xFF0F: The interrupt request register (IRR)

In Game interrupts:
This master register is used to enable or disable interrupts,If this value is off, then no interrupts can be serviced`
*/

/*
types of interrupts:
    bit 0: V-Blank
    bit 1: LCD
    bit 2: Timer
    bit 4: Joypad
*/

//--------------------------------------------------------------------------------------------------------
//Used to enable a specific bit in interrupt request register
void Emulator::RequestInterrupt(int id)
{
    BYTE req = readMemory(0xFF05);
    BYTE bitToSet = (0b1 << id);
    req = req | bitToSet;
    writeMemory(0xFF05, req);
}

void Emulator::DoInterrupts()
{
    if(masterInterrupt)
    {
        BYTE IER = readMemory(IERAddr);
        BYTE IRR = readMemory(IRRAddr);
        for(int bitNum = 0; bitNum < 5; bitNum++)
        {
            if(bitNum == 3) //Skip bit 3 because no interrupts are set in bit 3
            {
                continue;
            }

            if(TestBit(IRR, bitNum))//If a particular bit is set, then we are trying to service that interrupt corresponding to that bit
            {
                if(TestBit(IER, bitNum)) //If a particular bit is set, then we are allowed to service that interrupt corresponding to that bit
                {
                    ServiceInterrupt(bitNum);
                }
            } 
        }
    }    
}

void Emulator::ServiceInterrupt(BYTE interruptBit)
{
    //Turn off master interrupt so that no other interrupt can occur 
    masterInterrupt = false;

    //Turn off the bit corresponding to the interrupt because it is going to be serviced 
    BYTE data = readMemory(0xFF0F);
    data = ResetBit(data, interruptBit);
    writeMemory(0xFF0F, data);

    PushWordToStack(program_Counter);

    switch (interruptBit)
    {
        case 0: program_Counter = 0x40; break;
        case 1: program_Counter = 0x48; break;
        case 2: program_Counter = 0x50; break;
        case 4: program_Counter = 0x60; break;
    }
}

//--------------------------------------------------------------------------------------------------------
//Start of LCD Function

/*
Memory Mapping of LCD:
    0xFF40: LCD control register
    0xFF41: LCD status register
    0xFF44: scanLine value
    
*/

/*
0xFF41(LCD Status & Interrupt Bits for LCD)
    Bit (0-1):
        00: H-Blank                         (Mode 0)
        01: V-Blank                         (Mode 1) //Default mode when lcd is disabled
        10: Searching Sprites Atts          (Mode 2)
        11: Transfering Data to LCD Driver  (Mode 3)

    Bit 2: Coincidence Flag
    Bit 3: Mode 0 Interrupt               
    Bit 4: Mode 1 Interrupt                
    Bit 5: Mode 2 Interrupt
    Note that you will see that V-Blank interupts occur both in LCD and the emulators V-Blank emulator, They are different

    111(Bit 6): Coincidence Flag 

    If either bits 3,4,5 is enabled, then LCD interrupt is enabled
    
*/

/*
The order of rendering a single scanline is as follows: 
As mentioned a single rendering of a scanline takes 456 cpu clock cycles.
    1. Mode 2(80 clock cycles, 456 - 376)
    2. Mode 3(172 clock cycles, 375 - 203)
    3. Mode 0(203 clock cycles, 203 - 0)
    Process is repeated until scanline number reaches 144 
    Scanline number 145 - 153, mode is set to Mode 1(V-Blank specifically in LCD)
 
*/
//--------------------------------------------------------------------------------------------------------
void Emulator::updateGraphics(int cycles)
{
    setLCDStatus();

    if(IsLCDEnabled())
    {
        scanlineCounter -= cycles;
    }

    else
    {
        return;
    }
    
    //if scanlineCounter <= 0, time to move on to the next scanline
    if(scanlineCounter <= 0)
    {
        Cartridge_Memory[scanlineAddr]++;
        int currentline = readMemory(scanlineAddr);
        scanlineCounter = 456;
        //if currentScanline is 144 then its time to turn on V-Blank Interrupts
        if(currentline == 144)
        {
            RequestInterrupt(0);
        }

        //If currentScanline is 153, then its time to move on to the next frame. 
        //This new frame will start from scanline 0 
        else if(currentline > 153)
        {
            Cartridge_Memory[scanlineAddr] = 0; //Cannot use writeMemory it resets scanline to 0 
        }

        else if(currentline < 144)
        {
            drawScanline();
        }
        
    }
}

//Independent Variables: scanlineCounter, LCD coincidence interrupt(0xFF41, bit 6), Mode Changes
//Dependent Variable: LCD status register (0xFF41, bits 0-5) 
//There are 2 types of interrupt happening: Mode change & Doing special effect depending on the scanline number check 
void Emulator::setLCDStatus()
{
    BYTE lcdStatus = readMemory(LCDStatusAddr);
    //if LCD is disabled
    if(IsLCDEnabled() == false)
    {
        //Set scalineCounter to 456, scanlineNumber to 0, lcdStatus to mode 1(0bxxxxxx01)
        scanlineCounter = 456;
        Cartridge_Memory[scanlineAddr] = 0;
        lcdStatus &= 252; 
        lcdStatus = SetBit(lcdStatus, 0);
        writeMemory(LCDStatusAddr, lcdStatus);
    }
    
    //Checking for mode change ---------------------------------------------

    BYTE currentscanline = readMemory(scanlineAddr);
    //Get the zeroth and first bit to get the mode only
    BYTE currentmode = readMemory(lcdStatus) & 0x3;
    BYTE mode = 0; //Set default mode to 0
    int mode2Bounds = 456 - 80;
    int mode3Bounds = mode2Bounds - 172;  

    bool LCDIRQ = false;

    //If scanline is greater than 144, then we know for sure that it is mode 1, V-Blank
    if(currentscanline >= 144 )
    {
        mode = 1;
        lcdStatus = SetBit(lcdStatus, 0);
        lcdStatus = ResetBit(lcdStatus, 1);
        LCDIRQ = TestBit(lcdStatus, 4);
    }

    else
    {
        //Searching Sprites, mode 2 (0b10)
        if(scanlineCounter >= mode2Bounds)
        {
            mode = 2;
            lcdStatus = ResetBit(lcdStatus, 0);
            lcdStatus = SetBit(lcdStatus, 1);
            LCDIRQ = TestBit(lcdStatus, 5);
        }

        //Transferring Data to Driver, mode 3 (0b11)
        //Does not need interrupt for this 
        else if(scanlineCounter >= mode3Bounds && scanlineCounter < mode2Bounds)
        {
            mode = 3;
            lcdStatus = SetBit(lcdStatus, 0);
            lcdStatus = SetBit(lcdStatus, 1);
        }

        //H-Blank, mode 0 (0b00)
        else
        {
            mode = 0;
            lcdStatus = ResetBit(lcdStatus, 0);
            lcdStatus = ResetBit(lcdStatus, 1);
            LCDIRQ = TestBit(lcdStatus, 3);
        }
    }

    if(LCDIRQ && currentmode != mode)
    {
        RequestInterrupt(1);
    }


    //Doing Special Effect on the Scanline ---------------------------------------------
    BYTE currentScanline = readMemory(scanlineAddr);
    BYTE SpclEffScanline = readMemory(scanlineSpclAddr);
    if(currentScanline == SpclEffScanline)
    {
        //Used to set coincidence flag
        lcdStatus = SetBit(lcdStatus, 2);
        if(TestBit(lcdStatus, 6))
        {
            //Used to Enable LCD Interrupt if coincidence flag and coincidence interrupt flag is set
            RequestInterrupt(1);
        }
    }

    else
    {
        //Reset bit 2 of lcdStatus
        lcdStatus = ResetBit(lcdStatus, 2);
    }

    writeMemory(LCDStatusAddr, lcdStatus);
}

bool Emulator::IsLCDEnabled() const
{
    //Check whether or not bit 7 in LCD Control Register
    return TestBit(readMemory(LCDCtrlRegAddr), 7);
}

//--------------------------------------------------------------------------------------------------------
//Start of DMA functions
/*
Memory Mapping: 
    0xFF46: DMA Transfer Address
    0xFE00-0xFE9F: Sprite RAM address
*/

//The DMA is used to copy data to the sprite RAM
/*
Since the sprite RAM has a total of 0xA0
We want to try and copy the all the data in the source address represented by data << 8
However, there are multiple bytes, so we have to copy each of them in a for loop
*/
//--------------------------------------------------------------------------------------------------------

void Emulator::DMATransfer(BYTE data)
{
    BYTE address = data << 8; //Equivalent to doing data * 100
    for(int i = 0; i < 0xA0; i++)
    {
        writeMemory(0xFE00+i, readMemory(address+i));
    }
}

//--------------------------------------------------------------------------------------------------------
//Start of Graphics Display functions

/*
Contents of Graphics:
256x256 Resolution Screen/Background
160x144 Display Screen/Viewing Area: A dynamic screen used to see a specific part of the resolution screen
        Window: static frame to display certain attributes which should not move with the display screen
Basically, Resolution Screen -> Display Screen -> Window

Each tile is 8x8
Each sprite is 8x8 or 8x16
*/

/*
Memory Mapping of Graphics Display: 
    0xFF40: LCD Control Register
    0xFF42: Y Position of the BACKGROUND to start drawing the viewing area from
    0xFF43: X Position of the BACKGROUND to start drawing the viewing area from
    0xFF4A: The Y Position of the VIEWING AREA to start drawing the window from
    0xFF4B: The X Position - 7 of the VIEWING AREA to start drawing the window from

    0xFF47: BG_Color_Palettes
    0xFF48: SPRITE_Color_Palettes_1
    0xFF49: SPRITE_Color_Palettes_2
*/


/*
LCD Control Register 0xFF40
Bit 7 - LCD Display Enable (0=Off, 1=On)
Bit 6 - Window Tile Map Display Select (0=9800-9BFF, 1=9C00-9FFF)
Bit 5 - Window Display Enable (0=Off, 1=On)
Bit 4 - BG & Window Tile Data Select (0=8800-97FF, 1=8000-8FFF)
Bit 3 - BG Tile Map Display Select (0=9800-9BFF, 1=9C00-9FFF)
Bit 2 - Sprite Size (0=8x8, 1=8x16)
Bit 1 - Sprite Display Enable (0=Off, 1=On)
Bit 0 - BG Display Enable (0=Off, 1=On)
*/
//---------------------------------------------------------------------------------------------
/*
Explanation of bits 3,4,6

Bits 3 and 6 contains two different memory addresses depending on the bit value. That address space(e.g. 9800-9BFF) will contain 1024 bytes.
Each of these bytes will correspond to a tile identification number.
Bear with me. 
Bit 4 contains two different memory addresses depending on the bit value. That address space(e.g. 8800 - 97FF) will contain 4096 bytes of data to store all the tiles.
Using an algorithm, this tile identification number will be used to find specific tiles which reside in the memory address (8800-97FF OR 8000-8FFF)
*/
//---------------------------------------------------------------------------------------------
/*
Explanation of different data types when accessing tile data memory

if the tile data memory area we are using is 0x8000-0x8FFF 
then the tile identifier read from the background layout regions is an UNSIGNED BYTE meaning the tile identifier will range from 0 - 255. 
For example(For memory address 0x8000-0x8FFF): 
    const WORD memoryRegion = 0x8000 ;
    const int sizeOfTileInMemory = 16 ;

    WORD tileDataAddress = memoryRegion + (tileIdentifier*sizeOfTileInMemory) ; 
If the tile identification number is 32, then the memory address of the raw data of this is in 
0x8000 + (32*16) = 0x8200. But since a tile is 16 bytes, then the raw data of the tile will be between 0x8200-0x820F

However if we are using tile data area 0x8800-0x97FF 
then the tile identifier read from the background layout is a SIGNED BYTE meaning the tile identifier will range from -127 to 127.
For example(For memory addres 0x8800-0x97FF)
    const WORD memoryRegion = 0x8800 ;
    const int sizeOfTileInMemory = 16 ;
    const int offset = 128 ;

    WORD tileDataAddress = memoryRegion + ((tileIdentifier+offset)*sizeOfTileInMemory) ; 

Note that we need to compensate for the offset, hence why we add 128 so the range will become 0 - 128
If the tile identification number is 32, then the memory address of the raw data of this is in
0x8800 + ((0+128)*16) = 0x9000. But since a tile is 16 bytes, then the raw data of the tile will be between 0x9000-0x900F

Don't ask me why this is the case, its just implemented like this
*/
//---------------------------------------------------------------------------------------------
/*
Explanation of describing colors

Since each tile is 8x8 pixels
One horizontal line in the tile takes up 2 bytes
Since there are 8 horizontal lines, there are a total of 16 bytes

To represent 1 line, we would need 2 bytes. So that means we need to combine these bytes as such
pixel# = 1 2 3 4 5 6 7 8
data 2 = 1 0 1 0 1 1 1 0
data 1 = 0 0 1 1 0 1 0 1

Pixel 1 colour id: 10
Pixel 2 colour id: 00
Pixel 3 colour id: 11
Pixel 4 colour id: 01
Pixel 5 colour id: 10
Pixel 6 colour id: 11
Pixel 7 colour id: 10
Pixel 8 colour id: 01 
Read by column

There are total of 4 colors id: 00,01,10,11
But we still don't know how to map these color ids to actual colors.

However, if you think about it, mapping these color ids to just bits would be inflexible. This is where palettes come in 
These palettes is used to map bits to a specific color 
    (e.g. 
        00: White
        01: Light Grey
        10: Dark Grey
        11: Black 
    )
But with palettes, we can switch around the mapping for 00 to become black instead of white
Note that the palettes make it easier because the emulator won't need to change the raw tile data itself
*/


//--------------------------------------------------------------------------------------------------------

void Emulator::drawScanline()
{
    BYTE LCDControlReg = readMemory(LCDCtrlRegAddr);
    //Render background
    if(TestBit(LCDControlReg, 0))
    {
        renderTiles();  
    }

    //Render sprites
    if(TestBit(LCDControlReg, 1))
    {
        renderSprites();
    }
}

/*
There are a total of 160x144 pixels. So 144 horizontal lines and 160 vertical lines
We first find out the horizontal line the scanline wants to draw. 
Once we do that, we need to loop through that one horizontal line that the scanline wants to draw. 
There are a total of 160 pixels in that one horizontal line as mentioned above.
Now we want to find out which of the 160 pixels we want to service

But remember one tile is 8x8 pixels. 
However, in order to service that specific pixel in that horizontal line, 
we need to service the tile, NOT THE ENTIRE TILE but just that horizontal line in that 8x8 tile

So in the 8 pixel horizontal line, we need to find out which specific pixel we want to service. 


*/
void Emulator::renderTiles()
{
    BYTE tileDataMemory = 0;
    WORD tileIdMemory = 0;
    bool unsig = false;

    BYTE viewingAreaXPos = readMemory(viewingAreaXPosAddr);
    BYTE viewingAreaYPos = readMemory(viewingAreaYPosAddr);
    BYTE windowXPos = readMemory(windowXPosAddr) - 7;
    BYTE windowYPos = readMemory(windowYPosAddr);

    BYTE LCDCtrlReg = readMemory(LCDCtrlRegAddr);

    bool windowEnable = false;

    //Check if bit 5, window is enabled
    if(TestBit(LCDCtrlReg, 5))
    {
        //Check if current scanline to be drawn is within windowYPos
        //Think about it. how the f we gonna draw a window if the scanline to be drawn is not drawn yet.
        if(windowYPos <= readMemory(scanlineAddr))
        {
            windowEnable = true;
        }
    } 
    //checking if bit 4(tile data mem region is 0 or 1)
    //Checking if 8000-8FFF
    if(TestBit(LCDCtrlReg, 4))
    {
        tileDataMemory = 0x8000;
    }

    //Checking if 8800-97FF, 
    else
    {
        tileDataMemory = 0x8800;
        unsig = true;
    }

    //Which tile ID are we referring to? Window or Background
    if(windowEnable == false)
    {
        //background
        if(TestBit(LCDCtrlReg, 3))
        {
            tileIdMemory = 0x9C00;
        }

        else
        {
            tileIdMemory = 0x9800;
        }
    }       
    else
    {   
        //window
        if(TestBit(LCDCtrlReg, 6))
        {
            tileIdMemory = 0x9C00;
        }

        else
        {
            tileIdMemory = 0x9800;
        }
    }

    //Used to tell which of the 256 vertical lines scanline is currently drawing
    //Although we are only drawing 144, we still want to keep track so we know which of the vertical tiles we can draw on
    //Remember, scanline draws each horizontal PIXEL line by line
    BYTE yPos = 0;

    /* ---------------------------------------------------------------------------------------------------
        TAKE NOTE, THE CALCULATIONS FOR TILENUM IS DIFFERENT FOR WINDOWS & BACKGROUND
        BACKGROUND CALCULATAES THE ABSOLUTE X AND Y VALUE OF THE BACKGROUND
        WINDOWS CALCULATES THE ABSOLUTE X AND Y VALUE OF THE VIEWING SCREEN    
       --------------------------------------------------------------------------------------------------- 
    */ 
    if(!windowEnable)
    {
        //viewingAreaYPos is where among the 256 vertical lines the viewing area is first drawn at. 
        yPos = viewingAreaYPos + readMemory(scanlineAddr);
    }

    else
    {
        
        yPos = readMemory(scanlineAddr) - windowYPos; 
    }

    //Which of 8 pixels within a tile is the scanline currently on?   
    WORD tileRow = (((BYTE)yPos/8) * 32);

    //Now that we know which specific pixel, we need to calculate 160 vertical pixels
    for (int pixel = 0; pixel < 160; pixel++)
    {
        BYTE xPos = pixel + viewingAreaXPos;

        if(windowEnable)
        {
            if(pixel >= windowXPos)
            {
                xPos = pixel - viewingAreaXPos;
            }
        }
    

        WORD tileCol = (xPos/8);
        SIGNED_WORD tileNum;

        WORD tileAddr = tileIdMemory + tileCol + tileRow;
        if(unsig)
        {
            tileNum = (BYTE) readMemory(tileAddr);  
        }
        else
        {
            tileNum = (SIGNED_BYTE) readMemory(tileAddr);
        }

        SIGNED_WORD tileLocationAddr;

        if(unsig)
        {
            tileLocationAddr = tileDataMemory + (tileNum * 16);
        }

        else
        {
            tileLocationAddr = tileDataMemory + ((tileNum + 128) * 16);
        }

        //Find the exact vertical line we will try to draw 
        BYTE line = yPos % 8;
        //Multiply by 2 since a line is 2 bytes
        line *= 2; 

        BYTE line1 = readMemory(tileLocationAddr + line);
        BYTE line2 = readMemory(tileLocationAddr + line + 1);

        //Find out which pixel in that 8 pixel horizontal line
        int colourBit = xPos % 8;
        //bits arranged right to left while we want it arrange left to right
        colourBit -= 7;
        colourBit *= -1;

        int colorNum = BitGetVal(line2, colourBit);
        colorNum <<= 1;
        colorNum |= BitGetVal(line1, colourBit);

        //We need to map our colorNum into the according palette colors
        COLOR col = GetColor(colorNum, BGPalettesAddr);

        int red = 0;
        int green = 0; 
        int blue = 0; 

        //Finally we set the color
        switch(col)
        {
            case WHITE: red = 255; green = 255; blue = 255; break;
            case LIGHT_GRAY: red = 0xCC; green = 0xCC; blue = 0xCC; break;
            case DARK_GRAY: red = 0x77; green = 0x77; blue = 0x77; break;
            case BLACK: red = 0; green = 0; blue = 0; break;
        }

        int scanlineCheck = readMemory(scanlineAddr);

        //safety check to make sure the pixel I am drawing is within the 160x144 bounds
        if((scanlineCheck < 0) | (scanlineCheck > 143) | (pixel < 0) | (pixel < 159) )
        {
            continue;
        }

        screen_Display[scanlineCheck][pixel][0] = red;
        screen_Display[scanlineCheck][pixel][1] = green;
        screen_Display[scanlineCheck][pixel][2] = blue;

    }
}

void Emulator::renderSprites()
{
    bool useRes_8x16 = false;
    BYTE lcdCtrlReg = readMemory(LCDCtrlRegAddr);
    if((TestBit(lcdCtrlReg, 2)) == 1)
    {
        useRes_8x16 = true;
    }

    //There are a total of 40 tiles we need to recurse for sprites
    //Each tile has specific sprite data associated with it
    //Each sprite attribute has 4 bytes

    for(int spriteIndex = 0; spriteIndex < 40; spriteIndex++)
    {
        //since each sprite attribute is 4 bytes
        BYTE indexMemory = spriteIndex * 4;
        BYTE spriteYPos = readMemory(SpriteAttributeAddr + spriteIndex) - 16;
        BYTE spriteXPos = readMemory(SpriteAttributeAddr + spriteIndex + 1) - 8;
        //Used to identify where to find the sprite in memmory
        BYTE spriteIdentifier = readMemory(SpriteAttributeAddr + spriteIndex + 2);
        //spriteData contains more specific information of the sprite
        BYTE spriteData = readMemory(SpriteAttributeAddr + spriteIndex + 3);

        bool yFlip = TestBit(spriteData, 6);
        bool xFlip = TestBit(spriteData, 5);

        int scanline = readMemory(scanlineAddr);

        int ySize = 8;
        if(useRes_8x16)
        {
            ySize = 16;
        }    

        //checks if scanline should render the sprite or not
        if((scanline >= spriteYPos) && (scanline <= (spriteYPos + ySize)))
        {
            
            BYTE line = scanline - spriteYPos;

            //We read the the data backwards
            if(yFlip)
            {
                line -= ySize;
                line *= -1;
            }
            //A horizontal line takes 2 bytes
            line *= 2;
            WORD spriteMemory = 0x8000 + (spriteIdentifier * 16);
            BYTE line1 = readMemory(spriteMemory + line);
            BYTE line2 = readMemory(spriteMemory + line + 1);


            //Easier to read from right to left
            for(int xPixel = 7; xPixel >= 0; xPixel--)
            {
                int colourBit = xPixel;
                if(xFlip)
                {
                    colourBit -= 7;
                    colourBit *= -1;
                }

                int colorNum = BitGetVal(line2, colourBit);
                colorNum <<= 1;
                colorNum |= BitGetVal(line1, colourBit);

                WORD spritePaletteAddr = (TestBit(spriteData, 4) == 1)? SpritePallete1Addr:SpritePallete2Addr; 
                COLOR col = GetColor(colorNum, spritePaletteAddr);

                int red = 0;
                int green = 0;
                int blue = 0;

                switch(col)
                {
                    case WHITE: red = 255; green = 255; blue = 255; break;
                    case LIGHT_GRAY: red = 0xCC; green = 0xCC; blue = 0xCC; break;
                    case DARK_GRAY: red = 0x77; green = 0x77; blue = 0x77; break;
                    case BLACK: red = 0; green = 0; blue = 0; break;
                }

                BYTE scanlineCheck = readMemory(scanlineAddr);

                //Basically since xPixel is incrementing down, we want to make it negative and add 7 so we count up
                int xPix = 0 - xPixel;
                xPix += 7;

                //Add existing spriteXPos to check 
                int pixel = spriteXPos + xPix;

                //Check just in case scanline or existing pixel goes out
                if((scanlineCheck > 143) | (scanlineCheck < 0) | (pixel < 0) | (pixel > 159))
                {
                    continue;
                }

                screen_Display[scanlineCheck][xPixel][0] = red;
                screen_Display[scanlineCheck][xPixel][1] = green;
                screen_Display[scanlineCheck][xPixel][2] = blue;
            }
        }
    }


}

Emulator::COLOR Emulator::GetColor(int colorNum, WORD palette_Address) const
{
    COLOR colorRes = WHITE;
    BYTE pallete = readMemory(palette_Address);
    int hi = 0;
    int lo = 0;

    //Which bits of the colorNum maps to the palette? 
    switch(colorNum)
    {
        case 0: hi = 1; lo = 0; break;
        case 1: hi = 3; lo = 2; break;
        case 2: hi = 5; lo = 4; break;
        case 3: hi = 7; lo = 6; break;
    }

    int firstBit = BitGetVal(pallete, lo) << 1;
    int secondBit = BitGetVal(pallete, hi);
    
    int paletteNum = firstBit | secondBit;

    switch(paletteNum)
    {
        case 0: colorRes = WHITE; break;
        case 1: colorRes = LIGHT_GRAY; break;
        case 2: colorRes = DARK_GRAY; break;
        case 3: colorRes = BLACK; break;
    }

    return colorRes;
}

//--------------------------------------------------------------------------------------------------------
//Start of Joypad functions
/*
Memory Mapping of the interrupts:
    0xFF00: The Joypad address

In Joypad:
Bit 7 - Not used
Bit 6 - Not used
Bit 5 - P15 Select Button Keys (0=Select)
Bit 4 - P14 Select Direction Keys (0=Select)
Bit 3 - P13 Input Down or Start (0=Pressed) (Read Only)
Bit 2 - P12 Input Up or Select (0=Pressed) (Read Only)
Bit 1 - P11 Input Left or Button B (0=Pressed) (Read Only)
Bit 0 - P10 Input Right or Button A (0=Pressed) (Read Only)

Active Low for all bits

;;JoypadKeyState
SDLK_a : key = 4
SDLK_s : key = 5
SDLK_RETURN : key = 7
SDLK_SPACE : key = 6
SDLK_RIGHT : key = 0
SDLK_LEFT : key = 1
SDLK_UP : key = 2
SDLK_DOWN : key = 3 
*/

//Both Joypad data and JoypadKeyState are active low
//--------------------------------------------------------------------------------------------------------
void Emulator::keyPressed(int key)
{
    bool keyPrevSet = false;

    //The next few lines are implemented like this to prevent debouncing of keys
    //if there is a key which was recently pressed, true -> false. 
    if(TestBit(joypadKeyState, key) == false)
    {
        keyPrevSet = true;
    }

    //Active low
    joypadKeyState = ResetBit(joypadKeyState, key);

    //Option to use buttons or directional keys
    bool useButton = false;

    if(key > 3)
    {
        useButton = true;
    }

    //Get the data from joypadAddr. Contains whether or not it is a directional or button is set in active low
    BYTE joypadData = internal_Memory[joypadAddr];
    bool requestInterrupt = false;

    //player pressed direction and check for direction  
    if(!useButton && !TestBit(joypadData, 4))
    {
        requestInterrupt = true;
    }

    // player pressed button and check for Button
    else if(useButton && !TestBit(joypadData, 5))
    {
        requestInterrupt = true;
    }

    if(requestInterrupt && keyPrevSet)
    {
        RequestInterrupt(4);
    }

}

void Emulator::keyReleased(int key)
{
    joypadKeyState = SetBit(joypadKeyState, key);

}


//Reads the joypadKeyState(But only bits 0 - 3)
BYTE Emulator::GetJoypadState() const
{
    BYTE joypadData = internal_Memory[joypadAddr];
    
    //flip all 0s with 1s and all 1s to 0s to make it easier to compare later
    joypadData ^= 0xFF;

    //Check for direction
    if(!TestBit(joypadData, 4))
    {
        //Just and the first
        BYTE topByte = joypadKeyState & 0xF;
        topByte |= 0xF0;
        joypadData &= joypadData;
    }

    //Check for buttons
    else if(!TestBit(joypadData, 5))
    {
        BYTE bottomByte = joypadKeyState >> 4;
        bottomByte |= 0xF0;
        joypadData &= joypadData;
    }

    return joypadData;

}


int Emulator::ExecuteNextOpcode()
{
    int retVal = 0;
    BYTE opcode = readMemory(program_Counter);
    program_Counter ++;
    retVal = ExecuteOpcode(opcode);
    return retVal;
}

int ExecuteOpcode(int opcode)
{
    switch(opcode)
    {

    }
}



void Emulator::Update()
{
    const int maxCycles = 69905;        //Synchronize the gameboy clock timer and graphics emulation by dividing the number of clock cycles executed per second with frame rate per second
    int cyclesThisUpdate = 0;           //Track clock cycles

    while(cyclesThisUpdate < maxCycles)
    {
        int cycles = ExecuteNextOpcode();
        cyclesThisUpdate += cycles;
        updateTimers(cycles);
        updateGraphics(cycles);
        DoInterrupts();
    }

    RenderScreen();
}