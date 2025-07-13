# Arduino-Buzzer-Frequency-Controller

Hi, my name is Rahmat Hidayat. I code for fun and sometimes share my code here so I can reuse it later.  
Feel free to use or modify this project. If you need help setting it up, follow this tutorial.

## Required Materials
- Arduino Uno / Nano / Mega
- Passive Buzzer module
- 4 Push Buttons
- Breadboard
- ~12 Jumper wires (Male to Male)

## Wiring

| Component | Arduino Pin |
|-----------|-------------|
| Buzzer    | 3           |
| Button 1  | 2           |
| Button 2  | 4           |
| Button 3  | 5           |
| Button 4  | 6           |

> **Note:**  
> All buttons are connected to GND (because `INPUT_PULLUP` is enabled on Arduino), so pressed = LOW (0).

## How to Use
1. Download the code from the `Tags` section or copy it from this repo.
2. Open it with Arduino IDE.
3. Select your board (`Tools -> Board -> Arduino Uno`) and port (`Tools -> Port`).
4. Upload to your Arduino.
5. Press the buttons:
   - **Button 1:** Play the buzzer at the current frequency.
   - **Button 2:** Increase the buzzer frequency.
   - **Button 3:** Decrease the buzzer frequency.
   - **Button 4:** Reset the buzzer frequency to 1000 Hz.

## Code Highlights
- Uses arrays to map button pins (`buttonPinByNumber`), handle cooldown debounce (`cooldown`), and track press streaks (`buttonPressStreak`).
- The `buzzerPassiveAddKHZ` function ensures frequency stays within 0–8000 Hz.
- Serial monitor prints button states and current frequency.

## Full Code

```
#include <Arduino.h>

int buzzerPassivePin = 3;
int buttonPin = 2;
int button2Pin = 4;
int button3Pin = 5;
int button4Pin = 6;

int buzzerPassiveCurrentKHZ[] = {1000};
int buttonPressStreak[4] = {0, 0, 0, 0};
int buttonPinByNumber[4] = {2, 4, 5, 6};

unsigned long cooldown[5] = {millis(), millis(), millis(), millis(), millis()};

void buzzerPassiveAddKHZ(int which, int howMuch) {
  buzzerPassiveCurrentKHZ[which] = constrain((buzzerPassiveCurrentKHZ[which] + howMuch), 0, 8000);
}

void setup() {
  pinMode(buzzerPassivePin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(button2Pin, INPUT_PULLUP);
  pinMode(button3Pin, INPUT_PULLUP);
  pinMode(button4Pin, INPUT_PULLUP);

  Serial.begin(250000);
}

void loop() {
  Serial.println(String(digitalRead(buttonPin)) + 
                 String(digitalRead(button2Pin)) + 
                 String(digitalRead(button3Pin)) + 
                 String(digitalRead(button4Pin)) + 
                 String(buzzerPassiveCurrentKHZ[0]));

  if(digitalRead(buttonPin) == 0) {
    noTone(buzzerPassivePin);
    tone(buzzerPassivePin, buzzerPassiveCurrentKHZ[0], 1);
  }

  for(int i = 0; i < 4; i++) {
    if((digitalRead(buttonPinByNumber[i]) == 0) && 
       ((millis() - cooldown[i] > 250) || 
        (millis() - cooldown[i] > 1 && buttonPressStreak[i] > 2))) {
      cooldown[i] = millis();
      buttonPressStreak[i]++;

      switch (i) {
        case 1: buzzerPassiveAddKHZ(0, 1); break;
        case 2: buzzerPassiveAddKHZ(0, -1); break;
        case 3: buzzerPassiveCurrentKHZ[0] = 1000; break;
      }

      if(millis() - cooldown[4] > 500) {
        cooldown[4] = millis();
        noTone(buzzerPassivePin);
        tone(buzzerPassivePin, buzzerPassiveCurrentKHZ[0], 500);
      }
    } else if(digitalRead(buttonPinByNumber[i]) == 1) {
      buttonPressStreak[i] = 0;
    }
  }
}
```
## Debugging
- Open the Serial Monitor at 250000 baud.
It will display button states (0 = pressed, 1 = released) and the current buzzer frequency.

## License
**MIT**

Thanks for reading!
