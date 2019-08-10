#include <pitches.h>
#include <Servo.h>

//www.elegoo.com
//2016.12.08
#include "SR04.h"
#define TRIG_PIN 12
#define ECHO_PIN 11

// Define Pins
#define BLUE 3
#define GREEN 5
#define RED 6

SR04 sr04 = SR04(ECHO_PIN, TRIG_PIN);
long a;

int smallestDist;
int smallestAng;

// define variables
int redValue;
int greenValue;
int blueValue;

float tick = 0;

Servo myservo;

void setup() {
  Serial.begin(9600);
  delay(1000);

  myservo.attach(9);
  myservo.write(90);

  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  digitalWrite(RED, HIGH);
  digitalWrite(GREEN, LOW);
  digitalWrite(BLUE, LOW);

  setLED(100, 0, 0);
}

void loop() {

  tick = tick + 0.01;

  

  smallestDist = 9999;
  Serial.println("Set dist to 9999");

  scan();

  delay(1000);

  faceNear();

  delay(1000);

}

void scan() {

  setLED(0, 0, 100);

  Serial.println("Rotating to 0");

  myservo.write(0);

  Serial.println("Waiting...");

  delay(1000);

  Serial.println("Rotating to 100 and scanning...");

  int ang = myservo.read(); // 0

  //myservo.write(100);

  for (int i = 0; i < 36; i++) {
    a = sr04.Distance();

    ang += 5;
    myservo.write(ang);

    Serial.print(a);
    Serial.print("cm");
    Serial.print(" @ angle ");
    Serial.print(myservo.read());

    setLED(255 - orZero(25 * a), orZero(25 * a), 0);

    if (a < smallestDist) {
      smallestAng = myservo.read();
      smallestDist = a;

      Serial.println(" [Smallest]");
    } else {
      Serial.println();
    }

    delay(100);

  }

  delay(500);
}

void faceNear() {

  setLED(255, 255, 255);

  Serial.print("Smallest dist: ");
  Serial.println(smallestDist);
  Serial.print("Facing angle ");
  Serial.println(smallestAng);

  myservo.write(smallestAng);

  delay(3000);

}

void setLED(int r, int g, int b) {
  analogWrite(RED, r);
  analogWrite(GREEN, g);
  analogWrite(BLUE, b);
}

int orZero(int val) {
  if(val > 0) {
    return val;
  } else {
    return 0;
  }
}
