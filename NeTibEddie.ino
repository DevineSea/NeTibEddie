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
unsigned long TimeNow = millis();
unsigned long ledTimeB4 = 0;
unsigned long ledInterval = 1000;


void setup(){
  
  // Setup ledBlink pins to be outputs
  pinMode(ledBlink1, OUTPUT);
  pinMode(ledBlink2, OUTPUT);
  pinMode(ledBlink3, OUTPUT);
  pinMode(ledBlink4, OUTPUT);
}


// Main Program, run functions. 
void loop(){
  
  ledBlink();
//  ledFlicker();      <<<< COMING SOON >>>>
//  rgbCrossfade();    <<<< COMING SOON >>>>
}


// ledBlink function
void ledBlink(){
  
  // Check the time, see if it's time to blink, blink, change the state and record the time
  TimeNow = millis();
  
  if (TimeNow - ledTimeB4 > ledInterval){
    ledTimeB4 = TimeNow;
    
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
