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
      float velocity_right = rightStr.toFloat();
      float velocity_left = leftStr.toFloat();

      // Print results
    
      Serial.println(velocity_right, 2);
      Serial.println(velocity_left, 2);
  }
  else {
    Serial.println("Error: Invalid data format");
        }
        
  Serial.flush();
  newData = true;
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