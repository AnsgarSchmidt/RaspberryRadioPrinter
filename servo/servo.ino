#include <Servo.h> 
 
Servo myservo;

void setup() 
{ 
 Serial.begin(9600);
 myservo.attach(9);
 pinMode(13, OUTPUT);
} 
 
void loop() 
{
 while (Serial.available() > 0) {
  int pos = Serial.parseInt(); 
  myservo.write(pos);
  Serial.println(pos);
 }
} 
