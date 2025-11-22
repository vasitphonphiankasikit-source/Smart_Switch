#include <ESP32Servo.h>
#include <NewPing.h>

#define TRIGGER_PIN  5  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     12  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

const int in1Pin = 18;
const int in2Pin = 19;
const int in3Pin = 22;
const int in4Pin = 23;
const int servo_pin = 13;
const int button_pin = 14;
int button_state;
int robot_state;
int distances;
int distancesLeft;
int distancesRight;
int min_distances = 0;
int max_distances = 25;
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

Servo myservo; 
void setup() {
  Serial.begin(9600);
  pinMode(button_pin, INPUT);
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(in3Pin, OUTPUT);
  pinMode(in4Pin, OUTPUT);
	ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
	myservo.setPeriodHertz(50);   
	myservo.attach(servo_pin, 700, 5000);
}

void loop() {
  button_state = digitalRead(button_pin);
  distances = sonar.ping_cm();
   Serial.println(distances);
  if(robot_state == 0){
   Stop();
  }
  else{
  if(distances > min_distances && distances < max_distances){
    Backward();
    delay(500);
    Stop();
    delay(250);
    check_Left_Right();
  }
  else{
    myservo.write(100);
    Forward();
  } 
  }
  
  if(button_state == 1){
    robot_state ++;
    delay(300);
    if(robot_state > 1) robot_state = 0;
  }
}
void check_Left_Right(){
  myservo.write(180); 
	delay(1000); 
  distancesLeft = sonar.ping_cm();
  delay(100); 
  myservo.write(0); 
	delay(1000); 
  distancesRight = sonar.ping_cm();
  delay(100);
  myservo.write(100); 
  if(distancesLeft > distancesRight){
   turnLeft();
   delay(1650);
   Stop();
   delay(500);
  }
  else if(distancesLeft < distancesRight){
   turnRight();
   delay(1650);
   Stop();
   delay(500);
  }
  else{
  turnRight();
  delay(3300);
  Stop();
  delay(500);
  }
}
void Forward() {
  digitalWrite(in1Pin, HIGH);
  digitalWrite(in2Pin, HIGH);
  digitalWrite(in3Pin, LOW);
  digitalWrite(in4Pin, LOW);
}

void Backward() {
  digitalWrite(in1Pin, LOW);
  digitalWrite(in2Pin, LOW);
  digitalWrite(in3Pin, HIGH);
  digitalWrite(in4Pin, HIGH);

}

void turnLeft() {
  digitalWrite(in1Pin, LOW);
  digitalWrite(in2Pin, HIGH);
  digitalWrite(in3Pin, LOW);
  digitalWrite(in4Pin, HIGH);
 
}

void turnRight() {
  digitalWrite(in1Pin, HIGH);
  digitalWrite(in2Pin, LOW);
  digitalWrite(in3Pin, HIGH);
  digitalWrite(in4Pin, LOW);

}

void Stop() {
  digitalWrite(in1Pin, LOW);
  digitalWrite(in2Pin, LOW);
  digitalWrite(in3Pin, LOW);
  digitalWrite(in4Pin, LOW);
}
