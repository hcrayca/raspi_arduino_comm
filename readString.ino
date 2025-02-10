#include <Servo.h>
//16V -> 1100us-3465rpm 1896us-3527rpm  
//3527 rpm = 369.35 rad/s 367.53
//3465 rpm = 362.85 rad/s -364.66
Servo ESC1,ESC2;

const byte numChars = 32;
//char receivedChars[numChars];

// variables to hold the parsed data
float velocityRight = 0.0;
float velocityLeft = 0.0;

float max_vel = 50;
float min_vel = -50;

int pwmSignal1;
int pwmSignal2;
//String integerFromPC;

boolean newData = false;

void setup() {
  // put your setup code here, to run once:
  ESC1.attach(9);
  ESC2.attach(10);
  Serial.begin(9600);

}

void loop() {
  receiveData();
  if (newData) {
    Angular_Velocity(); 
    newData = false;
  }
}

void receiveData() {

  if (Serial.available() > 0) {

    String receivedData = Serial.readStringUntil('\n');  // Read full input
    receivedData.trim();  // Remove spaces and newlines

    // Find the comma position
    int commaIndex = receivedData.indexOf(',');
    if (commaIndex != -1) {  // Ensure there's a comma
      // Extract the two numbers as substrings
      String rightStr = receivedData.substring(0, commaIndex);
      String leftStr = receivedData.substring(commaIndex + 1);

      // Convert to float
      velocityRight = rightStr.toFloat();
      velocityLeft = leftStr.toFloat();

      if(velocityRight != 0.0 || velocityLeft != 0.0){

        velocityRight = velocityRight-1.4;
        velocityLeft = velocityLeft-1.4;
      }

      String data = String(velocityRight) + "," + String(velocityLeft);
      data.trim();
      Serial.println(data);
      newData = true;
  }
  else {
    Serial.println("Error: Invalid data format");
        }
        
  delay(100);
  
  while (Serial.available()) Serial.read();  // Clears remaining buffer
  }
  
}
/*
bool isValidInput(float value) {
  return (value >= min_vel && value <= max_vel);
}*/

void Angular_Velocity(){
  
  int pwmSignal1 = 1500;
  int pwmSignal2 = 1500;

  if (velocityRight > 0){
    pwmSignal1 = map(velocityRight, 0, max_vel, 1525, 1896);
  }
  else if(velocityRight < 0){
    pwmSignal1 = map(velocityRight, min_vel, 0, 1100, 1475);
  }

  if (velocityLeft > 0) {
    pwmSignal2 = map(velocityLeft, 0, max_vel, 1525, 1896);
  }
  else if (velocityLeft < 0) {
    pwmSignal2 = map(velocityLeft, min_vel, 0, 1100, 1475);
  }
/*
  Serial.print("PWM Signal1: ");
  Serial.println(pwmSignal1);

  Serial.print("PWM Signal2: ");
  Serial.println(pwmSignal2);*/

  ESC1.writeMicroseconds(pwmSignal1);
  ESC2.writeMicroseconds(pwmSignal2);

  delay(100);
  //ESC2.writeMicroseconds(pwmSignal2);
}
