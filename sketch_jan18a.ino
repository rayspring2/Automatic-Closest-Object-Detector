#include <Servo.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 10, 5, 4, 3, 2);

Servo myServo;

const int trigPin = 9;
const int echoPin = 8;

long duration;
int distance;

int closestDistance = 200;
int closestAngle = 0;
const int ledPin = 7;

void setup() {
  lcd.begin(16, 2);
  myServo.attach(11);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  Serial.begin(9600);
}

void loop() {
  closestDistance = 200;

  scanArea(0, 180, 1);
  displayClosestObject();
  delay(5000);

  scanArea(180, 0, -1); 
  displayClosestObject();
  delay(5000);
}

void scanArea(int startAngle, int endAngle, int stepDirection) {
  for (int angle = startAngle; (stepDirection == 1 ? angle <= endAngle : angle >= endAngle); angle += stepDirection) {
    myServo.write(angle);
    delay(30);

    distance = calculateDistance();
    if (distance > 0 && distance < 5) {
      digitalWrite(ledPin, HIGH);
    } else {
      digitalWrite(ledPin, LOW);
    }

    lcd.setCursor(0, 0);
    lcd.print("Dist: ");
    lcd.print(distance);
    lcd.print(" cm  ");

    lcd.setCursor(0, 1);
    lcd.print("Angle: ");
    lcd.print(angle);
    lcd.print(" deg ");

    if (distance < closestDistance && distance > 0) {
      closestDistance = distance;
      closestAngle = angle;
    }
  }
}

void displayClosestObject() {
  lcd.setCursor(0, 0);
  lcd.print("Closest: ");
  lcd.print(closestDistance);
  lcd.print(" cm  ");

  lcd.setCursor(0, 1);
  lcd.print("At: ");
  lcd.print(closestAngle);
  lcd.print(" deg  ");
}

int calculateDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  return distance;
}