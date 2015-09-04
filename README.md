# NeTibEddie

NeTibEddie is a Lighting effects controller for Nscale Model Railroad using Arduino.  

### Primary Functions:  

1. LED blinking function [ledBlink], for flashing x-mas tree effects
2. LED flickering function [ledFlicker], for flickering LEDs like gas lamps and/or candles
3. LED RGB Crossfading function [rgbCrossfade], for changing an RGB LED colour in a church

### Functions in more detail:  

### ledBlink.
Xmas tree light blinking effects.
This is managed using a shift out register freeing up digital pins on the Arduino. This will also allow for the future addition of various different blinking effects controlled by a momentary button on a voltage ladder. Mode button coming soon.  

ShiftOUT, addressing all LEDs for varying effects and flashing sequences  

    Position  8H        7G       6F      5E      4D      3C     2B     1A  
    Decimal   10000000  1000000  100000  10000   1000    100    10     1  
    Binary    128       64       32      16      8       4      2      1  

Current Shift-Out Values for basic blinking function:  

1st 1000 millis  

    Position  8H    7G    6F    5E    4D    3C    2B    1A  
    State     1     0     1     0     1     0     1     0  
    Value     128         32          8           2          = 85  

2nd 1000 millis  

    Position  8H    7G    6F    5E    4D    3C    2B    1A  
    State     0     1     0     1     0     1     0     1    
    Value           64          16          4           1   = 170  

More effects coming soon (requires additional button input to select).

### ledFlicker.
Gas lamp and/or candle flickering effect.
This is managed using PWM pins on the Arduino. 

LEDs flicker using pulse width modulation with random intervals.

    Duty Cycle: 40% to 50%
    Intervals: 0 to 100 millis

### rgbCrossfade.
RGB LED light colour fader for church lighting effects. This function also requires PWM pins. A momentary button allows for mode changes between automatic and manual function.    

In automatic mode the system picks at random (0 to 255) a new value for each channel (red, green, & blue) every 5000 millis (5 seconds - or near enough), and changes colour. The change is smoothed out for a more gradual transition by slowly incrementing the change (1 step every 10 millis).   

In manual mode the system reads the values of 3 potentiometers and assigns this to the rgb LED allowing manual adjustment of the light colour. The value read from the pots is divided by four as the analog read gives an integer value between 0-1023 while the duty cycle for the LED's requires an output between 0-255. 

### buttonRead.
A new function is required to allow 2 buttons on a voltage divider (ladder) to be read for selecting modes in ledBlink and rgbCrossfade.  
