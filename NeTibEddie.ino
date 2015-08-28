/*
N NeTibEddie vAlpha

Lighting effects controller for Nscale Model Railroad

Primary Functions:
ledBlink, xMas tree blinking effect, ON-OFF-ON-OFF [second set of lights opposite OFF-ON-OFF-ON] sort of thing.
ledFlicker, Gas light/candle effect for lamps with randomized flickering.
rgbCrossfade, Church Lights colour changing effects. Includes Button with Debounce & LED for mode indication.

June 2014, David Wood
*/

// Initiate ledBlink pins for Leds
const int ledBlink1 = 13;
const int ledBlink2 = 12;
const int ledBlink3 = 8;
const int ledBlink4 = 7;

// Initiate ledBlink Variables for Led State, time of last change and the Interval time to change states
int ledStateOne = LOW;
int ledStateTwo = HIGH;
unsigned long timeNow = millis();
unsigned long ledTimeB4 = 0;
unsigned long ledInterval = 1000;

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
const int rgbCrossRed = 11;
const int rgbCrossGrn = 10;
const int rgbCrossBlu = 9;

// Initiate rgbCrossfade variables for time of last change, and interval time to change light level
unsigned long rgbTimeB4 = 0;
unsigned long rgbInterval = 3000;

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
  pinMode(ledBlink1, OUTPUT);
  pinMode(ledBlink2, OUTPUT);
  pinMode(ledBlink3, OUTPUT);
  pinMode(ledBlink4, OUTPUT);
  // Setup ledFlicker pins to be outputs
  pinMode(ledFlicker1, OUTPUT);
  pinMode(ledFlicker2, OUTPUT);
  pinMode(ledFlicker3, OUTPUT);
  // Setup rgbCrossfade pins to be outputs
  pinMode(rgbCrossRed, OUTPUT);
  pinMode(rgbCrossGrn, OUTPUT);
  pinMode(rgbCrossBlu, OUTPUT);
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
  
  // Check the time, see if it's time to blink, blink, change the state and record the time
  timeNow = millis();
  
  if (timeNow - ledTimeB4 > ledInterval){
    ledTimeB4 = timeNow;
    
    // if the Led is off turn it on and vice-versa
    if (ledStateOne == LOW)
    ledStateOne = HIGH;
    else
    ledStateOne = LOW;
    
    if (ledStateTwo == HIGH)
    ledStateTwo = LOW;
    else
    ledStateTwo = HIGH;
        
    // set the Led's with the ledSate
    digitalWrite(ledBlink1, ledStateOne);
    digitalWrite(ledBlink2, ledStateTwo);
    digitalWrite(ledBlink3, ledStateOne);
    digitalWrite(ledBlink4, ledStateTwo);
  }
}


// ledFlicker Function
// Use random values, and add 135 so Leds are always over 50% duty cycle
// Create random delay to slow flicker effect
void ledFlicker(){
  
  // Check the time, see if it's time to flicker, flicker, record the time, and reassign the interval
  timeNow = millis();
  
  // ledFlicker1
  if (timeNow - ledFlickerTimeB41 > ledFlickerInterval1){
    ledFlickerTimeB41 = timeNow;
    ledFlickerInterval1 = random(100);
    
   // Flicker the Led randomly from 50% to 100% duty cycle
    analogWrite(ledFlicker1, random(100, 255));
  } 
    
  // ledFlicker2
  if (timeNow - ledFlickerTimeB42 > ledFlickerInterval2){
    ledFlickerTimeB42 = timeNow;
    ledFlickerInterval2 = random(100);
    
   // Flicker the Led randomly from 50% to 100% duty cycle
    analogWrite(ledFlicker2, random(100, 255));
  }
  
    // ledFlicker3
  if (timeNow - ledFlickerTimeB43 > ledFlickerInterval3){
    ledFlickerTimeB43 = timeNow;
    ledFlickerInterval3 = random(100);
    
   // Flicker the Led randomly from 50% to 100% duty cycle
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
  
  if (timeNow - rgbTimeB4 > rgbInterval){
    rgbTimeB4 = timeNow; 
  
    // Change the colour to a new random colour
    analogWrite(rgbCrossRed, random(0, 255));
    analogWrite(rgbCrossGrn, random(0, 255));
    analogWrite(rgbCrossBlu, random(0, 255));
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
    analogWrite(rgbCrossRed, redLed);
    analogWrite(rgbCrossGrn, grnLed);
    analogWrite(rgbCrossBlu, bluLed);
  }
}
