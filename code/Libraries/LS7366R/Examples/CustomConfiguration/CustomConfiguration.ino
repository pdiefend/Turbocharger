// ALPHA RELEASE
// prd005

/*
** Include the SPI and LS7366R Libraries. Even though we aren't
** directly using SPI, we must include it for the LS7366R
*/
#include <SPI.h>
#include "LS7366R.h"

/* Pin definitions for location of connections
** Based on the Bucknell University Turbocharger driver board
*/
#define SS1 4
#define CNT_EN 6
#define LFLAG_VECT 0 // Interrupt Vector for Pin 2 is 0

/*
** Configure MDR0 with 4x Quadrature Counting, Free Run, and no
** Clock Division
*/
#define MDR0 X4_QUAD | FREE_RUN | CLK_DIV_1

/*
** Configure MDR1 with 2-Byte width and interrupts on Index,
** Borrow (Undeflow) and Carry-over (Overflow)
*/
#define MDR1 TWO_BYTE | FLAG_IDX | FLAG_BW | FLAG_CY

/* 
** Create an encoder object with the pin locations and
** configuration we created above
** Usage is LS7366R <object name>(<chip select pin>, 
**   <count enable pin>, <MDR0 settings>, <MDR1 settings>);
*/
LS7366R enc1(SS1, CNT_EN, MDR0, MDR1);

// other global variables we need for this example code
volatile byte flag = 0;

void setup(){
  Serial.begin(115200);
  Serial.println("Begin");
  // first we clear the interrupt register after init
  enc1.readSTR();
  // enable the encoder counter
  enc1.enableEncoder();
  // reset the count to zero, sometimes things twitch are startup
  // so reset to 0 when we're ready to start.
  enc1.clearEncoder();
  
  // Finally attach an interrupt to the LFLAG pin
  attachInterrupt(LFLAG_VECT, ISR_ENC, FALLING);
}

void loop(){
  // If the ISR detected an interrupt, handle it
  if(flag){
    // read (then clear) the status register
    byte stat = enc1.readSTR();
    
    Serial.print("INT EVENT: ");
    
    // Compare the status register to the Carryover bit-mask to
    // determine if it was a carryover event
    if(stat & CY_MASK){
      // code incase it was a carryover event
      Serial.println("Carryover Event");
    }
    
    // Compare the status register to the Borrow bit-mask to
    // determine if it was a borrow event
    if(stat & BW_MASK){
      // code in case it was a borrow event
      Serial.println("Borrow Event");    
    }
    
    // Compare the status register to the Index bit-mask to
    // determine if it was an index pulse
    if(stat & IDX_MASK){
      // code in case it was an index pulse
      Serial.println("Index Pulse");    
    }
    // reset the ISR's flag noting that we handled it
    flag = 0;
  }
  
  // read the encoder's count as an unsigned long
  unsigned long count = enc1.readEncoder();
  
  // Now do something with the count. In this case we're just
  // printing it as a connections check.
  Serial.print(" Current Encoder Count: ");
  Serial.println(count);

  // delay so we don't flood the serial monitor
  delay(100);
}

/*
** Interrupt Service Routine for LFLAG.
*/
void ISR_ENC(){
  /*
  ** We can't read data from SPI in an ISR since its slow so tell
  ** the main loop that it needs to read the status register by
  ** setting the flag.
  */
  flag = 1;
}




