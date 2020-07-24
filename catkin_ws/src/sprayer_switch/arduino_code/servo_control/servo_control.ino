#include <Arduino.h>
#include <Servo.h>
#define servoPin 9

Servo myservo;
char cmd;

void setup(){
  Serial.begin(57600);
  myservo.attach(servoPin);
  myservo.write(80);
  Serial.println("Starting ...");
}

void loop(){
  while(Serial.available()){
    cmd = Serial.read();
    delay(10);

    if(cmd == 's')
    {
      Serial.println("Start to spray !");
      myservo.write(40);
    }
    if(cmd == 'f')
    {
      Serial.println("Finish to spray !");
      myservo.write(80);
    }
  }
  delay(20);
}
