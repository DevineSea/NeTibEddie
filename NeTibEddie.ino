/*
N NeTibEddie vBeta
Lighting effects controller for Nscale Model Railroad

April 2017, David Wood
*/

// Initiate ledBlink pins for LEDs and Shift Register 74HC595
const int ledBlinkLed = 13;
const int ledBlinkLatch = 8;
const int ledBlinkClock = 7;
const int ledBlinkData = 12;

// Initiate ledBlink Variable for LEDs on 74HC595
int ledBlinkAddress1 = 85;
int ledBlinkAddress2 = 170;

// Initiate ledSolid Variable for LEDs on 74HC595
int ledSolidAddress1 = 255;

// Initiate ledBlink Variables for Led State, time of last change and the Interval time to change states
int ledBlinkState = LOW;
unsigned long timeNow = millis();
unsigned long ledBlinkTimeB4 = 0;
unsigned long ledBlinkInterval = 1000;

// Initiate timing variables and constants for buttonRead function
unsigned long btnFtnInterval = 2000;
unsigned long btnOneFtnOneTimeB4 = 0;
unsigned long btnOneFtnTwoTimeB4 = 0;
unsigned long btnTwoFtnOneTimeB4 = 0;
unsigned long btnTwoFtnTwoTimeB4 = 0;
unsigned long btnDebounceTimeB4 = 0;
unsigned long btnDebounceDelay = 200;

// Initiate button variables and constants for buttonRead function
int btnOne = 0;
int btnTwo = 145;
int btnVolts = 1024;
const int btnTollerance = 50;

// Initiate button State variables
int btnOneState = 1;
int btnTwoState = 1;

// Initiate ledFlicker pins for LEDs [Need to be PWM pins]
const int ledFlicker1 = 3;
const int ledFlicker2 = 5;
const int ledFlicker3 = 6;

// Initiate ledFlicker variables for time of last change, and random interval time to change light level
unsigned long ledFlickerTimeB41 = 0;
unsigned long ledFlickerTimeB42 = 0;
unsigned long ledFlickerTimeB43 = 0;
unsigned long ledFlickerInterval1 = random(100);
unsigned long ledFlickerInterval2 = random(100);
unsigned long ledFlickerInterval3 = random(100);

// Initiate rgbCrossfade pins for Leds [Need to be PWM pins]
const int rgbRed = 11;
const int rgbGrn = 10;
const int rgbBlu = 9;

// Initiate rgbCrossfade variables for time of last change, and interval time to change light level
unsigned long rgbTimeB4 = 0;
unsigned long rgbInterval = 5000;
unsigned long rgbFadeTimeB4 = 0;
unsigned long rgbFadeInterval = 10;

// Initiate assignable values for current & next rgb colour mix.
int rgbRedOld = 0;
int rgbGrnOld = 0;
int rgbBluOld = 0;

int rgbRedNew = random(0, 255);
int rgbGrnNew = random(0, 255);
int rgbBluNew = random(0, 255);

// Initiate rgbCrossfade variables for manual operation
int redPot;
int grnPot;
int bluPot;
int redLed;
int grnLed;
int bluLed;

void setup(){
  
  // Setup ledBlink pins to be outputs
  pinMode(ledBlinkLed, OUTPUT);
  pinMode(ledBlinkLatch, OUTPUT);
  pinMode(ledBlinkClock, OUTPUT);
  pinMode(ledBlinkData, OUTPUT);
  // Setup ledFlicker pins to be outputs
  pinMode(ledFlicker1, OUTPUT);
  pinMode(ledFlicker2, OUTPUT);
  pinMode(ledFlicker3, OUTPUT);
  // Setup rgbCrossfade pins to be outputs
  pinMode(rgbRed, OUTPUT);
  pinMode(rgbGrn, OUTPUT);
  pinMode(rgbBlu, OUTPUT);
}


// Main Program, run functions. 
void loop(){
  
  // Run ledFlicker function
  ledFlicker();
  
  // Run button read function
  buttonRead();
  
  // Button One mode selection and run ledBlink or ledSolid
  switch (btnOneState){
    case 1:
    ledBlink();
    break;
    case 2:
    ledSolid();
    break;
  }
  
  // Button Two mode selection and run rgbCrossfadeAuto or rgbCrossfadeMan
  switch (btnTwoState){
    case 1:
    rgbCrossfadeAuto();
    break;
    case 2:
    rgbCrossfadeMan();
    break;
    }
}


// ledBlink function
void ledBlink(){

  // Light the tree top LED
  digitalWrite(ledBlinkLed, HIGH);
  
  // Check the time, see if it's time to blink, blink, change the state and record the time
  timeNow = millis();
  
  if (timeNow - ledBlinkTimeB4 > ledBlinkInterval){
    ledBlinkTimeB4 = timeNow;
    
    // if the Led is off turn it on and vice-versa
    if (ledBlinkState == LOW){
    ledBlinkState = HIGH;
    
    // take the latchPin low so the LEDs don't change while sending bits
    digitalWrite(ledBlinkLatch, LOW);
  
    //ShiftOUT address 1
    shiftOut(ledBlinkData, ledBlinkClock, MSBFIRST, ledBlinkAddress1);
    
    // take latchPin high to light up the LEDs
    digitalWrite(ledBlinkLatch, HIGH);  
    }
    
    else{
    ledBlinkState = LOW;
    
    // take the latchPin low so the LEDs don't change while sending bits
    digitalWrite(ledBlinkLatch, LOW);
  
    //ShiftOUT address 2
    shiftOut(ledBlinkData, ledBlinkClock, MSBFIRST, ledBlinkAddress2);
    
    // take latchPin high to light up the LEDs
    digitalWrite(ledBlinkLatch, HIGH);  
    }
  }
}


