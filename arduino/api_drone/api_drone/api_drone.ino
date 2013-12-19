#include <Servo.h>
  
// we use the servo library to control the motors
Servo motor1, motor2, motor3, motor4;

int motor1_pin = 9;
int motor2_pin = 10;


// mpx permax 400 brushed motors needs at least 80 to start (7,4V Lipo)
int min_speed = 80;
  
void setup()
{
  Serial.begin(9600);
  Serial.print("Ready\n");

  // calibrate motors during setup, otherwise they wouldn't start
  motor1.attach(motor1_pin);
  motor1.write(0);
  delay(500);
  motor2.attach(motor2_pin);
  motor2.write(0);
  delay(500);
  
  // wait a second to synchronize the motors
  delay(1000);
}

void loop()
{
  //startMotors();
  //delay(5000);
  //stopMotors();
  //delay(5000);
}

// before we take off let's start the motors with minimum speed
void startMotors() {
  motor1.write(min_speed); 
  motor2.write(min_speed);
}

// power off the motors
void stopMotors() {
  motor1.write(0);
  motor2.write(0);
}

