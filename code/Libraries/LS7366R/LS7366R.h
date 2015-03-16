// ALPHA RELEASE
// prd005

#ifndef LS7366R_H
#define LS7366R_H

#include <SPI.h>
#include "Arduino.h"

// MR0 values ===========================================
#define NON_QUAD 0x00 // x x xx xx 00
#define X1_QUAD 0x01 // x x xx xx 01
#define X2_QUAD 0x02 // x x xx xx 10
#define X4_QUAD 0x03 // x x xx xx 11

#define FREE_RUN 0x00 		// x x xx 00 xx
#define SINGLE_CYCLE 0x04 	// x x xx 01 xx
#define RANGE_LIMIT 0x08	// x x xx 10 xx
#define MOD_N_CNT 0x0C		// x x xx 11 xx

#define DISABLE_IDX 0x00	// x x 00 xx xx
#define LOAD_CNTR 0x10 		// x x 01 xx xx
#define RST_CNTR 0x20 		// x x 10 xx xx
#define LOAD_OTR 0x30		// x x 11 xx xx

#define ASYN_IDX 0x00 	// x 0 xx xx xx
#define SYN_IDX 0x40 	// x 1 xx xx xx

#define CLK_DIV_1 0x00 // 0 x xx xx xx
#define CLK_DIV_2 0x80 // 1 x xx xx xx
// ======================================================

// MR1 values ===========================================
#define FOUR_BYTE   0x00    // x x x x x x 00
#define THREE_BYTE  0x01    // x x x x x x 01
#define TWO_BYTE    0x02    // x x x x x x 10
#define ONE_BYTE    0x03    // x x x x x x 11

#define ENABLE_CNT  0x00  // x x x x x 0 xx
#define DISABLE_CNT 0x40  // x x x x x 1 xx

// !LFLAG on Index Pulse
#define FLAG_IDX 0x10  // x x x 1 x x xx
// !LFLAG on Compare (CNTR = DTR)
#define FLAG_CMP 0x20  // x x 1 x x x xx
// !LFLAG on Borrow (Underflow, - -> +)
#define FLAG_BW 0x40   // x 1 x x x x xx
// !LFLAG on Carry-over (Overflow, + -> -)
#define FLAG_CY 0x80   // 1 x x x x x xx
// ======================================================

// STR values ===========================================
// Carry-Over
#define CY_MASK 0x80
// Borrow
#define BW_MASK 0x40
// Compare (CNTR = DTR)
#define CMP_MASK 0x20
// Index
#define IDX_MASK 0x10
// Count Enable Status
#define CEN_MASK 0x08
// Power Loss Indicator
#define PLS_MASK 0x04
// Count Direction Indicator, 0 down, 1 up
#define UD_MASK 0x02
// Sign bit, I really haven't figured this one out yet. 
// The datasheet is rather ambiguous since the counter is unsigned.
#define S_MASK 0x01
// ======================================================

class LS7366R
{
  public:
    /*
    ** Constructor to Init Encoder with default settings
    ** Clock Division = 0, Free Run Counting, 4 byte counter, 4x counting, 
    **
    ** param1: Chip Select Pin
    ** param2: Count Enable Pin
    **
    ** LS7366R enc(SSP, CNT_EN);
    */
    LS7366R(int, int);

    /*
    ** Constructor to Init Encoder with custom settings
    **
    ** param1: Chip Select Pin
    ** param2: Count Enable Pin
    ** param3: MR0 Settings
    ** param4: MR1 Settings
    **
    ** See above MR0 and MR1 values valid settings
    ** Load settings using bit-wise OR i.e. 
    ** LS7366R enc(SSP, CNT_EN, X4_QUAD | FREE_RUN ,FOUR_BYTE | FLAG_IDX | FLAG_CY | FLAG_BW);
    */
    LS7366R(int, int, byte, byte);

    /*
    ** Reads the current encoder count
    **
    ** return: current encoder count as an unsigned long
    */
    unsigned long readEncoder();	
    
    /*
    ** Clears the current encoder count, reset to 0
    */    
    void clearEncoder();

    /*
    ** Enables the encoder counter
    */
    void enableEncoder();

    /*
    ** Disables the encoder counter
    */
    void disableEncoder();

    /*
    ** Writes a value to the DTR Register
    **
    ** param: value to write to DTR
    */
    void writeDTR(unsigned long);

    /*
    ** Reads then clears the encoder status register. This gives the interrupt
    ** source if interrupts are enabled and then clears the latching interrupt
    **
    ** return: status register contents
    */
    byte readSTR();
    
	// =======================================================================
	/*
    ** I'm working on it...
    **
    ** return: SIGNED!!!! long
    */	
	long s_readEncoder(); // Keep this function?
    // unsigned long readOTR();
	// void setCounter(unsigned long);
	// =======================================================================
	
  private:
    int _SS;
    int _EN;
	int _bytes;
};

#endif /* LS7366R_H */
