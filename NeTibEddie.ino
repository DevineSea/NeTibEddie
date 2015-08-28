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
}


// Main Program, run functions. 
void loop(){
  
  ledBlink();
  ledFlicker();
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


// ledFlicker Function
// Use random values, and add 135 so Leds are always over 50% duty cycle
// Create random delay to slow flicker effect
void ledFlicker(){
  
  // Check the time, see if it's time to flicker, flicker, record the time, and reassign the interval
  TimeNow = millis();
  
  // ledFlicker1
  if (TimeNow - ledFlickerTimeB41 > ledFlickerInterval1){
    ledFlickerTimeB41 = TimeNow;
    ledFlickerInterval1 = random(100);
    
   // Flicker the Led randomly from 50% to 100% duty cycle
    analogWrite(ledFlicker1, random(100, 255));
  } 
    
  // ledFlicker2
  if (TimeNow - ledFlickerTimeB42 > ledFlickerInterval2){
    ledFlickerTimeB42 = TimeNow;
    ledFlickerInterval2 = random(100);
    
   // Flicker the Led randomly from 50% to 100% duty cycle
    analogWrite(ledFlicker2, random(100, 255));
  }
  
    // ledFlicker3
  if (TimeNow - ledFlickerTimeB43 > ledFlickerInterval3){
    ledFlickerTimeB43 = TimeNow;
    ledFlickerInterval3 = random(100);
    
   // Flicker the Led randomly from 50% to 100% duty cycle
    analogWrite(ledFlicker3, random(100, 255));
  }
}
