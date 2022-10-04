#include <Stepper.h>
const int rpm = 10;
const int    stepsPerRev  = 2048; // for the 28BYJ-48 motor - default 2048

// Standard settings
const int DefaultDelay = 1000; //ms
int DelayValue = DefaultDelay;
const int DirectionDefault = -1;
int DirectionValue = DirectionDefault;

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

const int BUTTON_UP = A1;
int BUTTON_UPstate = 0;
int BUTTON_UPValue = 0;
  
const int BUTTON_DOWN = A2;
int BUTTON_DOWNstate = 0;
int BUTTON_DOWNValue = 0;

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
  pinMode(BUTTON_DOWN, INPUT);
    
  minuteStepper.setSpeed(rpm);
  hourStepper.setSpeed(rpm);
}

void loop() {
  // read the value from the button:
  BUTTON_UPValue = analogRead(BUTTON_UP);
  BUTTON_DOWNValue = analogRead(BUTTON_DOWN);
    
// Set speed and direction when button is pushed
    DelayValue = DefaultDelay;
    DirectionValue = DirectionDefault;
    if ((BUTTON_UPValue > 100)&&(BUTTON_DOWNValue < 100)){DelayValue = 100;DirectionValue = DirectionDefault;}
    if ((BUTTON_DOWNValue > 100)&&(BUTTON_UPValue < 100)){DelayValue = 100;DirectionValue = 1;}


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
        minuteStepper.step(minuteSteps*DirectionValue);
        hourStepper.step(hourSteps*DirectionValue);
    }
    // DelayCounter
    if (DelayCounter < MaxDelayCounter) {DelayCounter=DelayCounter+1;}else{DelayCounter = 1;}
    delay(DelayValue);

  // Stepcounter
  if (StepCounter < step60max) {StepCounter++;}else{StepCounter = 1;}  

  Serial.println((String)"StepCounter: "+StepCounter+" DelayCounter: "+DelayCounter+" hourSteps:"+hourSteps+"UP:"+BUTTON_UPValue+" DOWN:"+BUTTON_DOWNValue);
}
