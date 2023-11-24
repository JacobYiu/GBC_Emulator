#include "Emulator.h"
#include "utils.h"
#include "LogMessage.h"
#include <stdio.h>


int Emulator::ExecuteOpcode(BYTE opcode)
{
	switch(opcode)
	{
		//no-operation (A->A)
		case 0x00: cyclesThisUpdate += 4 ; break ;

		//Load 8 bit immediate value into register
		case 0x06: CPU_8BIT_LOAD(reg_BC.hi) ; return 8 ; break ;
		case 0x0E: CPU_8BIT_LOAD(reg_BC.lo) ; return 8 ; break ;
		case 0x16: CPU_8BIT_LOAD(reg_DE.hi) ; return 8 ; break ;
		case 0x1E: CPU_8BIT_LOAD(reg_DE.lo) ; return 8 ; break ;
		case 0x26: CPU_8BIT_LOAD(reg_HL.hi) ; return 8 ; break ;
		case 0x2E: CPU_8BIT_LOAD(reg_HL.lo) ; return 8 ; break ;

        //Load register into a register
        //Right -> Left
        //e.g. reg_AF.hi = A <- reg_BC.hi = B
        //A -> B
		case 0x7F: CPU_REG2REG_LOAD(reg_AF.hi, reg_AF.hi) ; return 4 ; break ;
		case 0x78: CPU_REG2REG_LOAD(reg_AF.hi, reg_BC.hi) ; return 4 ; break ;
		case 0x79: CPU_REG2REG_LOAD(reg_AF.hi, reg_BC.lo) ; return 4 ; break ;
		case 0x7A: CPU_REG2REG_LOAD(reg_AF.hi, reg_DE.hi) ; return 4 ; break ;
		case 0x7B: CPU_REG2REG_LOAD(reg_AF.hi, reg_DE.lo) ; return 4 ; break ;
		case 0x7C: CPU_REG2REG_LOAD(reg_AF.hi, reg_HL.hi) ; return 4 ; break ;
		case 0x7D: CPU_REG2REG_LOAD(reg_AF.hi, reg_HL.lo) ; return 4 ; break ;
		case 0x40: CPU_REG2REG_LOAD(reg_BC.hi, reg_BC.hi) ; return 4 ; break ;
		case 0x41: CPU_REG2REG_LOAD(reg_BC.hi, reg_BC.lo) ; return 4 ; break ;
		case 0x42: CPU_REG2REG_LOAD(reg_BC.hi, reg_DE.hi) ; return 4 ; break ;
		case 0x43: CPU_REG2REG_LOAD(reg_BC.hi, reg_DE.lo) ; return 4 ; break ;
		case 0x44: CPU_REG2REG_LOAD(reg_BC.hi, reg_HL.hi) ; return 4 ; break ;
		case 0x45: CPU_REG2REG_LOAD(reg_BC.hi, reg_HL.lo) ; return 4 ; break ;
		case 0x48: CPU_REG2REG_LOAD(reg_BC.lo, reg_BC.hi) ; return 4 ; break ;
		case 0x49: CPU_REG2REG_LOAD(reg_BC.lo, reg_BC.lo) ; return 4 ; break ;
		case 0x4A: CPU_REG2REG_LOAD(reg_BC.lo, reg_DE.hi) ; return 4 ; break ;
		case 0x4B: CPU_REG2REG_LOAD(reg_BC.lo, reg_DE.lo) ; return 4 ; break ;
		case 0x4C: CPU_REG2REG_LOAD(reg_BC.lo, reg_HL.hi) ; return 4 ; break ;
		case 0x4D: CPU_REG2REG_LOAD(reg_BC.lo, reg_HL.lo) ; return 4 ; break ;
		case 0x50: CPU_REG2REG_LOAD(reg_DE.hi, reg_BC.hi) ; return 4 ; break ;
		case 0x51: CPU_REG2REG_LOAD(reg_DE.hi, reg_BC.lo) ; return 4 ; break ;
		case 0x52: CPU_REG2REG_LOAD(reg_DE.hi, reg_DE.hi) ; return 4 ; break ;
		case 0x53: CPU_REG2REG_LOAD(reg_DE.hi, reg_DE.lo) ; return 4 ; break ;
		case 0x54: CPU_REG2REG_LOAD(reg_DE.hi, reg_HL.hi) ; return 4 ; break ;
		case 0x55: CPU_REG2REG_LOAD(reg_DE.hi, reg_HL.lo) ; return 4 ; break ;
		case 0x58: CPU_REG2REG_LOAD(reg_DE.lo, reg_BC.hi) ; return 4 ; break ;
		case 0x59: CPU_REG2REG_LOAD(reg_DE.lo, reg_BC.lo) ; return 4 ; break ;
		case 0x5A: CPU_REG2REG_LOAD(reg_DE.lo, reg_DE.hi) ; return 4 ; break ;
		case 0x5B: CPU_REG2REG_LOAD(reg_DE.lo, reg_DE.lo) ; return 4 ; break ;
		case 0x5C: CPU_REG2REG_LOAD(reg_DE.lo, reg_HL.hi) ; return 4 ; break ;
		case 0x5D: CPU_REG2REG_LOAD(reg_DE.lo, reg_HL.lo) ; return 4 ; break ;
		case 0x60: CPU_REG2REG_LOAD(reg_HL.hi, reg_BC.hi) ; return 4 ; break ;
		case 0x61: CPU_REG2REG_LOAD(reg_HL.hi, reg_BC.lo) ; return 4 ; break ;
		case 0x62: CPU_REG2REG_LOAD(reg_HL.hi, reg_DE.hi) ; return 4 ; break ;
		case 0x63: CPU_REG2REG_LOAD(reg_HL.hi, reg_DE.lo) ; return 4 ; break ;
		case 0x64: CPU_REG2REG_LOAD(reg_HL.hi, reg_HL.hi) ; return 4 ; break ;
		case 0x65: CPU_REG2REG_LOAD(reg_HL.hi, reg_HL.lo) ; return 4 ; break ;
		case 0x68: CPU_REG2REG_LOAD(reg_HL.lo, reg_BC.hi) ; return 4 ; break ;
		case 0x69: CPU_REG2REG_LOAD(reg_HL.lo, reg_BC.lo) ; return 4 ; break ;
		case 0x6A: CPU_REG2REG_LOAD(reg_HL.lo, reg_DE.hi) ; return 4 ; break ;
		case 0x6B: CPU_REG2REG_LOAD(reg_HL.lo, reg_DE.lo) ; return 4 ; break ;
		case 0x6C: CPU_REG2REG_LOAD(reg_HL.lo, reg_HL.hi) ; return 4 ; break ;
		case 0x6D: CPU_REG2REG_LOAD(reg_HL.lo, reg_HL.lo) ; return 4 ; break ;

		// write reg to memory address
		case 0x70: WRITE_BYTE(reg_HL.reg, reg_BC.hi) ; return 8 ; break ;
		case 0x71: WRITE_BYTE(reg_HL.reg, reg_BC.lo) ; return 8 ; break ;
		case 0x72: WRITE_BYTE(reg_HL.reg, reg_DE.hi) ; return 8 ; break ;
		case 0x73: WRITE_BYTE(reg_HL.reg, reg_DE.lo) ; return 8 ; break ;
		case 0x74: WRITE_BYTE(reg_HL.reg, reg_HL.hi) ; return 8 ; break ;
		case 0x75: WRITE_BYTE(reg_HL.reg, reg_HL.lo) ; return 8 ; break ;

		// write contents in memory address to reg
		case 0x7E: CPU_REG_LOAD_ROM(reg_AF.hi, reg_HL.reg) ; return 8 ; break ;
		case 0x46: CPU_REG_LOAD_ROM(reg_BC.hi, reg_HL.reg) ; return 8 ; break ;
		case 0x4E: CPU_REG_LOAD_ROM(reg_BC.lo, reg_HL.reg) ; return 8 ; break ;
		case 0x56: CPU_REG_LOAD_ROM(reg_DE.hi, reg_HL.reg) ; return 8 ; break ;
		case 0x5E: CPU_REG_LOAD_ROM(reg_DE.lo, reg_HL.reg) ; return 8 ; break ;
		case 0x66: CPU_REG_LOAD_ROM(reg_HL.hi, reg_HL.reg) ; return 8 ; break ;
		case 0x6E: CPU_REG_LOAD_ROM(reg_HL.lo, reg_HL.reg) ; return 8 ; break ;
		case 0x0A: CPU_REG_LOAD_ROM(reg_AF.hi, reg_BC.reg) ; return 8 ; break ;
		case 0x1A: CPU_REG_LOAD_ROM(reg_AF.hi, reg_DE.reg) ; return 8 ; break ;
		case 0xF2: CPU_REG_LOAD_ROM(reg_AF.hi, (0xFF00+reg_BC.lo)) ; return 8 ; break ;


		// put register A into a register
		case 0x47 : CPU_REG2REG_LOAD(reg_BC.hi, reg_AF.hi) ; return 4 ; break ;
		case 0x4F : CPU_REG2REG_LOAD(reg_BC.lo, reg_AF.hi) ; return 4 ; break ;
		case 0x57 : CPU_REG2REG_LOAD(reg_DE.hi, reg_AF.hi) ; return 4 ; break ;
		case 0x5F : CPU_REG2REG_LOAD(reg_DE.lo, reg_AF.hi) ; return 4 ; break ;
		case 0x67 : CPU_REG2REG_LOAD(reg_HL.hi, reg_AF.hi) ; return 4 ; break ;
		case 0x6F : CPU_REG2REG_LOAD(reg_HL.lo, reg_AF.hi) ; return 4 ; break ;

		// put register A into memory address
		case 0x02: WRITE_BYTE(reg_BC.reg, reg_AF.hi) ; return 8 ; break ;
		case 0x12: WRITE_BYTE(reg_DE.reg, reg_AF.hi) ; return 8 ; break ;
		case 0x77: WRITE_BYTE(reg_HL.reg, reg_AF.hi) ; return 8 ; break ;
		case 0xE2: WRITE_BYTE((0xFF00+reg_BC.lo), reg_AF.hi) ; return 8 ; break ;

		// put memory into register A, decrement/increment memory
		case 0x3A: CPU_REG_LOAD_ROM(reg_AF.hi,reg_HL.reg ) ; CPU_16BIT_DEC(reg_HL.reg) ; return 8 ; break ;
		case 0x2A: CPU_REG_LOAD_ROM(reg_AF.hi,reg_HL.reg ) ; CPU_16BIT_INC(reg_HL.reg) ; return 8 ; break ;

		// put register A into memory, decrement/increment reg
		case 0x32: WRITE_BYTE(reg_HL.reg, reg_AF.hi); CPU_16BIT_DEC(reg_HL.reg) ; return 8 ; break;
		case 0x22: WRITE_BYTE(reg_HL.reg, reg_AF.hi); CPU_16BIT_INC(reg_HL.reg) ; return 8 ; break;

		// 16 bit loads
		case 0x01: CPU_16BIT_LOAD( reg_BC.reg ) ; return 16 ; break ;
		case 0x11: CPU_16BIT_LOAD( reg_DE.reg ) ; return 16 ; break ;
		case 0x21: CPU_16BIT_LOAD( reg_HL.reg ) ; return 16 ; break ;
		case 0x31: CPU_16BIT_LOAD( stack_Pointer.reg ) ; return 16 ; break ;
		case 0xF9: stack_Pointer.reg = reg_HL.reg ; return 8 ; break ;

		// push word onto stack
		case 0xF5: PushWordOntoStack( reg_AF.reg ) ; return 16 ; break ;
		case 0xC5: PushWordOntoStack( reg_BC.reg ) ; return 16 ; break ;
		case 0xD5: PushWordOntoStack( reg_DE.reg ) ; return 16 ; break ;
		case 0xE5: PushWordOntoStack( reg_HL.reg ) ; return 16 ; break ;

		// pop word from stack into reg
		case 0xF1: reg_AF.reg = PopWordOffStack( ) ; return 12 ; break ;
		case 0xC1: reg_BC.reg = PopWordOffStack( ) ; return 12 ; break ;
		case 0xD1: reg_DE.reg = PopWordOffStack( ) ; return 12 ; break ;
		case 0xE1: reg_HL.reg = PopWordOffStack( ) ; return 12 ; break ;

		// 8-bit add
		case 0x87: CPU_8BIT_ADD(reg_AF.hi, reg_AF.hi,false,false) ; return 4 ; break ;
		case 0x80: CPU_8BIT_ADD(reg_AF.hi, reg_BC.hi,false,false) ; return 4 ; break ;
		case 0x81: CPU_8BIT_ADD(reg_AF.hi, reg_BC.lo,false,false) ; return 4 ; break ;
		case 0x82: CPU_8BIT_ADD(reg_AF.hi, reg_DE.hi,false,false) ; return 4 ; break ;
		case 0x83: CPU_8BIT_ADD(reg_AF.hi, reg_DE.lo,false,false) ; return 4 ; break ;
		case 0x84: CPU_8BIT_ADD(reg_AF.hi, reg_HL.hi,false,false) ; return 4 ; break ;
		case 0x85: CPU_8BIT_ADD(reg_AF.hi, reg_HL.lo,false,false) ; return 4 ; break ;
		case 0x86: CPU_8BIT_ADD(reg_AF.hi, readByte(reg_HL.reg),false,false) ; return 8 ; break ;
		case 0xC6: CPU_8BIT_ADD(reg_AF.hi, 0,true,false) ; return 8 ; break ;

		// 8-bit add + carry
		case 0x8F: CPU_8BIT_ADD(reg_AF.hi, reg_AF.hi,false,true) ; return 4 ; break ;
		case 0x88: CPU_8BIT_ADD(reg_AF.hi, reg_BC.hi,false,true) ; return 4 ; break ;
		case 0x89: CPU_8BIT_ADD(reg_AF.hi, reg_BC.lo,false,true) ; return 4 ; break ;
		case 0x8A: CPU_8BIT_ADD(reg_AF.hi, reg_DE.hi,false,true) ; return 4 ; break ;
		case 0x8B: CPU_8BIT_ADD(reg_AF.hi, reg_DE.lo,false,true) ; return 4 ; break ;
		case 0x8C: CPU_8BIT_ADD(reg_AF.hi, reg_HL.hi,false,true) ; return 4 ; break ;
		case 0x8D: CPU_8BIT_ADD(reg_AF.hi, reg_HL.lo,false,true) ; return 4 ; break ;
		case 0x8E: CPU_8BIT_ADD(reg_AF.hi, readByte(reg_HL.reg),false,true) ; return 8; break ;
		case 0xCE: CPU_8BIT_ADD(reg_AF.hi, 0,true,true) ; return 8 ; break ;

		// 8-bit subtract
		case 0x97: CPU_8BIT_SUB(reg_AF.hi, reg_AF.hi,false,false) ; return 4 ; break ;
		case 0x90: CPU_8BIT_SUB(reg_AF.hi, reg_BC.hi,false,false) ; return 4 ; break ;
		case 0x91: CPU_8BIT_SUB(reg_AF.hi, reg_BC.lo,false,false) ; return 4 ; break ;
		case 0x92: CPU_8BIT_SUB(reg_AF.hi, reg_DE.hi,false,false) ; return 4 ; break ;
		case 0x93: CPU_8BIT_SUB(reg_AF.hi, reg_DE.lo,false,false) ; return 4 ; break ;
		case 0x94: CPU_8BIT_SUB(reg_AF.hi, reg_HL.hi,false,false) ; return 4 ; break ;
		case 0x95: CPU_8BIT_SUB(reg_AF.hi, reg_HL.lo,false,false) ; return 4 ; break ;
		case 0x96: CPU_8BIT_SUB(reg_AF.hi, readByte(reg_HL.reg),false,false) ;  return 8 ; break ;
		case 0xD6: CPU_8BIT_SUB(reg_AF.hi, 0,true,false) ; return 8 ; break ;

		// 8-bit subtract + carry
		case 0x9F: CPU_8BIT_SUB(reg_AF.hi, reg_AF.hi,false,true) ; return 4 ; break ;
		case 0x98: CPU_8BIT_SUB(reg_AF.hi, reg_BC.hi,false,true) ; return 4 ; break ;
		case 0x99: CPU_8BIT_SUB(reg_AF.hi, reg_BC.lo,false,true) ; return 4 ; break ;
		case 0x9A: CPU_8BIT_SUB(reg_AF.hi, reg_DE.hi,false,true) ; return 4 ; break ;
		case 0x9B: CPU_8BIT_SUB(reg_AF.hi, reg_DE.lo,false,true) ; return 4 ; break ;
		case 0x9C: CPU_8BIT_SUB(reg_AF.hi, reg_HL.hi,false,true) ; return 4 ; break ;
		case 0x9D: CPU_8BIT_SUB(reg_AF.hi, reg_HL.lo,false,true) ; return 4 ; break ;
		case 0x9E: CPU_8BIT_SUB(reg_AF.hi, readByte(reg_HL.reg),false,true) ;  return 8 ; break ;
		case 0xDE: CPU_8BIT_SUB(reg_AF.hi, 0,true,true) ; return 8 ; break ;

		// 8-bit AND reg with reg
		case 0xA7: CPU_8BIT_AND(reg_AF.hi, reg_AF.hi, false) ; return 4 ; break ;
		case 0xA0: CPU_8BIT_AND(reg_AF.hi, reg_BC.hi, false) ; return 4 ; break ;
		case 0xA1: CPU_8BIT_AND(reg_AF.hi, reg_BC.lo, false) ; return 4 ; break ;
		case 0xA2: CPU_8BIT_AND(reg_AF.hi, reg_DE.hi, false) ; return 4 ; break ;
		case 0xA3: CPU_8BIT_AND(reg_AF.hi, reg_DE.lo, false) ; return 4 ; break ;
		case 0xA4: CPU_8BIT_AND(reg_AF.hi, reg_HL.hi, false) ; return 4 ; break ;
		case 0xA5: CPU_8BIT_AND(reg_AF.hi, reg_HL.lo, false) ; return 4 ; break ;
		case 0xA6: CPU_8BIT_AND(reg_AF.hi, readByte(reg_HL.reg), false) ; return 8 ; break ;
		case 0xE6: CPU_8BIT_AND(reg_AF.hi, 0, true) ; return 8 ; break ;

		// 8-bit OR reg with reg
		case 0xB7: CPU_8BIT_OR(reg_AF.hi, reg_AF.hi, false) ; return 4 ; break ;
		case 0xB0: CPU_8BIT_OR(reg_AF.hi, reg_BC.hi, false) ; return 4 ; break ;
		case 0xB1: CPU_8BIT_OR(reg_AF.hi, reg_BC.lo, false) ; return 4 ; break ;
		case 0xB2: CPU_8BIT_OR(reg_AF.hi, reg_DE.hi, false) ; return 4 ; break ;
		case 0xB3: CPU_8BIT_OR(reg_AF.hi, reg_DE.lo, false) ; return 4 ; break ;
		case 0xB4: CPU_8BIT_OR(reg_AF.hi, reg_HL.hi, false) ; return 4 ; break ;
		case 0xB5: CPU_8BIT_OR(reg_AF.hi, reg_HL.lo, false) ; return 4 ; break ;
		case 0xB6: CPU_8BIT_OR(reg_AF.hi, readByte(reg_HL.reg), false) ; return 8 ; break ;
		case 0xF6: CPU_8BIT_OR(reg_AF.hi, 0, true) ; return 8 ; break ;

		// 8-bit XOR reg with reg
		case 0xAF: CPU_8BIT_XOR(reg_AF.hi, reg_AF.hi, false) ; return 4 ; break ;
		case 0xA8: CPU_8BIT_XOR(reg_AF.hi, reg_BC.hi, false) ; return 4 ; break ;
		case 0xA9: CPU_8BIT_XOR(reg_AF.hi, reg_BC.lo, false) ; return 4 ; break ;
		case 0xAA: CPU_8BIT_XOR(reg_AF.hi, reg_DE.hi, false) ; return 4 ; break ;
		case 0xAB: CPU_8BIT_XOR(reg_AF.hi, reg_DE.lo, false) ; return 4 ; break ;
		case 0xAC: CPU_8BIT_XOR(reg_AF.hi, reg_HL.hi, false) ; return 4 ; break ;
		case 0xAD: CPU_8BIT_XOR(reg_AF.hi, reg_HL.lo, false) ; return 4 ; break ;
		case 0xAE: CPU_8BIT_XOR(reg_AF.hi, readByte(reg_HL.reg), false) ; return 8 ; break ;
		case 0xEE: CPU_8BIT_XOR(reg_AF.hi, 0, true) ; return 8 ; break ;

		// 8-Bit compare
		case 0xBF: CPU_8BIT_COMPARE(reg_AF.hi, reg_AF.hi, false) ; return 4 ; break ;
		case 0xB8: CPU_8BIT_COMPARE(reg_AF.hi, reg_BC.hi, false) ; return 4 ; break ;
		case 0xB9: CPU_8BIT_COMPARE(reg_AF.hi, reg_BC.lo, false) ; return 4 ; break ;
		case 0xBA: CPU_8BIT_COMPARE(reg_AF.hi, reg_DE.hi, false) ; return 4 ; break ;
		case 0xBB: CPU_8BIT_COMPARE(reg_AF.hi, reg_DE.lo, false) ; return 4 ; break ;
		case 0xBC: CPU_8BIT_COMPARE(reg_AF.hi, reg_HL.hi, false) ; return 4 ; break ;
		case 0xBD: CPU_8BIT_COMPARE(reg_AF.hi, reg_HL.lo, false) ; return 4 ; break ;
		case 0xBE: CPU_8BIT_COMPARE(reg_AF.hi, readByte(reg_HL.reg), false) ; return 8 ; break ;
		case 0xFE: CPU_8BIT_COMPARE(reg_AF.hi, 0, true) ;  return 8 ; break ;

		// 8-bit inc
		case 0x3C: CPU_8BIT_INC(reg_AF.hi) ; return 4 ; break ;
		case 0x04: CPU_8BIT_INC(reg_BC.hi) ; return 4 ; break ;
		case 0x0C: CPU_8BIT_INC(reg_BC.lo) ; return 4 ; break ;
		case 0x14: CPU_8BIT_INC(reg_DE.hi) ; return 4 ; break ;
		case 0x1C: CPU_8BIT_INC(reg_DE.lo) ; return 4 ; break ;
		case 0x24: CPU_8BIT_INC(reg_HL.hi) ; return 4 ; break ;
		case 0x2C: CPU_8BIT_INC(reg_HL.lo) ; return 4 ; break ;
		case 0x34: CPU_8BIT_MEMORY_INC(reg_HL.reg) ; return 12 ; break ;

		// 8-bit dec
		case 0x3D: CPU_8BIT_DEC(reg_AF.hi) ; return 4 ; break ;
		case 0x05: CPU_8BIT_DEC(reg_BC.hi) ; return 4 ; break ;
		case 0x0D: CPU_8BIT_DEC(reg_BC.lo) ; return 4 ; break ;
		case 0x15: CPU_8BIT_DEC(reg_DE.hi) ; return 4 ; break ;
		case 0x1D: CPU_8BIT_DEC(reg_DE.lo) ; return 4 ; break ;
		case 0x25: CPU_8BIT_DEC(reg_HL.hi) ; return 4 ; break ;
		case 0x2D: CPU_8BIT_DEC(reg_HL.lo) ; return 4 ; break ;
		case 0x35: CPU_8BIT_MEMORY_DEC(reg_HL.reg) ; return 12 ; break ;

		// 16-bit add
		case 0x09: CPU_16BIT_ADD(reg_HL.reg,reg_BC.reg) ; return 8 ; break ;
		case 0x19: CPU_16BIT_ADD(reg_HL.reg,reg_DE.reg) ; return 8 ; break ;
		case 0x29: CPU_16BIT_ADD(reg_HL.reg,reg_HL.reg) ; return 8 ; break ;
		case 0x39: CPU_16BIT_ADD(reg_HL.reg,stack_Pointer.reg) ; return 8 ; break ;

		// inc 16-bit register
		case 0x03: CPU_16BIT_INC( reg_BC.reg) ; return 8 ; break ;
		case 0x13: CPU_16BIT_INC( reg_DE.reg) ; return 8 ; break ;
		case 0x23: CPU_16BIT_INC( reg_HL.reg) ; return 8 ; break ;
		case 0x33: CPU_16BIT_INC( stack_Pointer.reg) ; return 8 ; break ;

		// dec 16-bit register
		case 0x0B: CPU_16BIT_DEC( reg_BC.reg) ; return 8 ; break ;
		case 0x1B: CPU_16BIT_DEC( reg_DE.reg) ; return 8 ; break ;
		case 0x2B: CPU_16BIT_DEC( reg_HL.reg) ; return 8 ; break ;
		case 0x3B: CPU_16BIT_DEC( stack_Pointer.reg) ; return 8 ; break ;

		// jumps to nn
		case 0xE9: program_Counter = reg_HL.reg 		; return 4 	; break ;
		case 0xC3: CPU_JUMP_TO_N(false, 0, false) 		; return 12 ; break ;
		case 0xC2: CPU_JUMP_TO_N(true, FLAG_Z, false) 	; return 12 ; break ;
		case 0xCA: CPU_JUMP_TO_N(true, FLAG_Z, true) 	; return 12 ; break ;
		case 0xD2: CPU_JUMP_TO_N(true, FLAG_C, false) 	; return 12 ; break ;
		case 0xDA: CPU_JUMP_TO_N(true, FLAG_C, true) 	; return 12 ; break ;

		// jump by nn
		case 0x18 : CPU_JUMP_BY_N( false, 0, false) 	; return 8 ; break ;
		case 0x20 : CPU_JUMP_BY_N( true, FLAG_Z, false)	; return 8 ; break ;
		case 0x28 : CPU_JUMP_BY_N( true, FLAG_Z, true) 	; return 8 ; break ;
		case 0x30 : CPU_JUMP_BY_N( true, FLAG_C, false) ; return 8 ; break ;
		case 0x38 : CPU_JUMP_BY_N( true, FLAG_C, true) 	; return 8 ; break ;

		// calls
		case 0xCD : CPU_CALL( false, 0, false) 			; return 12; break ;
		case 0xC4 : CPU_CALL( true, FLAG_Z, false) 		; return 12; break ;
		case 0xCC : CPU_CALL( true, FLAG_Z, true) 		; return 12; break ;
		case 0xD4 : CPU_CALL( true, FLAG_C, false) 		; return 12; break ;
		case 0xDC : CPU_CALL( true, FLAG_C, true) 		; return 12; break ;

		// returns
		case 0xC9: CPU_RETURN( false, 0, false ) 		; return 8; break ;
		case 0xC0: CPU_RETURN( true, FLAG_Z, false ) 	; return 8; break ;
		case 0xC8: CPU_RETURN( true, FLAG_Z, true ) 	; return 8; break ;
		case 0xD0: CPU_RETURN( true, FLAG_C, false ) 	; return 8; break ;
		case 0xD8: CPU_RETURN( true, FLAG_C, true ) 	; return 8; break ;


		// restarts
		case 0xC7: CPU_RESTART( 0x00 ) ; return 32; break ;
		case 0xCF: CPU_RESTART( 0x08 ) ; return 32; break ;
		case 0xD7: CPU_RESTART( 0x10 ) ; return 32; break ;
		case 0xDF: CPU_RESTART( 0x18 ) ; return 32; break ;
		case 0xE7: CPU_RESTART( 0x20 ) ; return 32; break ;
		case 0xEF: CPU_RESTART( 0x28 ) ; return 32; break ;
		case 0xF7: CPU_RESTART( 0x30 ) ; return 32; break ;
		case 0xFF: CPU_RESTART( 0x38 ) ; return 32; break ;

		//Decimal Adjust
		case 0x27: CPU_DAA( ) ; return 4 ; break ;

		// handle the extended opcodes
		case 0xCB: return ExecuteExtendedOpcode( ) ; break ;

		// Rotation Instructions
		case 0x07:CPU_RLC(reg_AF.hi) ; return 4; break ;
		case 0x0F:CPU_RRC(reg_AF.hi) ; return 4; break ;
		case 0x17:CPU_RL(reg_AF.hi)  ; return 4; break ;
		case 0x1F:CPU_RR(reg_AF.hi)  ; return 4; break ;

		//Pop two bytes, Jump to that address and enable interrupts
		case 0xD9:
		{
			program_Counter = PopWordOffStack( ) ;
			masterInterrupt = true ;
			return 8;
			// LogMessage::GetSingleton()->DoLogMessage("Returning from interupt", false) ;
		}break ;

		//LD (nn), SP
		case 0x08:
		{
			WORD word = readWord() ;
			program_Counter+=2 ;
			writeMemory(word, stack_Pointer.lo) ;
			writeMemory((word >> 8), stack_Pointer.hi) ;
			return 20;
		}break ;

		//LD (HL), n
		case 0x36:
		{
			BYTE n = readByte(program_Counter) ;
			program_Counter++;
			WRITE_BYTE(reg_HL.reg, n) ;
			return 12;
		}break ;

		//LD A, (nn)
		case 0xFA:
		{
			WORD nn = readWord( ) ;
			program_Counter+=2 ;
			BYTE n = readByte(nn) ;
			reg_AF.hi = n ;
			return 16;
		}break ;

		// LD A, #
		case 0x3E:
		{
			BYTE n = readByte(program_Counter) ;
			program_Counter++ ;
			reg_AF.hi = n;
			return 8;
		}break ;
		case 0xEA:
		{
			WORD nn = readWord( ) ;
			program_Counter+=2 ;
			WRITE_BYTE(nn, reg_AF.hi) ;
			return 16;
		}break ;

		//Disable interrupts but only after the next instruction(The one after F3) is executed
		case 0xF3:
		{
			pendingInteruptDisabled = true ;
			return 4;
		}break ;

		//Enable interrupts but only after the next instruction(The one after F3) is executed
		case 0xFB:
		{
			pendingInteruptEnabled = true ;
			return 4;
		}break ;

        //LDH n,A
		case 0xE0:
		{
			BYTE n = readByte(program_Counter) ;
			program_Counter++ ;
			WORD address = 0xFF00 + n ;
			WRITE_BYTE(address, reg_AF.hi) ;
			return 12;
		}break ;

        //LDH A,n
		case 0xF0:
		{
			BYTE n = readByte(program_Counter) ;
			program_Counter++ ;
			WORD address = 0xFF00 + n ;
			reg_AF.hi = readByte( address ) ;
			return 12;
		}break ;

		//Complement Register A
		case 0x2F:
		{
			reg_AF.hi ^= 0xFF;

			reg_AF.lo = SetBit(reg_AF.lo, FLAG_N) ;
			reg_AF.lo = SetBit(reg_AF.lo, FLAG_H) ;
			return 4;
		}break ;

		//Low Power Mode. Do not execute any instructions until after an interrupt occurs
		case 0x76:
		{
			//LOGMESSAGE(Logging::MSG_INFO, "Halting cpu") ;
			halted = true ;
			return 4;
		}break ;

		//Complement Carry Flag
		case 0x3F:
		{
			cyclesThisUpdate += 4 ;
			if (TestBit(reg_AF.lo, FLAG_C))
				reg_AF.lo = ResetBit(reg_AF.lo, FLAG_C) ;
			else
				reg_AF.lo = SetBit(reg_AF.lo, FLAG_C) ;

			reg_AF.lo = ResetBit(reg_AF.lo, FLAG_H) ;
			reg_AF.lo = ResetBit(reg_AF.lo, FLAG_N) ;

			return 4;
		}break ;

		//Set Carry Flag
		case 0x37:
		{
			reg_AF.lo = SetBit(reg_AF.lo, FLAG_C);
			reg_AF.lo = ResetBit(reg_AF.lo, FLAG_H);
			reg_AF.lo = ResetBit(reg_AF.lo, FLAG_N);

			return 4;
		} break ;

		//Put SP + n into address HL
		case 0xF8:
		{
			SIGNED_BYTE n = readByte(program_Counter) ;
			program_Counter++ ;
			reg_AF.lo = ResetBit(reg_AF.lo, FLAG_Z);
			reg_AF.lo = ResetBit(reg_AF.lo, FLAG_N);


			WORD value = (stack_Pointer.reg + n) & 0xFFFF;

			reg_HL.reg = value ;
			unsigned int v = stack_Pointer.reg + n ;

			//overflow imminent if n is greater than 0xFFFF
			if( v > 0xFFFF )
				reg_AF.lo = SetBit(reg_AF.lo,FLAG_C) ;
			else
				reg_AF.lo = ResetBit(reg_AF.lo,FLAG_C) ;

			//half overflow imminent
			if( (stack_Pointer.reg & 0xF) + (v & 0xF) > 0xF )
				reg_AF.lo = SetBit(reg_AF.lo,FLAG_H) ;
			else
				reg_AF.lo = ResetBit(reg_AF.lo,FLAG_H) ;

			return 12;
		}break ;

		//Halt the CPU & LCD Display until button is pressed. 
		//We can do this by simply incrementing the program counter
		case 0x10:
		{
			program_Counter++ ;
			return 4 ;
		}break ;

		//Unhandled Opcode
		default:
		{
			char buffer[200] ;
			int result = std::snprintf(buffer, sizeof(buffer), "Unhandled Opcode %x", opcode);
			if(result >= 0 && result < static_cast<int>(sizeof(buffer)))
			{
				LogMessage::getLogMsgInstance()->writeToLog(buffer);
			}

			else
			{
				char errormsg[50];
				std::snprintf(errormsg, sizeof(errormsg), "Not enough space, Increase the space for buffer");
				LogMessage::getLogMsgInstance()->writeToLog(errormsg);
			}
			assert(false) ;
		} break;
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

int Emulator::ExecuteExtendedOpcode( )
{
	BYTE opcode = internal_Memory[program_Counter] ;

	if ((program_Counter >= 0x4000 && program_Counter <= 0x7FFF) || (program_Counter >= 0xA000 && program_Counter <= 0xBFFF))
		opcode = readByte(program_Counter) ;

	if (false)
	{
		char buffer[200] ;
		int result = std::snprintf(buffer, sizeof(buffer), "EXTENDEDOP = %x PC = %x\n", opcode, program_Counter);
		if(result >= 0 && result < static_cast<int>(sizeof(buffer)))
		{
			LogMessage::getLogMsgInstance()->writeToLog(buffer);
		}

		else
		{
			char errormsg[50];
			std::snprintf(errormsg, sizeof(errormsg), "Not enough space, Increase the space for buffer");
			LogMessage::getLogMsgInstance()->writeToLog(errormsg);
		}
		
	}

	program_Counter++ ;

	//LOGMESSAGE(Logging::MSG_INFO, STR::Format("Processing Extended Opcode %x, Program Counter: %x", opcode, program_Counter).ConstCharPtr()) ;

	switch(opcode)
	{
		// rotate left through carry
  		case 0x0 : CPU_RLC(reg_BC.hi) ; return 8 ; break ;
  		case 0x1 : CPU_RLC(reg_BC.lo) ; return 8 ; break ;
  		case 0x2 : CPU_RLC(reg_DE.hi) ; return 8 ; break ;
  		case 0x3 : CPU_RLC(reg_DE.lo) ; return 8 ; break ;
  		case 0x4 : CPU_RLC(reg_HL.hi) ; return 8 ; break ;
  		case 0x5 : CPU_RLC(reg_HL.lo) ; return 8 ; break ;
  		case 0x6 : CPU_RLC_MEMORY(reg_HL.reg) ; return 16 ; break ;
  		case 0x7 : CPU_RLC(reg_AF.hi) ; return 8; break ;

  		// rotate right through carry
  		case 0x8 : CPU_RRC(reg_BC.hi) ; return 8 ; break ;
  		case 0x9 : CPU_RRC(reg_BC.lo) ; return 8 ; break ;
  		case 0xA : CPU_RRC(reg_DE.hi) ; return 8 ; break ;
  		case 0xB : CPU_RRC(reg_DE.lo) ; return 8 ; break ;
  		case 0xC : CPU_RRC(reg_HL.hi) ; return 8 ; break ;
  		case 0xD : CPU_RRC(reg_HL.lo) ; return 8 ; break ;
  		case 0xE : CPU_RRC_MEMORY(reg_HL.reg) ; return 16 ; break ;
  		case 0xF : CPU_RRC(reg_AF.hi) ; return 8; break ;

  		// rotate left
  		case 0x10: CPU_RL(reg_BC.hi) ; return 8 ; break;
  		case 0x11: CPU_RL(reg_BC.lo) ; return 8 ; break;
	  	case 0x12: CPU_RL(reg_DE.hi) ; return 8 ; break;
  		case 0x13: CPU_RL(reg_DE.lo) ; return 8 ; break;
  		case 0x14: CPU_RL(reg_HL.hi) ; return 8 ; break;
  		case 0x15: CPU_RL(reg_HL.lo) ; return 8 ; break;
  		case 0x16: CPU_RL_MEMORY(reg_HL.reg) ; return 16 ; break;
  		case 0x17: CPU_RL(reg_AF.hi) ; return 8 ; break;

  		// rotate right
  		case 0x18: CPU_RR(reg_BC.hi) ; return 8 ; break;
  		case 0x19: CPU_RR(reg_BC.lo) ; return 8 ; break;
  		case 0x1A: CPU_RR(reg_DE.hi) ; return 8 ; break;
  		case 0x1B: CPU_RR(reg_DE.lo) ; return 8 ; break;
  		case 0x1C: CPU_RR(reg_HL.hi) ; return 8 ; break;
  		case 0x1D: CPU_RR(reg_HL.lo) ; return 8 ; break;
  		case 0x1E: CPU_RR_MEMORY(reg_HL.reg) ; return 16 ; break;
  		case 0x1F: CPU_RR(reg_AF.hi) ; return 8 ; break;

  		case 0x20 : CPU_SLA( reg_BC.hi ) ; return 8 ; break ;
  		case 0x21 : CPU_SLA( reg_BC.lo ) ; return 8 ; break ;
  		case 0x22 : CPU_SLA( reg_DE.hi ) ; return 8 ; break ;
	  	case 0x23 : CPU_SLA( reg_DE.lo ) ; return 8 ; break ;
  		case 0x24 : CPU_SLA( reg_HL.hi ) ; return 8 ; break ;
  		case 0x25 : CPU_SLA( reg_HL.lo ) ; return 8 ; break ;
  		case 0x26 : CPU_SLA_MEMORY( reg_HL.reg ) ; return 16 ; break ;
		case 0x27 : CPU_SLA( reg_AF.hi ) ; return 8 ; break ;

  		case 0x28 : CPU_SRA( reg_BC.hi ) ; return 8 ; break ;
  		case 0x29 : CPU_SRA( reg_BC.lo ) ; return 8 ; break ;
  		case 0x2A : CPU_SRA( reg_DE.hi ) ; return 8 ; break ;
  		case 0x2B : CPU_SRA( reg_DE.lo ) ; return 8 ; break ;
  		case 0x2C : CPU_SRA( reg_HL.hi ) ; return 8 ; break ;
  		case 0x2D : CPU_SRA( reg_HL.lo ) ; return 8 ; break ;
  		case 0x2E : CPU_SRA_MEMORY( reg_HL.reg ) ; return 16 ; break ;
  		case 0x2F : CPU_SRA( reg_AF.hi ) ; return 8 ; break ;

  		case 0x38 : CPU_SRL( reg_BC.hi ) ; return 8 ; break ;
  		case 0x39 : CPU_SRL( reg_BC.lo ) ; return 8 ; break ;
  		case 0x3A : CPU_SRL( reg_DE.hi ) ; return 8 ; break ;
  		case 0x3B : CPU_SRL( reg_DE.lo ) ; return 8 ; break ;
  		case 0x3C : CPU_SRL( reg_HL.hi ) ; return 8 ; break ;
  		case 0x3D : CPU_SRL( reg_HL.lo ) ; return 8 ; break ;
  		case 0x3E : CPU_SRL_MEMORY( reg_HL.reg ) ; return 16 ; break ;
	  	case 0x3F : CPU_SRL( reg_AF.hi ) ; return 8 ; break ;

		// swap nibbles
		case 0x37 : CPU_SWAP_NIBBLES( reg_AF.hi ) ; return 8 ; break ;
		case 0x30 : CPU_SWAP_NIBBLES( reg_BC.hi ) ; return 8 ; break ;
		case 0x31 : CPU_SWAP_NIBBLES( reg_BC.lo ) ; return 8 ; break ;
		case 0x32 : CPU_SWAP_NIBBLES( reg_DE.hi ) ; return 8 ; break ;
		case 0x33 : CPU_SWAP_NIBBLES( reg_DE.lo ) ; return 8 ; break ;
		case 0x34 : CPU_SWAP_NIBBLES( reg_HL.hi ) ; return 8 ; break ;
		case 0x35 : CPU_SWAP_NIBBLES( reg_HL.lo ) ; return 8 ; break ;
		case 0x36 : CPU_SWAP_NIBBLES_MEMORY( reg_HL.reg ) ; return 16 ; break ;

		// test bit
		case 0x40 : CPU_TEST_BIT( reg_BC.hi, 0 ) ; return 8 ; break ;
		case 0x41 : CPU_TEST_BIT( reg_BC.lo, 0 ) ; return 8 ; break ;
		case 0x42 : CPU_TEST_BIT( reg_DE.hi, 0 ) ; return 8 ; break ;
		case 0x43 : CPU_TEST_BIT( reg_DE.lo, 0 ) ; return 8 ; break ;
		case 0x44 : CPU_TEST_BIT( reg_HL.hi, 0 ) ; return 8 ; break ;
		case 0x45 : CPU_TEST_BIT( reg_HL.lo, 0 ) ; return 8 ; break ;
		case 0x46 : CPU_TEST_BIT( readByte(reg_HL.reg), 0) ; return 16 ; break ;
		case 0x47 : CPU_TEST_BIT( reg_AF.hi, 0 ) ; return 8 ; break ;
		case 0x48 : CPU_TEST_BIT( reg_BC.hi, 1 ) ; return 8 ; break ;
		case 0x49 : CPU_TEST_BIT( reg_BC.lo, 1 ) ; return 8 ; break ;
		case 0x4A : CPU_TEST_BIT( reg_DE.hi, 1 ) ; return 8 ; break ;
		case 0x4B : CPU_TEST_BIT( reg_DE.lo, 1 ) ; return 8 ; break ;
		case 0x4C : CPU_TEST_BIT( reg_HL.hi, 1 ) ; return 8 ; break ;
		case 0x4D : CPU_TEST_BIT( reg_HL.lo, 1 ) ; return 8 ; break ;
		case 0x4E : CPU_TEST_BIT( readByte(reg_HL.reg), 1) ; return 16 ; break ;
		case 0x4F : CPU_TEST_BIT( reg_AF.hi, 1 ) ; return 8 ; break ;
		case 0x50 : CPU_TEST_BIT( reg_BC.hi, 2 ) ; return 8 ; break ;
		case 0x51 : CPU_TEST_BIT( reg_BC.lo, 2 ) ; return 8 ; break ;
		case 0x52 : CPU_TEST_BIT( reg_DE.hi, 2 ) ; return 8 ; break ;
		case 0x53 : CPU_TEST_BIT( reg_DE.lo, 2 ) ; return 8 ; break ;
		case 0x54 : CPU_TEST_BIT( reg_HL.hi, 2 ) ; return 8 ; break ;
		case 0x55 : CPU_TEST_BIT( reg_HL.lo, 2 ) ; return 8 ; break ;
		case 0x56 : CPU_TEST_BIT( readByte(reg_HL.reg), 2 ) ; return 16 ; break ;
		case 0x57 : CPU_TEST_BIT( reg_AF.hi, 2 ) ; return 8 ; break ;
		case 0x58 : CPU_TEST_BIT( reg_BC.hi, 3 ) ; return 8 ; break ;
		case 0x59 : CPU_TEST_BIT( reg_BC.lo, 3 ) ; return 8 ; break ;
		case 0x5A : CPU_TEST_BIT( reg_DE.hi, 3 ) ; return 8 ; break ;
		case 0x5B : CPU_TEST_BIT( reg_DE.lo, 3 ) ; return 8 ; break ;
		case 0x5C : CPU_TEST_BIT( reg_HL.hi, 3 ) ; return 8 ; break ;
		case 0x5D : CPU_TEST_BIT( reg_HL.lo, 3 ) ; return 8 ; break ;
		case 0x5E : CPU_TEST_BIT( readByte(reg_HL.reg), 3) ; return 16 ; break ;
		case 0x5F : CPU_TEST_BIT( reg_AF.hi, 3 ) ; return 8 ; break ;
		case 0x60 : CPU_TEST_BIT( reg_BC.hi, 4 ) ; return 8 ; break ;
		case 0x61 : CPU_TEST_BIT( reg_BC.lo, 4 ) ; return 8 ; break ;
		case 0x62 : CPU_TEST_BIT( reg_DE.hi, 4 ) ; return 8 ; break ;
		case 0x63 : CPU_TEST_BIT( reg_DE.lo, 4 ) ; return 8 ; break ;
		case 0x64 : CPU_TEST_BIT( reg_HL.hi, 4 ) ; return 8 ; break ;
		case 0x65 : CPU_TEST_BIT( reg_HL.lo, 4 ) ; return 8 ; break ;
		case 0x66 : CPU_TEST_BIT( readByte(reg_HL.reg), 4) ; return 16 ; break ;
		case 0x67 : CPU_TEST_BIT( reg_AF.hi, 4 ) ; return 8 ; break ;
		case 0x68 : CPU_TEST_BIT( reg_BC.hi, 5 ) ; return 8 ; break ;
		case 0x69 : CPU_TEST_BIT( reg_BC.lo, 5 ) ; return 8 ; break ;
		case 0x6A : CPU_TEST_BIT( reg_DE.hi, 5 ) ; return 8 ; break ;
		case 0x6B : CPU_TEST_BIT( reg_DE.lo, 5 ) ; return 8 ; break ;
		case 0x6C : CPU_TEST_BIT( reg_HL.hi, 5 ) ; return 8 ; break ;
		case 0x6D : CPU_TEST_BIT( reg_HL.lo, 5 ) ; return 8 ; break ;
		case 0x6E : CPU_TEST_BIT( readByte(reg_HL.reg), 5) ; return 16 ; break ;
		case 0x6F : CPU_TEST_BIT( reg_AF.hi, 5 ) ; return 8 ; break ;
		case 0x70 : CPU_TEST_BIT( reg_BC.hi, 6 ) ; return 8 ; break ;
		case 0x71 : CPU_TEST_BIT( reg_BC.lo, 6 ) ; return 8 ; break ;
		case 0x72 : CPU_TEST_BIT( reg_DE.hi, 6 ) ; return 8 ; break ;
		case 0x73 : CPU_TEST_BIT( reg_DE.lo, 6 ) ; return 8 ; break ;
		case 0x74 : CPU_TEST_BIT( reg_HL.hi, 6 ) ; return 8 ; break ;
		case 0x75 : CPU_TEST_BIT( reg_HL.lo, 6 ) ; return 8 ; break ;
		case 0x76 : CPU_TEST_BIT( readByte(reg_HL.reg), 6) ; return 16 ; break ;
		case 0x77 : CPU_TEST_BIT( reg_AF.hi, 6 ) ; return 8 ; break ;
		case 0x78 : CPU_TEST_BIT( reg_BC.hi, 7 ) ; return 8 ; break ;
		case 0x79 : CPU_TEST_BIT( reg_BC.lo, 7 ) ; return 8 ; break ;
		case 0x7A : CPU_TEST_BIT( reg_DE.hi, 7 ) ; return 8 ; break ;
		case 0x7B : CPU_TEST_BIT( reg_DE.lo, 7 ) ; return 8 ; break ;
		case 0x7C : CPU_TEST_BIT( reg_HL.hi, 7 ) ; return 8 ; break ;
		case 0x7D : CPU_TEST_BIT( reg_HL.lo, 7 ) ; return 8 ; break ;
		case 0x7E : CPU_TEST_BIT( readByte(reg_HL.reg), 7 ) ; return 16 ; break ;
		case 0x7F : CPU_TEST_BIT( reg_AF.hi, 7 ) ; return 8 ; break ;

		// reset bit
		case 0x80 : CPU_RESET_BIT( reg_BC.hi, 0 ) ; return 8 ; break ;
		case 0x81 : CPU_RESET_BIT( reg_BC.lo, 0 ) ; return 8 ; break ;
		case 0x82 : CPU_RESET_BIT( reg_DE.hi, 0 ) ; return 8 ; break ;
		case 0x83 : CPU_RESET_BIT( reg_DE.lo, 0 ) ; return 8 ; break ;
		case 0x84 : CPU_RESET_BIT( reg_HL.hi, 0 ) ; return 8 ; break ;
		case 0x85 : CPU_RESET_BIT( reg_HL.lo, 0 ) ; return 8 ; break ;
		case 0x86 : CPU_RESET_BIT_MEMORY( reg_HL.reg, 0 ) ; return 16 ; break ;
		case 0x87 : CPU_RESET_BIT( reg_AF.hi, 0 ) ; return 8 ; break ;
		case 0x88 : CPU_RESET_BIT( reg_BC.hi, 1 ) ; return 8 ; break ;
		case 0x89 : CPU_RESET_BIT( reg_BC.lo, 1 ) ; return 8 ; break ;
		case 0x8A : CPU_RESET_BIT( reg_DE.hi, 1 ) ; return 8 ; break ;
		case 0x8B : CPU_RESET_BIT( reg_DE.lo, 1 ) ; return 8 ; break ;
		case 0x8C : CPU_RESET_BIT( reg_HL.hi, 1 ) ; return 8 ; break ;
		case 0x8D : CPU_RESET_BIT( reg_HL.lo, 1 ) ; return 8 ; break ;
		case 0x8E : CPU_RESET_BIT_MEMORY( reg_HL.reg, 1 ) ; return 16 ; break ;
		case 0x8F : CPU_RESET_BIT( reg_AF.hi, 1 ) ; return 8 ; break ;
		case 0x90 : CPU_RESET_BIT( reg_BC.hi, 2 ) ; return 8 ; break ;
		case 0x91 : CPU_RESET_BIT( reg_BC.lo, 2 ) ; return 8 ; break ;
		case 0x92 : CPU_RESET_BIT( reg_DE.hi, 2 ) ; return 8 ; break ;
		case 0x93 : CPU_RESET_BIT( reg_DE.lo, 2 ) ; return 8 ; break ;
		case 0x94 : CPU_RESET_BIT( reg_HL.hi, 2 ) ; return 8 ; break ;
		case 0x95 : CPU_RESET_BIT( reg_HL.lo, 2 ) ; return 8 ; break ;
		case 0x96 : CPU_RESET_BIT_MEMORY( reg_HL.reg, 2 ) ; return 16 ; break ;
		case 0x97 : CPU_RESET_BIT( reg_AF.hi, 2 ) ; return 8 ; break ;
		case 0x98 : CPU_RESET_BIT( reg_BC.hi, 3 ) ; return 8 ; break ;
		case 0x99 : CPU_RESET_BIT( reg_BC.lo, 3 ) ; return 8 ; break ;
		case 0x9A : CPU_RESET_BIT( reg_DE.hi, 3 ) ; return 8 ; break ;
		case 0x9B : CPU_RESET_BIT( reg_DE.lo, 3 ) ; return 8 ; break ;
		case 0x9C : CPU_RESET_BIT( reg_HL.hi, 3 ) ; return 8 ; break ;
		case 0x9D : CPU_RESET_BIT( reg_HL.lo, 3 ) ; return 8 ; break ;
		case 0x9E : CPU_RESET_BIT_MEMORY( reg_HL.reg, 3  ) ; return 16 ; break ;
		case 0x9F : CPU_RESET_BIT( reg_AF.hi, 3 ) ; return 8 ; break ;
		case 0xA0 : CPU_RESET_BIT( reg_BC.hi, 4 ) ; return 8 ; break ;
		case 0xA1 : CPU_RESET_BIT( reg_BC.lo, 4 ) ; return 8 ; break ;
		case 0xA2 : CPU_RESET_BIT( reg_DE.hi, 4 ) ; return 8 ; break ;
		case 0xA3 : CPU_RESET_BIT( reg_DE.lo, 4 ) ; return 8 ; break ;
		case 0xA4 : CPU_RESET_BIT( reg_HL.hi, 4 ) ; return 8 ; break ;
		case 0xA5 : CPU_RESET_BIT( reg_HL.lo, 4 ) ; return 8 ; break ;
		case 0xA6 : CPU_RESET_BIT_MEMORY( reg_HL.reg, 4) ; return 16 ; break ;
		case 0xA7 : CPU_RESET_BIT( reg_AF.hi, 4 ) ; return 8 ; break ;
		case 0xA8 : CPU_RESET_BIT( reg_BC.hi, 5 ) ; return 8 ; break ;
		case 0xA9 : CPU_RESET_BIT( reg_BC.lo, 5 ) ; return 8 ; break ;
		case 0xAA : CPU_RESET_BIT( reg_DE.hi, 5 ) ; return 8 ; break ;
		case 0xAB : CPU_RESET_BIT( reg_DE.lo, 5 ) ; return 8 ; break ;
		case 0xAC : CPU_RESET_BIT( reg_HL.hi, 5 ) ; return 8 ; break ;
		case 0xAD : CPU_RESET_BIT( reg_HL.lo, 5 ) ; return 8 ; break ;
		case 0xAE : CPU_RESET_BIT_MEMORY( reg_HL.reg, 5 ) ; return 16 ; break ;
		case 0xAF : CPU_RESET_BIT( reg_AF.hi, 5 ) ; return 8 ; break ;
		case 0xB0 : CPU_RESET_BIT( reg_BC.hi, 6 ) ; return 8 ; break ;
		case 0xB1 : CPU_RESET_BIT( reg_BC.lo, 6 ) ; return 8 ; break ;
		case 0xB2 : CPU_RESET_BIT( reg_DE.hi, 6 ) ; return 8 ; break ;
		case 0xB3 : CPU_RESET_BIT( reg_DE.lo, 6 ) ; return 8 ; break ;
		case 0xB4 : CPU_RESET_BIT( reg_HL.hi, 6 ) ; return 8 ; break ;
		case 0xB5 : CPU_RESET_BIT( reg_HL.lo, 6 ) ; return 8 ; break ;
		case 0xB6 : CPU_RESET_BIT_MEMORY( reg_HL.reg, 6 ) ; return 16 ; break ;
		case 0xB7 : CPU_RESET_BIT( reg_AF.hi, 6 ) ; return 8 ; break ;
		case 0xB8 : CPU_RESET_BIT( reg_BC.hi, 7 ) ; return 8 ; break ;
		case 0xB9 : CPU_RESET_BIT( reg_BC.lo, 7 ) ; return 8 ; break ;
		case 0xBA : CPU_RESET_BIT( reg_DE.hi, 7 ) ; return 8 ; break ;
		case 0xBB : CPU_RESET_BIT( reg_DE.lo, 7 ) ; return 8 ; break ;
		case 0xBC : CPU_RESET_BIT( reg_HL.hi, 7 ) ; return 8 ; break ;
		case 0xBD : CPU_RESET_BIT( reg_HL.lo, 7 ) ; return 8 ; break ;
		case 0xBE : CPU_RESET_BIT_MEMORY( reg_HL.reg, 7 ) ; return 16 ; break ;
		case 0xBF : CPU_RESET_BIT( reg_AF.hi, 7 ) ; return 8 ; break ;


		// set bit
		case 0xC0 : CPU_SET_BIT( reg_BC.hi, 0 ) ; return 8 ; break ;
		case 0xC1 : CPU_SET_BIT( reg_BC.lo, 0 ) ; return 8 ; break ;
		case 0xC2 : CPU_SET_BIT( reg_DE.hi, 0 ) ; return 8 ; break ;
		case 0xC3 : CPU_SET_BIT( reg_DE.lo, 0 ) ; return 8 ; break ;
		case 0xC4 : CPU_SET_BIT( reg_HL.hi, 0 ) ; return 8 ; break ;
		case 0xC5 : CPU_SET_BIT( reg_HL.lo, 0 ) ; return 8 ; break ;
		case 0xC6 : CPU_SET_BIT_MEMORY( reg_HL.reg, 0 ) ; return 16 ; break ;
		case 0xC7 : CPU_SET_BIT( reg_AF.hi, 0 ) ; return 8 ; break ;
		case 0xC8 : CPU_SET_BIT( reg_BC.hi, 1 ) ; return 8 ; break ;
		case 0xC9 : CPU_SET_BIT( reg_BC.lo, 1 ) ; return 8 ; break ;
		case 0xCA : CPU_SET_BIT( reg_DE.hi, 1 ) ; return 8 ; break ;
		case 0xCB : CPU_SET_BIT( reg_DE.lo, 1 ) ; return 8 ; break ;
		case 0xCC : CPU_SET_BIT( reg_HL.hi, 1 ) ; return 8 ; break ;
		case 0xCD : CPU_SET_BIT( reg_HL.lo, 1 ) ; return 8 ; break ;
		case 0xCE : CPU_SET_BIT_MEMORY( reg_HL.reg, 1 ) ; return 16 ; break ;
		case 0xCF : CPU_SET_BIT( reg_AF.hi, 1 ) ; return 8 ; break ;
		case 0xD0 : CPU_SET_BIT( reg_BC.hi, 2 ) ; return 8 ; break ;
		case 0xD1 : CPU_SET_BIT( reg_BC.lo, 2 ) ; return 8 ; break ;
		case 0xD2 : CPU_SET_BIT( reg_DE.hi, 2 ) ; return 8 ; break ;
		case 0xD3 : CPU_SET_BIT( reg_DE.lo, 2 ) ; return 8 ; break ;
		case 0xD4 : CPU_SET_BIT( reg_HL.hi, 2 ) ; return 8 ; break ;
		case 0xD5 : CPU_SET_BIT( reg_HL.lo, 2 ) ; return 8 ; break ;
		case 0xD6 : CPU_SET_BIT_MEMORY( reg_HL.reg, 2 ) ; return 16 ; break ;
		case 0xD7 : CPU_SET_BIT( reg_AF.hi, 2 ) ; return 8 ; break ;
		case 0xD8 : CPU_SET_BIT( reg_BC.hi, 3 ) ; return 8 ; break ;
		case 0xD9 : CPU_SET_BIT( reg_BC.lo, 3 ) ; return 8 ; break ;
		case 0xDA : CPU_SET_BIT( reg_DE.hi, 3 ) ; return 8 ; break ;
		case 0xDB : CPU_SET_BIT( reg_DE.lo, 3 ) ; return 8 ; break ;
		case 0xDC : CPU_SET_BIT( reg_HL.hi, 3 ) ; return 8 ; break ;
		case 0xDD : CPU_SET_BIT( reg_HL.lo, 3 ) ; return 8 ; break ;
		case 0xDE : CPU_SET_BIT_MEMORY( reg_HL.reg, 3  ) ; return 16 ; break ;
		case 0xDF : CPU_SET_BIT( reg_AF.hi, 3 ) ; return 8 ; break ;
		case 0xE0 : CPU_SET_BIT( reg_BC.hi, 4 ) ; return 8 ; break ;
		case 0xE1 : CPU_SET_BIT( reg_BC.lo, 4 ) ; return 8 ; break ;
		case 0xE2 : CPU_SET_BIT( reg_DE.hi, 4 ) ; return 8 ; break ;
		case 0xE3 : CPU_SET_BIT( reg_DE.lo, 4 ) ; return 8 ; break ;
		case 0xE4 : CPU_SET_BIT( reg_HL.hi, 4 ) ; return 8 ; break ;
		case 0xE5 : CPU_SET_BIT( reg_HL.lo, 4 ) ; return 8 ; break ;
		case 0xE6 : CPU_SET_BIT_MEMORY( reg_HL.reg, 4) ; return 16 ; break ;
		case 0xE7 : CPU_SET_BIT( reg_AF.hi, 4 ) ; return 8 ; break ;
		case 0xE8 : CPU_SET_BIT( reg_BC.hi, 5 ) ; return 8 ; break ;
		case 0xE9 : CPU_SET_BIT( reg_BC.lo, 5 ) ; return 8 ; break ;
		case 0xEA : CPU_SET_BIT( reg_DE.hi, 5 ) ; return 8 ; break ;
		case 0xEB : CPU_SET_BIT( reg_DE.lo, 5 ) ; return 8 ; break ;
		case 0xEC : CPU_SET_BIT( reg_HL.hi, 5 ) ; return 8 ; break ;
		case 0xED : CPU_SET_BIT( reg_HL.lo, 5 ) ; return 8 ; break ;
		case 0xEE : CPU_SET_BIT_MEMORY( reg_HL.reg, 5 ) ; return 16 ; break ;
		case 0xEF : CPU_SET_BIT( reg_AF.hi, 5 ) ; return 8 ; break ;
		case 0xF0 : CPU_SET_BIT( reg_BC.hi, 6 ) ; return 8 ; break ;
		case 0xF1 : CPU_SET_BIT( reg_BC.lo, 6 ) ; return 8 ; break ;
		case 0xF2 : CPU_SET_BIT( reg_DE.hi, 6 ) ; return 8 ; break ;
		case 0xF3 : CPU_SET_BIT( reg_DE.lo, 6 ) ; return 8 ; break ;
		case 0xF4 : CPU_SET_BIT( reg_HL.hi, 6 ) ; return 8 ; break ;
		case 0xF5 : CPU_SET_BIT( reg_HL.lo, 6 ) ; return 8 ; break ;
		case 0xF6 : CPU_SET_BIT_MEMORY( reg_HL.reg, 6 ) ; return 16 ; break ;
		case 0xF7 : CPU_SET_BIT( reg_AF.hi, 6 ) ; return 8 ; break ;
		case 0xF8 : CPU_SET_BIT( reg_BC.hi, 7 ) ; return 8 ; break ;
		case 0xF9 : CPU_SET_BIT( reg_BC.lo, 7 ) ; return 8 ; break ;
		case 0xFA : CPU_SET_BIT( reg_DE.hi, 7 ) ; return 8 ; break ;
		case 0xFB : CPU_SET_BIT( reg_DE.lo, 7 ) ; return 8 ; break ;
		case 0xFC : CPU_SET_BIT( reg_HL.hi, 7 ) ; return 8 ; break ;
		case 0xFD : CPU_SET_BIT( reg_HL.lo, 7 ) ; return 8 ; break ;
		case 0xFE : CPU_SET_BIT_MEMORY( reg_HL.reg, 7 ) ; return 16 ; break ;
		case 0xFF : CPU_SET_BIT( reg_AF.hi, 7 ) ; return 8 ; break ;


		default:
		{
			char buffer[200] ;
			int result = std::snprintf(buffer, sizeof(buffer), "Unhandled Extended Opcode %x\n", opcode);
			if(result >= 0 && result < static_cast<int>(sizeof(buffer)))
			{
				LogMessage::getLogMsgInstance()->writeToLog(buffer);
			}

			else
			{
				char errormsg[50];
				std::snprintf(errormsg, sizeof(errormsg), "Not enough space, Increase the space for buffer");
				LogMessage::getLogMsgInstance()->writeToLog(errormsg);
			}

			assert(false) ;
		} break;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
