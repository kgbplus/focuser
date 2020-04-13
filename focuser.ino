/* 
  Copyright 2020 Roman Mindlin <Roman@Mindlin.ru>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
  MA 02110-1301, USA.
*/

// Include the Arduino Stepper.h library:
#include <Stepper.h>

// Arduino pin numbers
const int SW_pin = 11; // digital pin connected to switch output
const int X_pin = 0; // analog pin connected to X output
const int Y_pin = 1; // analog pin connected to Y output
const int ENLED = 13; // Enable led

// Define number of steps per rotation:
const int stepsPerRevolution = 2048;

// Create stepper object called 'myStepper', note the pin order:
Stepper myStepper = Stepper(stepsPerRevolution, 4, 6, 5, 7);

bool enable=true;

void setup() {
  // Set the speed to 5 rpm:
  myStepper.setSpeed(5);

  // Init joystick
  pinMode(SW_pin, INPUT);
  digitalWrite(SW_pin, HIGH);

  // Init enable led
  pinMode(ENLED, OUTPUT);
  digitalWrite(ENLED, HIGH);
  
  // Begin Serial communication at a baud rate of 9600:
  Serial.begin(9600);
}

int joy_X;
int speed;
int step;

static inline int8_t sgn(int val) {
 if (val < 0) return -1;
 if (val==0) return 0;
 return 1;
}

void loop() {
  //enable-disable button
  if (!digitalRead(SW_pin)) {
    delay(10);
    if (!digitalRead(SW_pin)) {
      enable=!enable;
      if (enable) {
        digitalWrite(ENLED, HIGH);
      }
      else {
        digitalWrite(ENLED, LOW);
      }
      while (!digitalRead(SW_pin)) delay(100);
    }
  }

  if (!enable) {
    delay(100);
    return;
  }

  // read joystick
  joy_X=analogRead(X_pin);
  step = sgn(joy_X - 500);
  speed=0;

  //calculate speed and step
  if (joy_X < 10) {
    speed=10;
    step*=4;
  }
  else if (joy_X>=10 and joy_X<200) speed=3;
  else if (joy_X>=200 and joy_X<300) speed=1;
  else if (joy_X>=300 and joy_X<490) {
    speed=1;
    delay(100);
  }
  else if (joy_X>=510 and joy_X<700) {
    speed=1;
    delay(100);
  }
  else if (joy_X>=700 and joy_X<800) speed=1;
  else if (joy_X>=800 and joy_X<990) speed=3;
  else if (joy_X>=990) {
    speed=10;
    step*=4;
  }
    
  Serial.print("   speed=");
  Serial.print(speed);
  Serial.print("  step=");
  Serial.print(step);
  Serial.print("\n");

  // stop motor
  if(!speed) return;

  myStepper.setSpeed(speed);
  myStepper.step(step);
}
