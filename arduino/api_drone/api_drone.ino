#include <Servo.h>
  
#define MOTOR1_PIN  9
#define MOTOR2_PIN 10

// we use the servo library to control the motors
Servo motor1, motor2, motor3, motor4;

// mpx permax 400 brushed motors needs at least 80 to start (7,4V Lipo)
int min_speed = 80;

String input = "";
  
void setup()
{
  Serial.begin(9600);
  Serial.print("Initializing... ");

  // calibrate motors during setup, otherwise they wouldn't start
  motor1.attach(MOTOR1_PIN);
  motor1.write(0);
  delay(500);
  motor2.attach(MOTOR2_PIN);
  motor2.write(0);
  delay(500);
  
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
  motor1.write(min_speed); 
  motor2.write(min_speed);
  
  return "ok";
}


// power off the motors
String stopMotors() {
  motor1.write(0);
  motor2.write(0);
  
  return "ok";
}


