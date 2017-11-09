/* 
 * TimeSerial.pde
 * example code illustrating Time library set through serial port messages.
 *
 * Messages consist of the letter T followed by ten digit time (as seconds since Jan 1 1970)
 * you can send the text on the next line using Serial Monitor to set the clock to noon Jan 1 2013
 T1357041600  
 *
 * A Processing example sketch to automatically send the messages is included in the download
 * On Linux, you can use "date +T%s\n > /dev/ttyACM0" (UTC time zone)
 */ 
 
#include <TimeLib.h>
#define TIME_REQUEST  7    // ASCII bell character requests a time sync message 

int wait = 0;
int oldhour = 0;
long ran;
long ran2;
long ran3;

//Define TPIC6B595N pins to the Arduino Uno
#define DATA 6
#define LATCH 8
#define CLOCK 10

void setup()  {
    //initialize TPIC6B595N pins for Arduino
   pinMode(DATA, OUTPUT);
   pinMode(LATCH, OUTPUT);
   pinMode(CLOCK, OUTPUT);

  Serial.begin(9600);
  while (!Serial) ; // Needed for Leonardo only
  pinMode(13, OUTPUT);
  setSyncProvider( requestSync);  //set function to call when sync required
  Serial.println("Waiting for sync message");


}

byte fontArray1[] = {

B01111101, //0
B01010000, //1
B00111110, //2
B01110110, //3
B01010011, //4
B01100111, //5
B01101111, //6
B01110000, //7
B01111111, //8
B01110011, //9
B01111101, //0
B01010000, //1
B00111110, //2
B01110110, //3
B01010011, //4
B01100111, //5
B01101111, //6
B01110000, //7
B01111111, //8
B01110011, //9
B01111101, //0
B01010000, //1
B00111110, //2
B01110110, //3
B01010011, //4
B01100111, //5
B01101111, //6
B01110000, //7
B01111111, //8
B01110011, //9
B01111101, //0
B01010000, //1
B00111110, //2
B01110110, //3
B01010011, //4
B01100111, //5
B01101111, //6
B01110000, //7
B01111111, //8
B01110011, //9
B01111101, //0
B01010000, //1
B00111110, //2
B01110110, //3
B01010011, //4
B01100111, //5
B01101111, //6
B01110000, //7
B01111111, //8
B01110011, //9
B01111101, //0
B01010000, //1
B00111110, //2
B01110110, //3
B01010011, //4
B01100111, //5
B01101111, //6
B01110000, //7
B01111111, //8
B01110011, //9

};
byte fontArray2[] = {

B01111110, //0
B01111110, //0
B01111110, //0
B01111110, //0
B01111110, //0
B01111110, //0
B01111110, //0
B01111110, //0
B01111110, //0
B01111110, //0
B00110000, //1
B00110000, //1
B00110000, //1
B00110000, //1
B00110000, //1
B00110000, //1
B00110000, //1
B00110000, //1
B00110000, //1
B00110000, //1
B01011101, //2
B01011101, //2
B01011101, //2
B01011101, //2
B01011101, //2
B01011101, //2
B01011101, //2
B01011101, //2
B01011101, //2
B01011101, //2
B01110101, //3
B01110101, //3
B01110101, //3
B01110101, //3
B01110101, //3
B01110101, //3
B01110101, //3
B01110101, //3
B01110101, //3
B01110101, //3
B00110011, //4
B00110011, //4
B00110011, //4
B00110011, //4
B00110011, //4
B00110011, //4
B00110011, //4
B00110011, //4
B00110011, //4
B00110011, //4
B01100111, //5
B01100111, //5
B01100111, //5
B01100111, //5
B01100111, //5
B01100111, //5
B01100111, //5
B01100111, //5
B01100111, //5
B01100111, //5
B00000000, //blank
};
byte fontArray3[] = {

B00000000,
B01010000, //1 
B00111101, //2 
B01110101, //3 
B01010011, //4 
B01100111, //5 
B01101111, //6 
B01110000, //7 
B01110011, //8 
B01111110, // 9
B00000000, //0
B01010000, //1 2
B00111101, //2 3
B01010000, //1 
B00111101, //2 
B01110101, //3 
B01010011, //4 
B01100111, //5 
B01101111, //6 
B01110000, //7 
B01110011, //8 
B01111110, // 9
B00000000, //0
B01010000, //1 2
B00111101, //2 3

};
byte fontArray4[] = {

B00000000,
B00000000,
B00000000,
B00000000,
B00000000,
B00000000,
B00000000,
B00000000,
B00000000,
B00000000,
B10010000, //1
B10010000, //1
B10010000, //1
B00000000,
B00000000,
B00000000,
B00000000,
B00000000,
B00000000,
B00000000,
B00000000,
B00000000,
B10010000, //1
B10010000, //1
B10010000, //1
};

