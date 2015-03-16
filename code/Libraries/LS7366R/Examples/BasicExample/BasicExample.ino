#include <SPI.h>
#include "LS7366R.h"

#define SS1 4
#define CNT_EN 6

LS7366R enc1(SS1, CNT_EN);

void setup(){
  Serial.begin(115200);
  enc1.enableEncoder();
}

void loop(){
  for(int i = 0; i < 7; i++){
    Serial.print("Encoder Count: ")
    Serial.println(enc1.readEncoder());
    delay(250);
  }
  enc1.clearEncoder();
}


