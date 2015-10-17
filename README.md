# MB Comfort Closure  
*the lazy man's window control/rainy day lifesaver!*  
  
Author: Joel.Gomez  
Created: 2014-04-30  
Microcontroller: Digispark  
Project: MB Comfort Closure

### Purpose  
Monitors OEM fob messages via RS232 from remote engine starter black box and  
activates window roll-up on second lock press only if car is already locked.  

### How it works
The remote start black box communicates to the car's ECUs over CAN bus, and 
offers both control over body functions such as locking/unlocking doors, 
controlling windows, etc. and can also communicate a status change of the car's 
inputs. The Digispark monitors the RS232 port of the remote start black box and 
listens for the message that the car has been locked using the remote key fob. 
If the Digispark detects that two consecutive lock requests have been sent from 
the key fob within a short window of time, the Digispark will send the window 
roll-up request to the black box, and the black box will actuate the windows 
and moonroof until they are all closed. 
  
#### Pin Map  
 Pin 0 - future use  
 Pin 1 - On Board LED (HIGH = ON)  
 Pin 2 - future use  
 Pin 3 - *1.5K pull-up resistor in parallel  
 Pin 4 - RS232 TX (Blue)  
 Pin 5 - RS232 RX (Green)  

****Note*: RS232 data and baud rate have been redacted from source code in 
an effort to protect any IP rights of the black box manufacturer, but with the 
right tools, it shouldn't be difficult to reverse engineer this info for your 
application.
  
#### Change Log  
2014-04-30  
-first release, includes comfort closure on 2nd lock press  