byte fontArray5[] = {

B01111110, //0
B00110000, //1
B01011101, //2
B01110101, //3
B00110011, //4
B01100111, //5
B01101111, //6
B01110000, //7
B01111111, //8
B01110011, //9
B00000000, //blank
};

byte fontArray6[] = {

B10010000,
B10101110,
B10110110,
B10010011,
B00110111,
B00011111,
B10110000,
B10111111,
B10110011,
};

void loop(){    
  if (wait == 0) {
        waitmessage();
  }
  
  if (Serial.available()) {
    processSyncMessage();
  }
  if (timeStatus()!= timeNotSet) {
    digitalClockDisplay();  
  }
  if (timeStatus() == timeSet) {
    digitalWrite(13, HIGH); // LED on if synced
  } else {
    digitalWrite(13, LOW);  // LED off if needs refresh
  }
  if (minute() == 59) {
   delay(60000);
   houranimation();
  }
  delay(1000);
}

void digitalClockDisplay(){
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year()); 
  Serial.println(); 

      digitalWrite(LATCH, LOW);
        shiftOut(DATA, CLOCK, LSBFIRST, fontArray1[minute()]);
        shiftOut(DATA, CLOCK, LSBFIRST, fontArray2[minute()]);
        shiftOut(DATA, CLOCK, LSBFIRST, fontArray3[hour()]);
        shiftOut(DATA, CLOCK, LSBFIRST, fontArray4[hour()]);
    digitalWrite(LATCH, HIGH);
}

void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

void waitmessage() {
       digitalWrite(LATCH, LOW);
        shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
        shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
        shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
        shiftOut(DATA, CLOCK, LSBFIRST, B01000000);
    digitalWrite(LATCH, HIGH);
    delay(500);
         digitalWrite(LATCH, LOW);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
        shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
        shiftOut(DATA, CLOCK, LSBFIRST, B10000000);
        shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
    digitalWrite(LATCH, HIGH);
    delay(500);
         digitalWrite(LATCH, LOW);
         shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
        shiftOut(DATA, CLOCK, LSBFIRST, B10000000);
        shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
        shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
    digitalWrite(LATCH, HIGH);
    delay(500);
         digitalWrite(LATCH, LOW);
         shiftOut(DATA, CLOCK, LSBFIRST, B10000000);
        shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
        shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
        shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
    digitalWrite(LATCH, HIGH);
}

void processSyncMessage() {
  unsigned long pctime;
  const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013

  if(Serial.available()) {
     pctime = Serial.parseInt();
     if( pctime >= DEFAULT_TIME) { // check the integer is a valid time (greater than Jan 1 2013)
       setTime(pctime); // Sync Arduino clock to the time received on the serial port
       wait = 1;
     }
  }
}

