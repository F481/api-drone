#include <Servo.h>
#include <NewPing.h>

#define TRIGGER_PIN 2
#define ECHO_PIN 13
#define MAX_DISTANCE 200

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

bool debugMode = true;

// we use the servo library to control the motors
Servo motor1, motor2, motor3, motor4;
Servo motor[] = { motor1, motor2, motor3, motor4 };

// maps motors to its related pins
int motor_pin[] = { 9, 10, 11, 12 };

// maybe we have to change min_speed depending on used motors, ESC and battery
int min_speed = 80;
int max_speed = 180;

int numberOfMotors = sizeof(motor) / sizeof(motor[0]);
String input = "";
  
void setup()
{
  // hack to make printf work..
  // http://reza.net/wordpress/?p=269
  fdevopen(&my_putc, 0);
  
  Serial.begin(9600);
  Serial.println("Initializing... ");
  
  // calibrate motors during setup, otherwise they wouldn't start
  for (int i = 0; i < numberOfMotors; i++) {
    motor[i].attach(motor_pin[i]);
    motor[i].write(0);
    delay(500);
    if (debugMode) {
      printf("Motor%d successfully calibrated on pin %d \n", i+1, motor_pin[i]);
    }
  }
  
  // wait a second to synchronize the motors
  delay(1000);
  Serial.println("Ready!");
}

void loop()
{  
  char character;
  
  // read commands from serial
  while(Serial.available()) {
    character = Serial.read();
    input.concat(character);
    delay(2);
  }
  
  if (input != "") {
    String result = handleCommand(input);
    Serial.println(result);
    input = "";
  }
  
  //Serial.println(getDistanceInCm());
}

// format: <command>[:argument] (where ":argument" is optional)
String handleCommand(String input) {
  String result;
  
  String command = getCommandFromInput(input);
  int argument = getArgumentFromInput(input);

  // switch-case-construct would be better, but it's not supporting strings as case value
  if (command == "start") {
    result = startMotors();
  } else if (command == "stop") {
    result = stopMotors();
  } else if (command == "speed") {
    //result = setMotorSpeed(1, 0);
    //result = setMotorSpeed(2, 30);
  } else {
    result = "unknown command: " + command;
  }
    
  return result;
}


// parses the command from serial input
String getCommandFromInput(String input) {
  String result = input;
  int positionOfColon = input.indexOf(':');
  
  if (positionOfColon) {
    result = input.substring(0, positionOfColon);
  }
 
  return result;
}


// parses the argument (is any given) from serial input
int getArgumentFromInput(String input) {
  int result = -1;
  int positionOfColon = input.indexOf(':');
  
  if (positionOfColon) {
    int length = input.length() - positionOfColon;
    String argument = input.substring(positionOfColon + 1);
    result = argument.toInt();
  }
 
  return result;
}


// before we take off let's start the motors with minimum speed
String startMotors() {
  for (int i = 0; i < numberOfMotors; i++) {
    motor[i].write(min_speed);
    if (debugMode) {
      printf("Starting motor%d... \n", i+1);
    }
  }
  
  return "ok";
}


// power off the motors
String stopMotors() {
  for (int i = 0; i < numberOfMotors; i++) {
    motor[i].write(0);
    if (debugMode) {
      printf("Stopping motor%d... \n", i+1);
    }
  }
  
  return "ok";
}

// set speed in percent of motor "motorNumber"
String setMotorSpeed(int motorNumber, int speedInPercent) {
  // map the given percentage speed to our speed range
  // for security reasons 0 percent is mapped to min_speed, if you really want to power off the motors call stopMotors()
  int motorSpeed = map(
    speedInPercent,
    0, 100,
    min_speed, max_speed
  );
  
  // motor array start with 0
  motor[motorNumber - 1].write(motorSpeed);
  if (debugMode) {
    printf("Set speed of motor%d to %d \n", motorNumber, motorSpeed);
  }
  
  return "ok";
}


int getDistanceInCm() {
  return sonar.ping_cm(); 
}


// hack to make printf work..
int my_putc(char c, FILE *t) {
  Serial.write(c);
}
