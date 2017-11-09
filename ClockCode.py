import time
import serial
import struct

timestamp = int(time.time())
timestamp2 = timestamp  + 14409
timestamp3 = str(timestamp2)
timestamp4 = bytes(timestamp3, encoding='utf-8')

import sys
import serial.tools.list_ports
serPort = ""
int1 = 0
str1 = ""
str2 = ""

    
# Find Live Ports
ports = list(serial.tools.list_ports.comports())
for p in ports:
   print (p) # This causes each port's information to be printed out.
           # To search this p data, use p[1].

   while int1 < 9:   # Loop checks "COM0" to "COM8" for Adruino Port Info. 
            

      if "Arduino Uno" in p[1]:  # Looks for "CH340" in P[1].
            str2 = str(int1) # Converts an Integer to a String, allowing:
            str1 = "COM" + str2 # add the strings together.

      if "Arduino Uno" in p[1] and str1 in p[1]: # Looks for "CH340" and "COM#"
         print ("Found Arduino on ",str1)
         int1 = 9 # Causes loop to end.

      if int1 == 12:
         print ("Arduino not found!")


      int1 = int1 + 1
   
# Set Port
ser = serial.Serial(str1, 9600, writeTimeout=0)

time.sleep(7)
print(timestamp)
print (timestamp)
print(timestamp3)
ser.write (timestamp4)


# To see what it looks like on python side
val = timestamp

# send and receive via pyserial


