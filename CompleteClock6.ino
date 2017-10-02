/*The Babybird Numitron IV-9 Clock with TPIC6B595N shift registers. 
This clock does not use an RTC (real time clock) and instead has the
Arduino keep track of time through counting with the millis function. 
Arrays are currently set up to display time in a 12 hour format.
After 12 hours clock was still perfectly in sync with my computers clock. 
The arrays and counter variables are specifically tailored to one 
another in order to get the correct number to display. This clock
includes three buttons: one to change the minutes, one for hours, and
one which will display the temperature in farenheit and humidity. 
Seconds reset to 0 when minutes are changed. Small time loss occurs
when using functions such as temperature when seconds counter
is above 4. This small time loss is outlined more below.*/

//Temperature variables DHT11
#include <dht11.h>  
dht11 DHT;                  //Note: DHT on behalf of the DHT11 sensor 
const int dht11_data = 3;   //Please put the DH11`s dht11_data pin connect with arduino digital Port 6
int temp=0;
int temp1=0;
int temp2=0;
int temp3=0;
int temp4=0;
int hum=0;
int hum1=0;
int hum2=0;
int hum3=0;
int hum4=0;
float tempF = 0.0;

//Define TPIC6B595N pins to the Arduino Uno
#define DATA 6
#define LATCH 8
#define CLOCK 10

//Variables for milisecond counter
unsigned long timeNow = 0;
unsigned long timeLast = 0;

//Define the button pins to arduino and beginning variables
const int buttonPin = 4; // the number of the pushbutton pin 
const int buttonPin2 = 7; // the number of the pushbutton pin 
const int buttonPin3 = 12; // the number of the pushbutton pin
int buttonState = 0; 
int buttonState2 = 0; 
int buttonState3 = 0; 

//Time start Settings for the hour (do not edit)

int startingHour = 0;
int startingHour2 = 0;

// set your starting hour here, not below at int hour. This ensures accurate daily correction of time
//Numbers split into 1-9 increments to be inserted into the various numitron arrays.

int seconds = 0;
int seconds2 = 0;
int seconds3 = 0;

int minutes = 0;
int minutes2 = 2;

int hours = 3;
int hours2 = 3;

int days = 0;

//Accuracy settings

int dailyErrorFast = 0; // set the average number of milliseconds your microcontroller's time is fast on a daily basis

int dailyErrorBehind = 0; // set the average number of milliseconds your microcontroller's time is behind on a daily basis

int correctedToday = 1; // do not change this variable, one means that the time has already been corrected today for the error in your boards crystal. This is true for the first day because you just set the time when you uploaded the sketch.

void setup() { // put your setup code here, to run once:
  //initialize TPIC6B595N pins for Arduino
   pinMode(DATA, OUTPUT);
   pinMode(LATCH, OUTPUT);
   pinMode(CLOCK, OUTPUT);
   // initialize the pushbutton pin as an input:
   pinMode(buttonPin, INPUT);
   pinMode(buttonPin2, INPUT);
   pinMode(buttonPin3, INPUT);
   //begins serial connection
   Serial.begin(9600); }

//Various arrays for displaying numbers on the Numitron. 
//These coincide with specific counter variables to display the correct hour, minute, temperature, etc.

byte fontArray[] = {

B11110110, //0
B11000000, //1
B10100111, //2
B11100011, //3
B11010001, //4
B01110011, //5
B01110111, //6
B11100000, //7
B11110111, //8
B11110001, //9
B00000000, //blank

};

byte fontArray2[] = {

B11000000, //1  0
B10100111, //2  1
B11100011, //3  2
B11010001, //4  3
B01110011, //5  4
B01110111, //6  5
B11100000, //7  6
B11110111, //8  7
B11110001, //9  8
B11110110, //0  9
B11000000, //1  10
B10100111, //2  11

};

