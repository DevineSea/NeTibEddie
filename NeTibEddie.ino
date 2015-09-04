/*
N NeTibEddie vBeta

Lighting effects controller for Nscale Model Railroad

Primary Functions:
ledBlink, xMas tree blinking effect, ON-OFF-ON-OFF [second set of lights opposite OFF-ON-OFF-ON] sort of thing.
ledFlicker, Gas light/candle effect for lamps with randomized flickering.
rgbCrossfade, Church Lights colour changing effects. Includes Button with Debounce & LED for mode indication.

User Interfaces:
Manual to Automatic modes for rgbCrossfade
Red Green Blue Pot control of rgbCrossfade
[Coming Soon] ledBlink mode changes

ShiftOUT, addressing all LEDs for varying effects and flashing sequences
Possition 8H        7G       6F      5E     4D    3C   2B  1A
Decimal   10000000  1000000  100000  10000  1000  100  10  1
Binary    128       64       32      16     8     4    2   1

Map for address 1 = 85
H G F E D C B A
0 1 0 1 0 1 0 1

Map for address 2 = 170
H G F E D C B A
1 0 1 0 1 0 1 0

Sept 2015, David Wood
*/

// Initiate ledBlink pins for LEDs and Shift Register 74HC595
const int ledBlinkLed = 13;
const int ledBlinkLatch = 8;
const int ledBlinkClock = 7;
const int ledBlinkData = 12;

// Initiate ledBlink Variable for LED set 1 on 74HC595
int ledBlinkAddress1 = 85;
int ledBlinkAddress2 = 170;

// Initiate ledBlink Variables for Led State, time of last change and the Interval time to change states
int ledBlinkState = LOW;
unsigned long timeNow = millis();
unsigned long ledBlinkTimeB4 = 0;
unsigned long ledBlinkInterval = 1000;

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

// Initiate DeBounce button Constants
const int buttonPin = 2;    // the number of the pushbutton pin
const int buttonLed = 4;      // the number of the LED pin

// Initiate Variables for DeBounce
int buttonLedState = LOW;   // the current state of the output pin
int buttonState;             // the current reading from the input pin
int lastButtonState = HIGH;   // the previous reading from the input pin

// Initiate DeBounce variables for time of last change, and interval time to ignor noise
long debounceTimeB4 = 0;
long debounceDelay = 50;


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
  // Setup DeBounce pins
  pinMode(buttonPin, INPUT);
  pinMode(buttonLed, OUTPUT);
  // set deBounce LED initial state
  digitalWrite(buttonLed, buttonLedState);
}


// Main Program, run functions. 
void loop(){
  
  ledBlink();
  ledFlicker();
  rgbCrossfade();
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


// rgbCrossfade function
void rgbCrossfade(){
  
  // DeBounce Button System
  // read the state of the switch into a local variable:
  int reading = digitalRead(buttonPin);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH),  and you've waited
  // long enough since the last press to ignore any noise:  

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    debounceTimeB4 = millis();
  }
 
  if ((millis() - debounceTimeB4) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState == HIGH) {
        buttonLedState = !buttonLedState;
      }
    }
  }
 
  // set the LED:
  digitalWrite(buttonLed, buttonLedState);

  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  lastButtonState = reading;
  
  
  // when the button is not active run automatic mode   
  if (buttonLedState == LOW){
  
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
    
    // When Red LED colours have changed, gentally fade to the new colour
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
  
  else {
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
 }
}
