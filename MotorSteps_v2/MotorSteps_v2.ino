#include <Stepper.h>
const int rpm = 10;

const int    stepsPerRev  = 2048; // for the 28BYJ-48 motor - default 2048
const double step60Actual = stepsPerRev / 60.0;  // 2048/60 = 34.13333
const int    step60       = int(floor(step60Actual)); // 34
const int    maxsteps60     = 60;
const int    step60rest     = stepsPerRev - (step60 * maxsteps60) ;

const int BUTTON_UP = 2;
const int LED = 4;
int BUTTON_UPstate = 0;

Stepper minuteStepper (stepsPerRev, 6, 8, 7, 9);

int StepCounter = 1;

void setup() {
  pinMode(BUTTON_UP, INPUT);
  pinMode(LED, OUTPUT);
  
  minuteStepper.setSpeed(rpm);
}

void loop() {
  // Time adjustment buttons
  // when button pressed
     BUTTON_UPstate = digitalRead(BUTTON_UP);
      while (BUTTON_UPstate == HIGH)
      { 
        BUTTON_UPstate = digitalRead(BUTTON_UP);
        digitalWrite(LED, HIGH); // LED ON
        minuteStepper.setSpeed(2);
        minuteStepper.step(step60 / 2 * -1); // backward
        //minuteStepper.step(step60 / 2 * 1); // forward
      } 
     // RESET LED and speed
     minuteStepper.setSpeed(rpm);
     digitalWrite(LED, LOW); // LED OFF  

  // Offset Delay 
   delay(900); // 900 = secs  1800 = mins  21600 = hrs

  // Move stepping motor
   minuteStepper.step(step60);

  // Fix offset problem stepPerRev not fully devided by 60
  if (StepCounter == maxsteps60){minuteStepper.step(step60rest);}

  // Stepcounter
  if (StepCounter < maxsteps60) {StepCounter++;}else{StepCounter = 1;}  
}
