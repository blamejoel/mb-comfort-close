#include <SoftwareSerial.h>
/*
Author: Joel.Gomez
Created: 2014-04-30
Microcontroller: Digispark
Project: MB Comfort Closure

Monitors OEM fob messages via RS232 from black box designed for remote engine
start applications and activates aux channel for window roll-up upon second
lock press, but only if car is already locked.

-------Pin Map--------
 Pin 0 - future plans
 Pin 1 - On Board LED (HIGH = ON)
 Pin 2 - future plans
 Pin 3 - *1.5K pull-up resistor in parallel
 Pin 4 - RS232 TX (Blue)
 Pin 5 - RS232 RX (Green)
 
/*--------Inputs/Outputs--------*/
const int LED = 1;	//pin 1
const int TX = 4;	//pin 4
const int RX = 5;	//pin 5

/*--------Program Variables--------*/
SoftwareSerial rs232(RX, TX); //RX, TX
const int BAUD = 0;          //actual baud redacted due to possible IP
const int YES = 1;
const int NO = 0;
const int ON = HIGH;
const int OFF = LOW;

/*--------Delays--------*/
const long LIGHT_FLASH = 500;   //light flash and pause between flash duration
const int RESPONSE_BUFFER = 10; //delay before responding to RS232 message
const long BUF_INTERVAL = 600;	//minimum time (millis) between messages to be
                                //considered valid
/*--------RS232 Commands--------*/
const int LOCK = 0x00;          //actual messages redacted due to possible IP
const int UNLOCK = 0x00;        //actual messages redacted due to possible IP
const int WINDOWSUP = 0x00;     //actual messages redacted due to possible IP


/*--------FLAGS--------*/
bool door_open =	false;
bool trunk_open =	false;
bool ign_on =		false;
bool locked =		false;
bool activated =	false;
long prevTime = 0;	//last time clock was looked at
int flashCount = 0;	//flashes done
int flashes = 0;	//flashes to do
int ledState = OFF;	//LED state to keep track of flashes

void setup()
{
	rs232.begin(BAUD);		//start serial comm
	pinMode(LED, OUTPUT);	//set LED pin as output
	pinMode(RX, INPUT);		//set RX pin as input
	pinMode(TX, OUTPUT);	//set TX pin as output
	digitalWrite(LED, OFF);	//LED off
}

void loop()
{
	unsigned long curTime = millis();	//update current time
	if (rs232.available() > 0)
	{
		int serial = rs232.read();
		
		//car locked by OE fob
		if (serial == LOCK) 
        { 
            if (curTime - prevTime > BUF_INTERVAL) 
            { 
                updateTime(curTime);
				rs232.write(serial);	//respond
				if (!locked)	        //doors unlocked?
				{
					locked = true;	    //set doors to locked
					activated = false;	//reset aux activated flag
					flash(1);
				}
				else if (!activated)	//aux hasn't been activated
				{
					activated = true;	//aux activated
					rs232.write(WINDOWSUP);	//activate aux
					flash(3);
				}
			}
		}
		
		//car unlocked by OE fob
		if (serial == UNLOCK)
		{
			if (curTime - prevTime > BUF_INTERVAL)
			{
				updateTime(curTime);
				rs232.write(serial);	//respond
				if (locked)		        //doors locked?
				{
					locked = false;	//set doors to unlocked
					flash(2);
				}

			}
		}
	}
	
	if (flashCount < flashes)
	{
		unsigned long ledTime = millis();

		if (ledTime - prevTime > LIGHT_FLASH)
		{
			updateTime(ledTime);
			if (ledState == OFF)
			{
				ledState = ON;
			}
			else
			{
				ledState = OFF;
				flashCount++;
			}
			
			digitalWrite(LED, ledState);
		}
	}
	
	//end loop
}

void updateTime(unsigned long time)
{
	prevTime = time;
}

void flash(int count)
{
	flashCount = 0;
	flashes = count;
}
