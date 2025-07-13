#include <Arduino.h>

int buzzerPassivePin = 3;int buttonPin = 2;int button2Pin = 4;
int button3Pin = 5;int button4Pin = 6;

int buzzerPassiveCurrentKHZ[] = {1000};
int buttonPressStreak[4] = {0, 0, 0, 0};
int buttonPinByNumber[4] = {2, 4, 5, 6};

unsigned long cooldown[5] = {millis(), millis(), millis(), millis(), millis()};

void buzzerPassiveAddKHZ(int which, int howMuch) {buzzerPassiveCurrentKHZ[which] = constrain((buzzerPassiveCurrentKHZ[which] + howMuch), 0, 8000);}

void setup() {
  pinMode(buzzerPassivePin, OUTPUT);pinMode(buttonPin, INPUT_PULLUP);;pinMode(button2Pin, INPUT_PULLUP);
  pinMode(button3Pin, INPUT_PULLUP);;pinMode(button4Pin, INPUT_PULLUP);

  Serial.begin(250000);
}

void loop() {
  Serial.println(String(digitalRead(buttonPin)) + String(digitalRead(button2Pin)) + String(digitalRead(button3Pin)) + String(digitalRead(button4Pin)) + String(buzzerPassiveCurrentKHZ[0]));
  if(digitalRead(buttonPin) == 0) {noTone(buzzerPassivePin);tone(buzzerPassivePin, buzzerPassiveCurrentKHZ[0], 1);}
  for(int i = 0; i < 4; i++) {
    if((digitalRead(buttonPinByNumber[i]) == 0) && ((millis() - cooldown[i] > 250) || (millis() - cooldown[i] > 1 && buttonPressStreak[i] > 2))) {
      cooldown[i] = millis();
      buttonPressStreak[i]++;
      //    Serial.println(String(i) + String(buttonPressStreak[i]));
      switch (i) {
        case 1 : {buzzerPassiveAddKHZ(0, 1);break;}
        case 2 : {buzzerPassiveAddKHZ(0, -1);break;}
        case 3 : {buzzerPassiveCurrentKHZ[0] = 1000;break;}
      }
      if(millis() - cooldown[4] > 500) {
        cooldown[4] = millis();
        noTone(buzzerPassivePin);tone(buzzerPassivePin, buzzerPassiveCurrentKHZ[0], 500);
      }
    } else if(digitalRead(buttonPinByNumber[i]) == 1) {buttonPressStreak[i] = 0;}
  }
}