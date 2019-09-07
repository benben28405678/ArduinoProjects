#include <DS3231.h>

#include <pitches.h>

#include <IRremoteInt.h>
#include <IRremote.h>

//www.elegoo.com
//2016.12.9

#include "IRremote.h"

DS3231 clock;
RTCDateTime dt;

IRrecv irrecv(11);
decode_results results;

int receiver = 11;

int alarmHour;
int alarmMinute;

int melody[] = {
  NOTE_A4, NOTE_B4, NOTE_C5, NOTE_D5
};

void setup()
{
  Serial.begin(9600);

  clock.begin();

  irrecv.enableIRIn();

  clock.setDateTime(__DATE__, __TIME__);

  alarmHour = 7;
  alarmMinute = 10;
}

int subtick = 0;
int tick = 0;
int tickSinceStartOfAlarm = -1;
boolean doAlarm = true;
void loop()
{
  fourPerSec();

  if (irrecv.decode(&results)) // have we received an IR signal?
  {
    translateIR();
    irrecv.resume(); // receive the next value

    if(results.value == 0xFFA25D) {
      doAlarm = false;
      tone(8, melody[3], 500);
    }
  }

  delay(250);
}

void translateIR() // takes action based on IR code received

// describing Remote IR codes

{

  switch (results.value)

  {
    case 0xFFA25D: Serial.println("POWER"); break;
    case 0xFFE21D: Serial.println("FUNC/STOP"); break;
    case 0xFF629D: Serial.println("VOL+"); break;
    case 0xFF22DD: Serial.println("FAST BACK");    break;
    case 0xFF02FD: Serial.println("PAUSE");    break;
    case 0xFFC23D: Serial.println("FAST FORWARD");   break;
    case 0xFFE01F: Serial.println("DOWN");    break;
    case 0xFFA857: Serial.println("VOL-");    break;
    case 0xFF906F: Serial.println("UP");    break;
    case 0xFF9867: Serial.println("EQ");    break;
    case 0xFFB04F: Serial.println("ST/REPT");    break;
    case 0xFF6897: Serial.println("0");    break;
    case 0xFF30CF: Serial.println("1");    break;
    case 0xFF18E7: Serial.println("2");    break;
    case 0xFF7A85: Serial.println("3");    break;
    case 0xFF10EF: Serial.println("4");    break;
    case 0xFF38C7: Serial.println("5");    break;
    case 0xFF5AA5: Serial.println("6");    break;
    case 0xFF42BD: Serial.println("7");    break;
    case 0xFF4AB5: Serial.println("8");    break;
    case 0xFF52AD: Serial.println("9");    break;
    case 0xFFFFFFFF: Serial.println(" REPEAT"); break;

    default:
      Serial.println(" other button   ");

  }

  delay(500); // Do not get immediate repeat

}

void fourPerSec() {
  if (tick > 1000) {
    tick = 0;
  }

  tick += 1;

  if (tickSinceStartOfAlarm >= 0 && doAlarm) {
    tickSinceStartOfAlarm += 1;

    if (tickSinceStartOfAlarm < 45 && tick % 4 == 0) {
      tone(8, melody[0], 500);
    }
    else if (tickSinceStartOfAlarm > 45 && tickSinceStartOfAlarm < 90 && tick % 2 == 0) {
      tone(8, melody[1], 250);
    }
    else if (tickSinceStartOfAlarm > 90 && tickSinceStartOfAlarm < 180) {
      tone(8, melody[2], 125);
    }

    if (tickSinceStartOfAlarm > 60) {
      tickSinceStartOfAlarm = -1;
    }
  }

  dt = clock.getDateTime();

  if(!doAlarm && dt.minute > 1 + alarmMinute) {
    doAlarm = true;
  }

  Serial.print(dt.hour);   Serial.print(":");
  Serial.print(dt.minute); Serial.print(":");
  Serial.print(dt.second); Serial.println("");

  if (dt.hour == alarmHour && dt.minute == alarmMinute && dt.second == 0) {
    Serial.println("ALARM");
    tickSinceStartOfAlarm = 0;
  }
}
