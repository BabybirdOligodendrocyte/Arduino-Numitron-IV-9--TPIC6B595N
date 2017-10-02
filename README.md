# Arduino-Numitron-IV-9--TPIC6B595N

The Babybird Numitron IV-9 Clock with TPIC6B595N shift registers. This clock does not use an RTC (real time clock) and instead has the Arduino keep track of time through counting with the millis function. Arrays are currently set up to display time in a 12 hour format.After 12 hours clock was still perfectly in sync with my computers clock. The arrays and counter variables are specifically tailored to one another in order to get the correct number to display. This clockincludes three buttons: one to change the minutes, one for hours, andone which will display the temperature in farenheit and humidity. Seconds reset to 0 when minutes are changed. Small time loss occurswhen using functions such as temperature when seconds counteris above 4. This small time loss is outlined more below.

Thank you to the coders whose code I scaffolded this on and forgot the names of.
