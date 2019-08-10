#include <Servo.h>
#include <pitches.h>

//www.elegoo.com
//2016.12.08
#include "SR04.h"
#define TRIG_PIN 12
#define ECHO_PIN 11

// Define Pins
#define BLUE 3
#define GREEN 5
#define RED 6

Servo myservo;

SR04 sr04 = SR04(ECHO_PIN, TRIG_PIN);
long a;

// notes in the melody:
int melody[] = {
  NOTE_A4, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5
};
int duration = 500;  // 500 miliseconds

void setup() {
  Serial.begin(9600);
  delay(1000);

  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  digitalWrite(RED, HIGH);
  digitalWrite(GREEN, LOW);
  digitalWrite(BLUE, LOW);

  myservo.attach(9);
  myservo.write(90);// move servos to center position -> 90°
}

int last;

void loop() {
  a = sr04.Distance();

  if (abs(a - last) > 3 && a < 2000) {
    Serial.print(a);
    Serial.println("cm");
  }

  last = a;
  //delay(10);

  if (a < 100) {
    tone(8, melody[(int)random(0, 7)], 509);
    rainbow();
    spin();
  } else {
    greenPulse();
  }
}

// define variables
int redValue;
int greenValue;
int blueValue;

void rainbow() {
#define delayTime 1 // fading time between colors

  redValue = 255; // choose a value between 1 and 255 to change the color.
  greenValue = 0;
  blueValue = 0;


  // this is unnecessary as we've either turned on RED in SETUP
  // or in the previous loop ... regardless, this turns RED off
  // analogWrite(RED, 0);
  // delay(1000);

  for (int i = 0; i < 255; i += 1) // fades out red bring green full when i=255
  {
    redValue -= 1;
    greenValue += 1;
    // The following was reversed, counting in the wrong directions
    // analogWrite(RED, 255 - redValue);
    // analogWrite(GREEN, 255 - greenValue);
    analogWrite(RED, redValue);
    analogWrite(GREEN, greenValue);
    delay(delayTime);
  }

  redValue = 0;
  greenValue = 255;
  blueValue = 0;

  for (int i = 0; i < 255; i += 1) // fades out green bring blue full when i=255
  {
    greenValue -= 1;
    blueValue += 1;
    // The following was reversed, counting in the wrong directions
    // analogWrite(GREEN, 255 - greenValue);
    // analogWrite(BLUE, 255 - blueValue);
    analogWrite(GREEN, greenValue);
    analogWrite(BLUE, blueValue);
    delay(delayTime);
  }

  redValue = 0;
  greenValue = 0;
  blueValue = 255;

  for (int i = 0; i < 255; i += 1) // fades out blue bring red full when i=255
  {
    // The following code has been rearranged to match the other two similar sections
    blueValue -= 1;
    redValue += 1;
    // The following was reversed, counting in the wrong directions
    // analogWrite(BLUE, 255 - blueValue);
    // analogWrite(RED, 255 - redValue);
    analogWrite(BLUE, blueValue);
    analogWrite(RED, redValue);
    delay(delayTime);
  }
  delay(10);
}

float tick = 0;
void greenPulse() {
  tick += 0.03;
  redValue = 0;
  greenValue = abs(50 * sin(tick));
  blueValue = 0;

  analogWrite(BLUE, blueValue);
  analogWrite(RED, redValue);
  analogWrite(GREEN, greenValue);
  delay(10);
}

void spin() {
  myservo.write((int)random(0, 360));
}