// ledSolid function
void ledSolid(){

  // Light the tree top LED
  digitalWrite(ledBlinkLed, HIGH);
  
  // take the latchPin low so the LEDs don't change while sending bits
  digitalWrite(ledBlinkLatch, LOW);
  
  //ShiftOUT address 1
  shiftOut(ledBlinkData, ledBlinkClock, MSBFIRST, ledSolidAddress1);
    
  // take latchPin high to light up the LEDs
  digitalWrite(ledBlinkLatch, HIGH);  
}


void buttonRead(){
  // button read function
  
  // read voltage levels of button ladder
  btnVolts = analogRead(A3);
  
  // Debounce button for stability
  if(timeNow > btnDebounceTimeB4 + btnDebounceDelay){
    
    // Check if a button has been pressed and change States
    if(btnVolts < btnOne + btnTollerance){
      ++ btnOneState;
      btnDebounceTimeB4 = millis();
      }
  
    if(btnVolts >= btnTwo - btnTollerance && btnVolts <= btnTwo + btnTollerance){
      ++ btnTwoState;
      btnDebounceTimeB4 = millis();
    }
  
    // Make sure Button States don't exceed number of functions
    if(btnOneState >= 3){
      btnOneState = 1;
    }
    if(btnTwoState >= 3){
      btnTwoState = 1;
    }
  }
}


// ledFlicker Function
// Use random values, from 100 to 255, so Leds are between 40% and 100% duty cycle
// Create random delay to slow flicker effect
void ledFlicker(){
  
  // Check the time, see if it's time to flicker, flicker, record the time, and reassign the interval
  timeNow = millis();
  
  // ledFlicker1
  if (timeNow - ledFlickerTimeB41 > ledFlickerInterval1){
    ledFlickerTimeB41 = timeNow;
    ledFlickerInterval1 = random(100);
    
   // Flicker the Led randomly from 40% to 100% duty cycle
    analogWrite(ledFlicker1, random(100, 255));
  } 
    
  // ledFlicker2
  if (timeNow - ledFlickerTimeB42 > ledFlickerInterval2){
    ledFlickerTimeB42 = timeNow;
    ledFlickerInterval2 = random(100);
    
   // Flicker the Led randomly from 40% to 100% duty cycle
    analogWrite(ledFlicker2, random(100, 255));
  }
  
    // ledFlicker3
  if (timeNow - ledFlickerTimeB43 > ledFlickerInterval3){
    ledFlickerTimeB43 = timeNow;
    ledFlickerInterval3 = random(100);
    
   // Flicker the Led randomly from 40% to 100% duty cycle
    analogWrite(ledFlicker3, random(100, 255));
  }
}


// rgbCrossfadeAuto function
void rgbCrossfadeAuto(){
  
  // Check the time, see if it's time to change colour and change, record the time.
  timeNow = millis();
  
  // If correct interval has passed asign new colours to the RGB LEDs, and reset the time
  if (timeNow - rgbTimeB4 > rgbInterval){
    rgbTimeB4 = timeNow; 
  
    // Pick new random colours for the RGB LEDs
    rgbRedNew = random(0, 255);
    rgbGrnNew = random(0, 255);
    rgbBluNew = random(0, 255);
    }
    
    // When LED colours have changed, gentally fade to the new colour
  if (rgbRedOld != rgbRedNew || rgbGrnOld != rgbGrnNew || rgbBluOld != rgbBluNew){
    
    // If correct interval has passed asign new colours to the RGB LEDs, and reset the time
    if (timeNow - rgbFadeTimeB4 > rgbFadeInterval){
      rgbFadeTimeB4 = timeNow; 
      
      // Check and Change Red LED on step up or down
       if (rgbRedOld < rgbRedNew){
         ++ rgbRedOld;
         analogWrite(rgbRed, rgbRedOld);
       }
       if (rgbRedOld > rgbRedNew){
         -- rgbRedOld;
         analogWrite(rgbRed, rgbRedOld);
       }
       
       // Check and Change Green LED on step up or down
       if (rgbGrnOld < rgbGrnNew){
         ++ rgbGrnOld;
         analogWrite(rgbGrn, rgbGrnOld);
       }
       if (rgbGrnOld > rgbGrnNew){
         -- rgbGrnOld;
         analogWrite(rgbGrn, rgbGrnOld);
       }
       
       // Check and Change Blue LED on step up or down
       if (rgbBluOld < rgbBluNew){
         ++ rgbBluOld;
         analogWrite(rgbBlu, rgbBluOld);
       }
       if (rgbBluOld > rgbBluNew){
         -- rgbBluOld;
         analogWrite(rgbBlu, rgbBluOld);
       }
      }
    }

    // Change rgbCrossfade Leds Colour
    analogWrite(rgbRed, redLed);
    analogWrite(rgbGrn, grnLed);
    analogWrite(rgbBlu, bluLed);
}


// rgbCrossfadeMan function
void rgbCrossfadeMan(){
  
  // Read the Pot levels, divide by 4 for light levels
  redPot = analogRead(A0);
  redLed = redPot / 4;
  grnPot = analogRead(A1);
  grnLed = grnPot / 4;
  bluPot = analogRead(A2);
  bluLed = bluPot / 4;
  
  // Change rgbCrossfade Leds Colour
  analogWrite(rgbRed, redLed);
  analogWrite(rgbGrn, grnLed);
  analogWrite(rgbBlu, bluLed);
}
