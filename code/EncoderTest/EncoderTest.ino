#include <SPI.h>

#define SS1 4
#define SS2 5
#define CNT_EN 6

// These hold the current encoder count.
signed long encoder1count = 0;
signed long encoder2count = 0;

void initEncoders() {

  // Set slave selects as outputs
  pinMode(SS1, OUTPUT);
  pinMode(SS2, OUTPUT);

  pinMode(CNT_EN, OUTPUT);

  // Raise select pins
  // Communication begins when you drop the individual select signsl
  digitalWrite(SS1, HIGH);
  digitalWrite(SS2, HIGH);
  digitalWrite(CNT_EN, HIGH);

  SPI.begin();

  // Initialize encoder 1
  //    Clock division factor: 0
  //    Negative index input
  //    free-running count mode
  //    x4 quatrature count mode (four counts per quadrature cycle)
  // NOTE: For more information on commands, see datasheet
  digitalWrite(SS1, LOW);       // Begin SPI conversation
  SPI.transfer(0x88);                       // Write to MDR0
  SPI.transfer(0x03);                       // Configure to 4 byte mode
  digitalWrite(SS1, HIGH);      // Terminate SPI conversation

  // Initialize encoder 2
  //    Clock division factor: 0
  //    Negative index input
  //    free-running count mode
  //    x4 quatrature count mode (four counts per quadrature cycle)
  // NOTE: For more information on commands, see datasheet
  digitalWrite(SS2, LOW);       // Begin SPI conversation
  SPI.transfer(0x88);                       // Write to MDR0
  SPI.transfer(0x03);                       // Configure to 4 byte mode
  digitalWrite(SS2, HIGH);      // Terminate SPI conversation
}

long readEncoder(int encoder) {

  // Initialize temporary variables for SPI read
  unsigned int count_1, count_2, count_3, count_4;
  long count_value;

  switch (encoder) {
    case 1:
      digitalWrite(SS1, LOW);     // Begin SPI conversation
      break;
    default: // If it was not 1, it must be 2...
      digitalWrite(SS2, LOW);     // Begin SPI conversation
      break;
  }
  
  SPI.transfer(0x60);                     // Request count
  count_1 = SPI.transfer(0x00);           // Read highest order byte
  count_2 = SPI.transfer(0x00);
  count_3 = SPI.transfer(0x00);
  count_4 = SPI.transfer(0x00);           // Read lowest order byte

  digitalWrite(SS1, HIGH);    // Terminate SPI conversation
  digitalWrite(SS2, HIGH);    // Terminate SPI conversation

  // Calculate encoder count
  count_value = (count_1 << 8) + count_2;
  count_value = (count_value << 8) + count_3;
  count_value = (count_value << 8) + count_4;

  return count_value;
}

void clearEncoderCount() {

  // Set encoder1's data register to 0
  digitalWrite(SS1, LOW);     // Begin SPI conversation
  // Write to DTR
  SPI.transfer(0x98);
  // Load data
  SPI.transfer(0x00);  // Highest order byte
  SPI.transfer(0x00);
  SPI.transfer(0x00);
  SPI.transfer(0x00);  // lowest order byte
  digitalWrite(SS1, HIGH);    // Terminate SPI conversation

  delayMicroseconds(100);  // provides some breathing room between SPI conversations

  // Set encoder1's current data register to center
  digitalWrite(SS1, LOW);     // Begin SPI conversation
  SPI.transfer(0xE0);
  digitalWrite(SS1, HIGH);    // Terminate SPI conversation

  // Set encoder2's data register to 0
  digitalWrite(SS2, LOW);     // Begin SPI conversation
  // Write to DTR
  SPI.transfer(0x98);
  // Load data
  SPI.transfer(0x00);  // Highest order byte
  SPI.transfer(0x00);
  SPI.transfer(0x00);
  SPI.transfer(0x00);  // lowest order byte
  digitalWrite(SS2, HIGH);    // Terminate SPI conversation

  delayMicroseconds(100);  // provides some breathing room between SPI conversations

  // Set encoder2's current data register to center
  digitalWrite(SS2, LOW);     // Begin SPI conversation
  SPI.transfer(0xE0);
  digitalWrite(SS2, HIGH);    // Terminate SPI conversation
}


void setup() {
  Serial.begin(115200);      // Serial com for data output

  initEncoders();       Serial.println("Encoders Initialized...");
  clearEncoderCount();  Serial.println("Encoders Cleared...");
}

void loop() {
  delay(500);

  // Retrieve current encoder counters
  encoder1count = readEncoder(1);
  encoder2count = readEncoder(2);
  
  Serial.print("Enc1: "); Serial.print(encoder1count);
  Serial.print(" Enc2: "); Serial.print(encoder2count);
  Serial.println();
  
}
