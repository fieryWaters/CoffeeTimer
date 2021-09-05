#include <Arduino.h>

///////////////////////////////////////////////BUZZER
void beepBeep(int pin, int numTimesRepeated)
{
	for (int i = 0; i < numTimesRepeated; i++)
	{
		digitalWrite(pin, HIGH);
		delay(60);
		digitalWrite(pin, LOW);
		delay(60);
	}
}
int buzzerPin = 12;

#include "thermistor.h"
#include "HardwareSerial.h"

// Analog pin used to read the NTC
#define NTC_PIN A3

// Thermistor object
THERMISTOR thermistor(NTC_PIN, // Analog pin
							 100000,	 // Nominal resistance at 25 ºC
							 3950,	 // thermistor's beta coefficient
							 100000); // Value of the series resistor

// Global temperature reading
double temp;
double previousTemp;

/*
void setup()
{
	Serial.begin(9600);
}


void loop()
{
}
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x3F, 16, 2);

#include <JC_Button.h> // https://github.com/JChristensen/JC_Button

// pin assignments
const byte upPin = 2,
			  downPin = 3;

Button up(upPin);		 // define the button
Button down(downPin); // define the button

void setup()
{
	pinMode(buzzerPin,OUTPUT);
	//////////////////////////////////////////////////
	// initialize the LCD
	lcd.begin();

	// Turn on the blacklight and print a message.
	lcd.backlight();
	/////////////////////////////////////////////////////
	Serial.begin(9600);
	up.begin();	  // initialize the button object
	down.begin(); // initialize the button object
}

int idealTemperature = 140; //our coffee is now chicken
int previousIdeal = idealTemperature;


void updateLCD()
{
	lcd.clear();
	lcd.print("Current F: ");
	lcd.print(((int)temp));

	lcd.setCursor(0,1);//x,y
	
	lcd.print("Ideal : "); 
	lcd.print(idealTemperature);
	
}

void loop()
{
	up.read();	 // read the button
	down.read(); // read the button

	if (up.wasReleased()) // if the button was released, change the LED state
	{
		idealTemperature++;
	}
	if (down.wasReleased()) // if the button was released, change the LED state
	{
		idealTemperature--;
	}

	temp = thermistor.read(); // Read temperature
	temp /= 10;

	temp = ((temp * 9) / 5.) + 32;

	if((int)temp<idealTemperature && (int)previousTemp>=idealTemperature)
	{
		beepBeep(buzzerPin, 3);
		Serial.print("BEEP");
	}
	if((int)temp!=(int)previousTemp || idealTemperature != previousIdeal)
	{
		updateLCD();
		previousTemp=temp;
		previousIdeal=idealTemperature;
	}




	//delay(5000);
}