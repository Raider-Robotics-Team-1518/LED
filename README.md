# LED

2018 - "blinkies" is the project we're using for the bot. It shows random color palettes while the bot is not active, then the alliance color during the game. 

* RoboRio sends high/low signals over inputs 3 & 4 on the arduino.
* Code switches on those pin states:
	* If both low/off - random palettes
	* If one high - all solid red
	* If the other high - all solid blue
	* If both high - all off / black 

See comments in the project code for further details.

----

Some of the LED "pixel" sketches we've been playing with. Some of these are examples from the FastLED library.

You'll need the FastLED library installed. We have found that installing it via the IDE was the only way to make it work. 

In the Arduino IDE, choose Sketch > Include Library > Manage Libraries. Then, search for and include FastLED.

Background info on how RGB "pixel" (addressable LED strips) work can be found at https://docs.google.com/document/d/1kS5UGXYoc85QVdPiJfxRqpg7cZpuU3gIULq6Q6EKniA/edit?hl=en 

The FastLED project is at https://github.com/FastLED/FastLED

Projects:

* cylon - demo project from FastLED
* Example_LED - demo project from FastLED
* switchtest - simple project for reading from a couple of switches. Hook two switches to pins 3 & 4 (and ground). Prints to the serial console which switch is pressed.
* solid_color - starting point for switching between alliance colors and the flashing pattern. Reads from the switches as described above, and turns the LEDs to a solid red, blue, or white.