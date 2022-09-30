#include <Stepper.h>
const int rpm = 10;

const int    stepsPerRev  = 2048; // for the 28BYJ-48 motor - default 2048

// Steps for seconds or Minutes
const double step60Actual = stepsPerRev / 60.0;  // 2048/60 = 34.13333
const int    step60       = int(floor(step60Actual)); // 34
const int    step60max     = 60;
const int    step60rest     = stepsPerRev - (step60 * step60max) ; // 8
const int    step60fixCount = floor(step60max / step60rest );

// Steps for hour
const int step5ForHour = 3;  // 34*5 =170   170 / (step60) = 3
const int step5ForHourRest = 10;
const int step5ForHourfixCount = floor(step60max / step5ForHourRest );

// Delay counters 1= 60 sec  60= 1 min
const int MaxDelayCounter = 1;
const int MaxDelayCounterMins = 60;

const int BUTTON_UP = 2;
const int LED = 4;
int BUTTON_UPstate = 0;

int minuteSteps;
int hourSteps;
//Stepper minuteStepper (stepsPerRev, 2, 4, 3, 5);
//Stepper hourStepper (stepsPerRev, 6, 8, 7, 9);
Stepper hourStepper (stepsPerRev, 2, 4, 3, 5);
Stepper minuteStepper (stepsPerRev, 6, 8, 7, 9);


int StepCounter = 0;
int DelayCounter = 1;
int TempVar1 = 1;

void setup() {
// setup Arduino Debug 
  Serial.begin(9600);
  
  pinMode(BUTTON_UP, INPUT);
  pinMode(LED, OUTPUT);
  
  minuteStepper.setSpeed(rpm);
  hourStepper.setSpeed(rpm);
}

void loop() {
  // Time adjustment buttons
  // when button pressed
  /*
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
*/

  // Fix offset problem stepPerRev not fully devided by 60
  if ((StepCounter % step60fixCount)==0){
        minuteSteps = step60+1;
      }else{
        {minuteSteps = step60;}
      };
  if ((StepCounter % step5ForHourfixCount)==0){
        hourSteps = step5ForHour-1;
      }else{
        {hourSteps = step5ForHour;}
      };
  
    // Delay 
    if(DelayCounter ==1){
        // Move stepping motor
        minuteStepper.step(-minuteSteps);
        hourStepper.step(-hourSteps);
    }
    // DelayCounter
    if (DelayCounter < MaxDelayCounter) {DelayCounter=DelayCounter+1;}else{DelayCounter = 1;}
    delay(1000);

  // Stepcounter
  if (StepCounter < step60max) {StepCounter++;}else{StepCounter = 1;}  

  Serial.println((String)"StepCounter: "+StepCounter+" DelayCounter: "+DelayCounter+" hourSteps:"+hourSteps);
}