byte fontArray3[] = {

B00000000, //blank  0
B00000000, //blank  1
B00000000, //blank  2
B00000000, //blank  3
B00000000, //blank  4
B00000000, //blank  5
B00000000, //blank  6
B00000000, //blank  7
B00000000, //blank  8
B11000000, //1  9
B11000000, //1  10
B11000000, //1  11

};

byte tempArray[] = {

B00000000, //blank  8
B11000000, //1  9
};

void loop() { // This is the main loop of the clock

timeNow = millis()/1000; // the number of milliseconds that have passed since boot

seconds = timeNow - timeLast;

// read the state of the pushbutton value:
buttonState = digitalRead(buttonPin);
buttonState2 = digitalRead(buttonPin2);
buttonState3 = digitalRead(buttonPin3);

  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  
  if (buttonState == HIGH) {
    //adds 1 minute to the minutes counter
    delay(200);
    minutes = minutes +1;
    seconds2 = 0;
    timeLast = timeNow;
  }
  if (buttonState2 == HIGH) {
    //adds hour to the hour counter
    delay(300);
    hours = hours +1;
    hours2 = hours2 +1;
  }

  if (buttonState3 == HIGH) {
  //displays temperature andd humidity read out from DHT11
  //Due to the way the counters are set up if this is used when the seconds counter is at 9 for example,
  //the seconds counter will be at 16 before resetting to 0 and adding 1 to minutes. In effect the clock
  //loses 6 seconds due to this. If temp button is used <4 seconds the clock maintains accuracy. 
  DHT.read(dht11_data);
  //This section includes basic algebra to break down the incoming data to since place values, (since
  //the numitrons do not know how to display "10", but can display 1 & 0. 
  temp=DHT.temperature * 1.8 + 32;
  temp1=temp / 10;
  temp2=(temp1 * 10 - temp);
  temp3=-(temp2);
  temp4=temp / 100;
  hum=DHT.humidity;
  hum1=hum / 10;
  hum2=(hum1 * 10 - hum);
  hum3=-(hum2);
  hum4=hum / 100;
  Serial.print("Hum=\t");            
  Serial.print(hum);
  Serial.print("\tTemp=\t");  
  Serial.println(temp);          
  Serial.println(temp1);
  Serial.println(temp3);
  Serial.println(hum1);
  Serial.println(hum3);

//Displays Temperature
  digitalWrite(LATCH, LOW);  
      shiftOut(DATA, CLOCK, LSBFIRST, B00110101);
      shiftOut(DATA, CLOCK, LSBFIRST, fontArray[temp3]);
      shiftOut(DATA, CLOCK, LSBFIRST, fontArray[temp1]);
      shiftOut(DATA, CLOCK, LSBFIRST, tempArray[temp4]);
  digitalWrite(LATCH, HIGH);
  delay(3000);
//Displays Humidity
 digitalWrite(LATCH, LOW);
     shiftOut(DATA, CLOCK, LSBFIRST, B11010101);
     shiftOut(DATA, CLOCK, LSBFIRST, fontArray[hum3]);
     shiftOut(DATA, CLOCK, LSBFIRST, fontArray[hum1]);
     shiftOut(DATA, CLOCK, LSBFIRST, tempArray[hum4]); 
 digitalWrite(LATCH, HIGH);
 delay(3000);
  }

//the number of seconds that have passed since the last time 10 seconds was reached.
//seconds resets at 10 and adds a 1 to "seconds2"

if (seconds >= 10){

  timeLast = timeNow;

  seconds2 = seconds2 + 1;}
  
//After 6 "seconds2" code adds 1 to minutes and resets seconds2
if (seconds2 >= 6){

  seconds2= 0;

  minutes = minutes +1;}

//After 10 minutes have been counted up it will add 1 to "minutes2" and reset minutes
if (minutes == 10){

  minutes = 0;

  minutes2 = minutes2 + 1; }

//After 6 minutes, 1 is added to hour and minutes reset.
//Animation and temp is played every hour change here. 
if (minutes2 == 6){

  minutes2 = 0;

  hours = hours + 1; 

  hours2 = hours2 + 1;
  
//Variables for temperature to be display every hour
DHT.read(dht11_data);
  temp=DHT.temperature * 1.8 + 32;
  temp1=temp / 10;
  temp2=(temp1 * 10 - temp);
  temp3=-(temp2);
  temp4=temp / 100;

  //Hour change animation
    digitalWrite(LATCH, LOW);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000001);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000001);
    digitalWrite(LATCH, HIGH);
    delay(250);

    digitalWrite(LATCH, LOW); 
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000001);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000001);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
     digitalWrite(LATCH, HIGH);
     delay(250);

     digitalWrite(LATCH, LOW);
          shiftOut(DATA, CLOCK, LSBFIRST, B00000001);
          shiftOut(DATA, CLOCK, LSBFIRST, B00000001);
          shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
          shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
     digitalWrite(LATCH, HIGH);
     delay(250);
     
     digitalWrite(LATCH, LOW);
          shiftOut(DATA, CLOCK, LSBFIRST, B11000001);
          shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
          shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
          shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
     digitalWrite(LATCH, HIGH);
     delay(250);
     
     digitalWrite(LATCH, LOW);
          shiftOut(DATA, CLOCK, LSBFIRST, B11100010);
          shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
          shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
          shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
     digitalWrite(LATCH, HIGH);
     delay(250);

     digitalWrite(LATCH, LOW);
          shiftOut(DATA, CLOCK, LSBFIRST, B00100010);
          shiftOut(DATA, CLOCK, LSBFIRST, B00100010);
          shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
          shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
     digitalWrite(LATCH, HIGH);
     delay(250);

     digitalWrite(LATCH, LOW);
          shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
          shiftOut(DATA, CLOCK, LSBFIRST, B00100010);
          shiftOut(DATA, CLOCK, LSBFIRST, B00100010);
          shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
     digitalWrite(LATCH, HIGH);
     delay(250);

     digitalWrite(LATCH, LOW);
          shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
          shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
          shiftOut(DATA, CLOCK, LSBFIRST, B00100010);
          shiftOut(DATA, CLOCK, LSBFIRST, B00100010);
     digitalWrite(LATCH, HIGH);
     delay(250);

     digitalWrite(LATCH, LOW);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
         shiftOut(DATA, CLOCK, LSBFIRST, B00110110);
     digitalWrite(LATCH, HIGH);
     delay(200);

     digitalWrite(LATCH, LOW);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
         shiftOut(DATA, CLOCK, LSBFIRST, B00110111);
     digitalWrite(LATCH, HIGH);
     delay(200);

     digitalWrite(LATCH, LOW);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000001);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000001);
     digitalWrite(LATCH, HIGH);
     delay(250);

     digitalWrite(LATCH, LOW);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000001);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000001);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
     digitalWrite(LATCH, HIGH);
     delay(250);


     digitalWrite(LATCH, LOW);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000001);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000001);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
     digitalWrite(LATCH, HIGH);
     delay(250);
     
     digitalWrite(LATCH, LOW);
         shiftOut(DATA, CLOCK, LSBFIRST, B11000001);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
     digitalWrite(LATCH, HIGH);
     delay(250);

     digitalWrite(LATCH, LOW);
         shiftOut(DATA, CLOCK, LSBFIRST, B11100010);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
     digitalWrite(LATCH, HIGH);
     delay(250);

     digitalWrite(LATCH, LOW);
         shiftOut(DATA, CLOCK, LSBFIRST, B00100010);
         shiftOut(DATA, CLOCK, LSBFIRST, B00100010);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
     digitalWrite(LATCH, HIGH);
     delay(250);

     digitalWrite(LATCH, LOW);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
         shiftOut(DATA, CLOCK, LSBFIRST, B00100010);
         shiftOut(DATA, CLOCK, LSBFIRST, B00100010);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
     digitalWrite(LATCH, HIGH);
     delay(250);
     
     digitalWrite(LATCH, LOW);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
         shiftOut(DATA, CLOCK, LSBFIRST, B00100010);
         shiftOut(DATA, CLOCK, LSBFIRST, B00100010);
     digitalWrite(LATCH, HIGH);
     delay(250);

     digitalWrite(LATCH, LOW);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
         shiftOut(DATA, CLOCK, LSBFIRST, B00110110);
     digitalWrite(LATCH, HIGH);
     delay(200);

     digitalWrite(LATCH, LOW);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
         shiftOut(DATA, CLOCK, LSBFIRST, B00110111);
     digitalWrite(LATCH, HIGH);
     delay(200);

     digitalWrite(LATCH, LOW);
          shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
          shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
          shiftOut(DATA, CLOCK, LSBFIRST, B00000001);
          shiftOut(DATA, CLOCK, LSBFIRST, B00000001);
     digitalWrite(LATCH, HIGH);
     delay(100);

     digitalWrite(LATCH, LOW);
          shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
          shiftOut(DATA, CLOCK, LSBFIRST, B00000001);
          shiftOut(DATA, CLOCK, LSBFIRST, B00000001);
          shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
     digitalWrite(LATCH, HIGH);
     delay(100);

     //Temperature display every hour
     digitalWrite(LATCH, LOW);
          shiftOut(DATA, CLOCK, LSBFIRST, B00110101);
          shiftOut(DATA, CLOCK, LSBFIRST, fontArray[temp3]);
          shiftOut(DATA, CLOCK, LSBFIRST, fontArray[temp1]);
          shiftOut(DATA, CLOCK, LSBFIRST, tempArray[temp4]);
     digitalWrite(LATCH, HIGH);
     delay(3000);

