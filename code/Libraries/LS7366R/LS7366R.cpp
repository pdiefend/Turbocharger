// ALPHA RELEASE
// prd005

#include <SPI.h>
#include "LS7366R.h"
#include "Arduino.h"

LS7366R::LS7366R(int SSP, int CNT_EN){
  _SS = SSP;
  _EN = CNT_EN;
  _bytes = FOUR_BYTE;
	  
  pinMode(_SS, OUTPUT);
  pinMode(CNT_EN, OUTPUT);
  digitalWrite(_SS, HIGH);

  SPI.begin();

  //  Default Encoder Settings
  //  Clock division factor: 0
  //  Negative index input
  //  free-running count mode
  //  x4 quatrature count mode (four counts per quadrature cycle)
  //  2-byte counter
	digitalWrite(_SS, LOW);
	SPI.transfer(0x88);      // Write to MDR0
	SPI.transfer(X4_QUAD);       
	digitalWrite(_SS, HIGH);
	
	delayMicroseconds(100);
	
	digitalWrite(_SS, LOW);
	SPI.transfer(0x90);      // Write to MDR1
	SPI.transfer(TWO_BYTE);       
	digitalWrite(_SS, HIGH);
}


LS7366R::LS7366R(int SSP, int CNT_EN, byte MDR0, byte MDR1){
	_SS = SSP;
	_EN = CNT_EN;
	  
	_bytes = MDR1 & 0x3;
	  
	pinMode(_SS, OUTPUT);
	pinMode(CNT_EN, OUTPUT);
	digitalWrite(_SS, HIGH);
	
	SPI.begin();

	digitalWrite(_SS, LOW);
	SPI.transfer(0x88);      // Write to MDR0
	SPI.transfer(MDR0);       
	digitalWrite(_SS, HIGH);
	
	delayMicroseconds(100);
	
	digitalWrite(_SS, LOW);
	SPI.transfer(0x90);      // Write to MDR1
	SPI.transfer(MDR1);       
	digitalWrite(_SS, HIGH);
}

unsigned long LS7366R::readEncoder(){
  unsigned long count_value = 0;

  digitalWrite(_SS, LOW);
  
  SPI.transfer(0x60);  // Request count
  /*
  unsigned int count_1 = 0;
  unsigned int count_2 = 0;
  unsigned int count_3 = 0;
  unsigned int count_4 = 0;
  
  switch(_bytes){
	case 0:
		count_1 = SPI.transfer(0x00);
		count_2 = SPI.transfer(0x00);
		count_3 = SPI.transfer(0x00);
		count_4 = SPI.transfer(0x00);
		break;
	case 1:
		count_2 = SPI.transfer(0x00);
		count_3 = SPI.transfer(0x00);
		count_4 = SPI.transfer(0x00);
		break;
	case 2:
		count_3 = SPI.transfer(0x00);
		count_4 = SPI.transfer(0x00);
		break;
	default:
		count_4 = SPI.transfer(0x00);
		break;		
	}
	
	  digitalWrite(_SS, HIGH);

  // Calculate encoder count
  count_value = (count_1 << 8) + count_2;
  count_value = (count_value << 8) + count_3;
  count_value = (count_value << 8) + count_4;
	*/
	
	// This is not an improper use of switch-case, just clever...
	switch(_bytes){
	case 0:
		count_value = SPI.transfer(0x00);
		count_value = count_value << 8;
	case 1:
		count_value = count_value + SPI.transfer(0x00);
		count_value = count_value << 8;
	case 2:
		count_value = count_value + SPI.transfer(0x00);
		count_value = count_value << 8;
	default:
		count_value = count_value + SPI.transfer(0x00);		
	}
  digitalWrite(_SS, HIGH);

  

  return count_value;
}

// There's a better way to accomplish this...
void LS7366R::clearEncoder(){
  digitalWrite(_SS, LOW);

  SPI.transfer(0x98);

  SPI.transfer(0x00);
  SPI.transfer(0x00);
  SPI.transfer(0x00);
  SPI.transfer(0x00);
  digitalWrite(_SS, HIGH);

  delayMicroseconds(100);


  digitalWrite(_SS, LOW);     
  SPI.transfer(0xE0);      // Load DTR into CNTR
  digitalWrite(_SS, HIGH);
}

void LS7366R::enableEncoder(){
	digitalWrite(_EN, HIGH);
}

void LS7366R::disableEncoder(){
	digitalWrite(_EN, LOW);
}

// =================================================================================
// Wrong, DTR changes its width with the byte mode. This function needs to be fixed
// to reflect those changes.
void LS7366R::writeDTR(unsigned long set){
  byte b1, b2, b3, b4;
  b1 = set & 0xFF;
  b2 = (set >> 8) & 0xFF;
  b3 = (set >> 16) & 0xFF;
  b4 = (set >> 32) & 0xFF;

  digitalWrite(_SS, LOW);
  
  SPI.transfer(0x98);  // Write DTR
  SPI.transfer(b1);
  SPI.transfer(b2);
  SPI.transfer(b3);
  SPI.transfer(b4);

  digitalWrite(_SS, HIGH); 
}
// =================================================================================


byte LS7366R::readSTR(){
  byte ret = 0;
  digitalWrite(_SS, LOW);
  SPI.transfer(0x70);  // Request STR
  ret = SPI.transfer(0x00);
  digitalWrite(_SS, HIGH);
  
  delayMicroseconds(100);
  
  digitalWrite(_SS, LOW);
  SPI.transfer(0x30); // Clear STR
  digitalWrite(_SS, HIGH); 
  return ret;
}

/*
void LS7366R::setCounter(unsigned long set){
	// TODO implement this.
}
*/

/*
unsigned long LS7366R::readOTR(){
	// TODO implement this.
}
*/

// =================================================================================
long LS7366R::s_readEncoder(){
  // TODO, decide on keeping or not.
  long count_value = 0;

  digitalWrite(_SS, LOW);
  
  SPI.transfer(0x60);  // Request count
	switch(_bytes){
	case FOUR_BYTE:
		// get the data it is kind of signed already...
		count_value = SPI.transfer(0x00);
		count_value = (count_value << 8) + SPI.transfer(0x00);
		count_value = (count_value << 8) + SPI.transfer(0x00);
		count_value = (count_value << 8) + SPI.transfer(0x00);
		break;
	case THREE_BYTE:
		// get the data
		count_value = SPI.transfer(0x00);
		count_value = (count_value << 8) + SPI.transfer(0x00);
		count_value = (count_value << 8) + SPI.transfer(0x00);
		
		// sign extend the MSB
		if(count_value & 0x00800000){
			count_value |= 0xFF000000;
		}
		break;
		
	case TWO_BYTE:
		// get the data
		count_value = SPI.transfer(0x00);
		count_value = (count_value << 8) + SPI.transfer(0x00);
		
		// sign extend the MSB
		if(count_value & 0x00008000){
			count_value |= 0xFFFF0000;
		}
		
		break;
		
	default: // ONE_BYTE
		count_value = count_value + SPI.transfer(0x00);		
		// sign extend the MSB
		if(count_value & 0x00000080){
			count_value |= 0xFFFFFF00;
		}
		
		break;
	}
  digitalWrite(_SS, HIGH);

  

  return count_value;
}


