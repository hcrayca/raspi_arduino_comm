#include <Servo.h>

Servo ESC1, ESC2;
int last_value;
bool throttleExecuted = false;

//int ESC1_value = 1500;
//int ESC2_value = 1500;

void setup() {
  Serial.begin(9600);

  // Attach ESCs to Arduino pins
  ESC1.attach(9, 1100, 1900);
  ESC2.attach(10, 1100, 1900);

  // Send neutral signal to arm ESCs
  Serial.println("Arming ESCs...");
  ESC1.writeMicroseconds(1500);
  ESC2.writeMicroseconds(1500);
  delay(7000); // Allow time for arming sequence
  Serial.println("ESCs armed and ready.");
}

void loop() {

  ESC1.writeMicroseconds(1540);
  ESC2.writeMicroseconds(1540);
  Serial.println("ESCs : 1540");
  delay(7000);

  ESC1.writeMicroseconds(1500);
  ESC2.writeMicroseconds(1500);
  delay(1000);


  ESC1.writeMicroseconds(1450);
  ESC2.writeMicroseconds(1450);
  Serial.println("ESCs : 1450");
  delay(7000); 

  ESC1.writeMicroseconds(1500);
  ESC2.writeMicroseconds(1500);
  delay(1000);

  ESC1.writeMicroseconds(1540);
  ESC2.writeMicroseconds(1450);
  Serial.println("ESC1 : 1540");
  Serial.println("ESC2 : 1450");
  delay(7000);

  ESC1.writeMicroseconds(1500);
  ESC2.writeMicroseconds(1500);
  delay(1000);
  
  ESC1.writeMicroseconds(1450);
  ESC2.writeMicroseconds(1540);
  Serial.println("ESC1 : 1450");
  Serial.println("ESC2 : 1540");
  delay(7000);  

  ESC1.writeMicroseconds(1500);
  ESC2.writeMicroseconds(1500);
  delay(1000);
  }



