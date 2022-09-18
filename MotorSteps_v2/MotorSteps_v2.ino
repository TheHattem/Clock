#include <Stepper.h>
const int rpm = 10;

const int    stepsPerRev  = 2048; // for the 28BYJ-48 motor - default 2048
const double step60Actual = stepsPerRev / 60.0;  // 2048/60 = 34.13333
const int    step60       = int(floor(step60Actual)); // 34
const int    maxsteps60     = 60;
const int    step60rest     = stepsPerRev - (step60 * maxsteps60) ;

// Delay counters 1= 60 sec  60= 1 min
const int MaxDelayCounter = 1;

const int BUTTON_UP = 2;
const int LED = 4;
int BUTTON_UPstate = 0;

Stepper minuteStepper (stepsPerRev, 6, 8, 7, 9);

int StepCounter = 1;
int DelayCounter = 1;

void setup() {
// setup Arduino Debug 
  Serial.begin(9600);
  
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
    if(DelayCounter ==1){
        // Move stepping motor
        minuteStepper.step(step60);
    }
    // DelayCounter
    if (DelayCounter < MaxDelayCounter) {DelayCounter=DelayCounter+1;}else{DelayCounter = 1;}
    // Delay 900 = secs  1800 = mins  21600 = hrs
    delay(1000);

  // Fix offset problem stepPerRev not fully devided by 60
  if (StepCounter == maxsteps60){minuteStepper.step(step60rest);}

  // Stepcounter
  if (StepCounter < maxsteps60) {StepCounter++;}else{StepCounter = 1;}  

  Serial.println((String)"StepCounter: "+StepCounter+" DelayCounter: "+DelayCounter);
}
