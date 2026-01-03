#include <Servo.h>

const int ENA = 11;  
const int IN1 = 5;
const int IN2 = 4;

const int ENB = 6;   
const int IN3 = 3;
const int IN4 = 2;

const int US_TRIGGER = A1;
const int US_ECHO    = A0;

const int SERVO_PIN = 10;
Servo headServo;

const int SERVO_CENTER = 90;
const int SERVO_LEFT   = 170;   
const int SERVO_RIGHT  = 10;    

const int SPEED_LEFT  = 55;
const int SPEED_RIGHT = 100;

const int STOP_DISTANCE_CM = 12;


void setup() {
  Serial.begin(9600);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(US_TRIGGER, OUTPUT);
  pinMode(US_ECHO, INPUT);

  headServo.attach(SERVO_PIN);
  headServo.write(SERVO_CENTER);
  delay(500);

  stopMotors();
  delay(500);
}


void loop() {

  long front = readDistanceCM();
  Serial.print("Front: ");
  Serial.println(front);

  if (front > STOP_DISTANCE_CM) {
    moveForward();
    return;
  }

  // Obstacol → frânare
  moveBackward();
  delay(120);
  stopMotors();
  delay(120);

  long leftDist  = scanLeftSlow();
  long rightDist = scanRightSlow();

  Serial.print("Left avg: ");
  Serial.println(leftDist);
  Serial.print("Right avg: ");
  Serial.println(rightDist);

  headServo.write(SERVO_CENTER);
  delay(250);



  if (leftDist > STOP_DISTANCE_CM) {
    Serial.println("Turn LEFT 90");
    turnLeft90();
    return;
  }

  if (rightDist > STOP_DISTANCE_CM) {
    Serial.println("Turn RIGHT 90");
    turnRight90();
    return;
  }

  Serial.println("Turn 180");
  turn180();
}



void stopMotors() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, SPEED_LEFT);
  analogWrite(ENB, SPEED_RIGHT);
}

void moveBackward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  analogWrite(ENA, SPEED_LEFT);
  analogWrite(ENB, SPEED_RIGHT);
}



void turnLeft90() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, SPEED_LEFT);
  analogWrite(ENB, SPEED_RIGHT);

  delay(300);  
  stopMotors();
  delay(150);
}

void turnRight90() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  analogWrite(ENA, SPEED_LEFT);
  analogWrite(ENB, SPEED_RIGHT);

  delay(270);  
  stopMotors();
  delay(150);
}

void turn180() {
  turnRight90();
  delay(150);
  turnRight90();
}





long scanLeftSlow() {
  headServo.write(SERVO_LEFT);
  delay(350);

  long d1 = readDistanceCM();
  delay(150);
  long d2 = readDistanceCM();

  long avg = (d1 + d2) / 2;

  Serial.print("Left d1=");
  Serial.print(d1);
  Serial.print(" d2=");
  Serial.print(d2);
  Serial.print(" avg=");
  Serial.println(avg);

  return avg;
}

long scanRightSlow() {
  headServo.write(SERVO_RIGHT);
  delay(350);

  long d1 = readDistanceCM();
  delay(150);
  long d2 = readDistanceCM();

  long avg = (d1 + d2) / 2;

  Serial.print("Right d1=");
  Serial.print(d1);
  Serial.print(" d2=");
  Serial.print(d2);
  Serial.print(" avg=");
  Serial.println(avg);

  return avg;
}


long readDistanceCM() {

  digitalWrite(US_TRIGGER, LOW);
  delayMicroseconds(2);
  digitalWrite(US_TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(US_TRIGGER, LOW);

  long duration = pulseIn(US_ECHO, HIGH, 30000);
  if (duration == 0) return 999;

  long distance = duration * 0.034 / 2;

  return distance;
}
