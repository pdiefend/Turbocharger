#define DIRA 7
#define DIRB 8
#define PWMA 9
#define PWMB 10
#define CSA A3
#define CSB A2

void setup(){
  Serial.begin(115200);

  pinMode(DIRA, OUTPUT);
  pinMode(DIRB, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(PWMB, OUTPUT);

  digitalWrite(DIRA, LOW);
  digitalWrite(DIRB, LOW);
  digitalWrite(PWMA, LOW);
  digitalWrite(PWMB, LOW);

  pinMode(A3, INPUT);
  pinMode(A4, INPUT);

  Serial.println("Begin in 1s");
  delay(1000);
}

void loop(){
  digitalWrite(DIRA, HIGH);
  Serial.println("Forward Ramp Up");
  for(int i = 0; i < 255; i++){
    analogWrite(PWMA, i);
    delay(10); 
  }

  Serial.println("Forward Hold");
  analogWrite(PWMA, 255);
  delay(1000);
  Serial.println(analogRead(CSA));
  delay(1000);

  Serial.println("Forward Ramp Down");
  for(int i = 255; i > 0; i--){
    analogWrite(PWMA, i);
    delay(10); 
  }

  digitalWrite(PWMA, LOW);
  Serial.println("Halt");
  delay(2000);  



  digitalWrite(DIRA, LOW);
  Serial.println("Backward Ramp Up");
  for(int i = 0; i < 255; i++){
    analogWrite(PWMA, i);
    delay(10); 
  }

  Serial.println("Backward Hold");
  analogWrite(PWMA, 255);
  delay(1000);
  Serial.println(analogRead(CSA));
  delay(1000);

  Serial.println("Backward Ramp Down");
  for(int i = 255; i > 0; i--){
    analogWrite(PWMA, i);
    delay(10); 
  }

  digitalWrite(PWMA, LOW);
  Serial.println("Halt");
  delay(2000);  
}