//This animation and temp section uses 8 seconds, but is fully accounted for in the counter. 
//Increasing this above 10 seconds will result in lost time.

}
//When hours reach 12 the hour counters will reset.
if (hours == 12){
  hours = 0;
  hours2 = 0;
}

// After 24 hours adds 1 day to the clock (not used in this current setup)
if (hours == 24){
hours = 0;
days = days + 1;
}

//if 24 hours have passed, add one day
if (hours ==(24 - startingHour) && correctedToday == 0);{
delay(dailyErrorFast*1000);
seconds = seconds + dailyErrorBehind;
correctedToday = 1; }

//every time 24 hours have passed since the initial starting time and it has not been reset this day before, add milliseconds or delay the program with some milliseconds.

//Change these varialbes according to the error of your board.

// The only way to find out how far off your boards internal clock is, is by uploading this sketch at exactly the same time as the real time, letting it run for a few days

// and then determining how many seconds slow/fast your boards internal clock is on a daily average. (24 hours).

if (hours == 24 - startingHour + 2);{
correctedToday = 0; }

//let the sketch know that a new day has started for what concerns correction, if this line was not here the arduiono // would continue to correct for an entire hour that is 24 - startingHour.


//This section displays the current time onto the Numitrons using the counters plugged into the byte arrays.  
    digitalWrite(LATCH, LOW);
        shiftOut(DATA, CLOCK, LSBFIRST, fontArray[minutes]);
        shiftOut(DATA, CLOCK, LSBFIRST, fontArray[minutes2]);
        shiftOut(DATA, CLOCK, LSBFIRST, fontArray2[hours]);
        shiftOut(DATA, CLOCK, LSBFIRST, fontArray3[hours2]);
    digitalWrite(LATCH, HIGH);

//Serial for debugging.    
Serial.print("The time is: ");
Serial.print(days);
Serial.print(":");
Serial.print(hours);
Serial.print(":");
Serial.print(minutes);
Serial.print(":");
Serial.print(seconds2);
Serial.print(":");
Serial.println(seconds);
}