void houranimation(){

     for ( int x = 0; x < 35; x++ ) {// Loop to do "something" n times
       // Do "something"

    
  digitalWrite(LATCH, LOW);
          ran = random(0,9);
          ran2 = random(0,9);
          shiftOut(DATA, CLOCK, LSBFIRST, fontArray1[ran]);
          shiftOut(DATA, CLOCK, LSBFIRST, fontArray5[ran2]);
          shiftOut(DATA, CLOCK, LSBFIRST, fontArray3[ran]);
          shiftOut(DATA, CLOCK, LSBFIRST, fontArray6[ran2]);
     digitalWrite(LATCH, HIGH);
     delay(100);
   }

   
     for ( int x = 0; x < 25; x++ ) {// Loop to do "something" n times
       // Do "something"
    
  digitalWrite(LATCH, LOW);
          ran = random(0,9);
          ran2 = random(0,9);
          shiftOut(DATA, CLOCK, LSBFIRST, fontArray1[ran]);
          shiftOut(DATA, CLOCK, LSBFIRST, fontArray2[minute()]);
          shiftOut(DATA, CLOCK, LSBFIRST, fontArray3[ran]);
          shiftOut(DATA, CLOCK, LSBFIRST, fontArray6[ran2]);
     digitalWrite(LATCH, HIGH);
     delay(100);
    }

    for ( int x = 0; x < 9; x++ ) {// Loop to do "something" n times
       // Do "something"
    
  digitalWrite(LATCH, LOW);
          ran = random(0,9);
          ran2 = random(0,9);
          shiftOut(DATA, CLOCK, LSBFIRST, fontArray1[minute()]);
          shiftOut(DATA, CLOCK, LSBFIRST, fontArray2[minute()]);
          shiftOut(DATA, CLOCK, LSBFIRST, fontArray3[ran]);
          shiftOut(DATA, CLOCK, LSBFIRST, fontArray6[ran2]);
     digitalWrite(LATCH, HIGH);
     delay(100);
    }

    for ( int x = 0; x < 19; x++ ) {// Loop to do "something" n times
       // Do "something"
    
  digitalWrite(LATCH, LOW);
          ran = random(0,9);
          ran2 = random(0,9);
          shiftOut(DATA, CLOCK, LSBFIRST, fontArray1[minute()]);
          shiftOut(DATA, CLOCK, LSBFIRST, fontArray2[minute()]);
          shiftOut(DATA, CLOCK, LSBFIRST, fontArray3[ran]);
          shiftOut(DATA, CLOCK, LSBFIRST, fontArray4[hour()]);
     digitalWrite(LATCH, HIGH);
     delay(100);
    }

    for ( int x = 0; x < 8; x++ ) {// Loop to do "something" n times
       // Do "something"
    
  digitalWrite(LATCH, LOW);
          ran = random(0,9);
          ran2 = random(0,9);
          shiftOut(DATA, CLOCK, LSBFIRST, fontArray1[minute()]);
          shiftOut(DATA, CLOCK, LSBFIRST, fontArray2[minute()]);
          shiftOut(DATA, CLOCK, LSBFIRST, fontArray3[ran]);
          shiftOut(DATA, CLOCK, LSBFIRST, fontArray4[hour()]);
     digitalWrite(LATCH, HIGH);
     delay(250);
    }

      for ( int x = 0; x < 3; x++ ) {// Loop to do "something" n times
       // Do "something"
    
  digitalWrite(LATCH, LOW);
          ran = random(0,9);
          ran2 = random(0,9);
          shiftOut(DATA, CLOCK, LSBFIRST, fontArray1[minute()]);
          shiftOut(DATA, CLOCK, LSBFIRST, fontArray2[minute()]);
          shiftOut(DATA, CLOCK, LSBFIRST, fontArray3[ran]);
          shiftOut(DATA, CLOCK, LSBFIRST, fontArray4[hour()]);
     digitalWrite(LATCH, HIGH);
     delay(1000);
    }
        for ( int x = 0; x < 3; x++ ) {// Loop to do "something" n times
       // Do "something"
    
  digitalWrite(LATCH, LOW);
          shiftOut(DATA, CLOCK, LSBFIRST, fontArray1[minute()]);
          shiftOut(DATA, CLOCK, LSBFIRST, fontArray2[minute()]);
          shiftOut(DATA, CLOCK, LSBFIRST, fontArray3[hour()]);
          shiftOut(DATA, CLOCK, LSBFIRST, fontArray4[hour()]);
     digitalWrite(LATCH, HIGH);

     delay(1000);

       digitalWrite(LATCH, LOW);
          shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
          shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
          shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
          shiftOut(DATA, CLOCK, LSBFIRST, B00000000);
     digitalWrite(LATCH, HIGH);
     delay (1000);
     
    
}
}
time_t requestSync()
{
  Serial.write(TIME_REQUEST);  
  return 0; // the time will be sent later in response to serial mesg
}

