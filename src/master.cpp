#include <Arduino.h>
#ifdef __AVR_ATmega2560__
/*************/
/* LIBRARIES */
/*************/
#include <Wire.h>
#include <avr/wdt.h>
#include "sensors.hpp"
#include "settings.hpp"
#include "commands.hpp"

#define SERIALMENU_DISABLE_HEARTBEAT_ON_IDLE true
#define SERIALMENU_DISABLE_SERIAL_SETUP true
//#define SERIALMENU_DISABLE_PROGMEM_SUPPORT true
#define SERIALMENU_MINIMAL_FOOTPRINT true
#define SERIALMENU_DISABLE_MENU_ERROR true
#include <SerialMenu.hpp>
#include "SerialMenu/functions.hpp"
#include "SerialMenu/UI.hpp"

/*************/
/* VARIABLES */
/*************/
byte i2c_address[127];
byte i2c_address_total=0;
byte debugARR[3] = {DEBUG_VCC,0,0};
uint16_t reading;
uint16_t i2c_reading;
uint16_t state=0;
uint16_t pinState=0;
uint16_t debounceTime=0;
uint16_t debounce=50;
uint16_t lastPinState=0;
uint16_t debug=0;
uint16_t VCal = 0;
bool debugON = false;
int ID;
struct i2c_datain
{
	int VCC = 0;
	byte flags = 0;
} i2c_dataIn;
struct i2c_dataout
{
	byte command;
	byte val1;
	byte val2;
} i2c_dataOut;
SerialMenu& menu = SerialMenu::get();
extern const uint8_t MainMenuSize;
extern const uint8_t SettingsMenuSize;
extern const SerialMenuEntry MainMenu[];
extern const SerialMenuEntry SettingsMenu[];

void detectI2C(){
	memset(i2c_address, 0, sizeof(i2c_address));
	i2c_address_total=0;
	for (byte i = 1; i < 127; i++){
		Wire.beginTransmission (i);
		if (Wire.endTransmission () == 0)
		{
			i2c_address[i]=1; //making sure we have the 1st index be the 8th one
			i2c_address_total++;
			//delay(1);
		}
	}
}

void debugtoggle(int pin){
			pinState=digitalRead(12);
			if(pinState==0&&lastPinState==1&&millis()-debounceTime>debounce){
				debounceTime = millis();
				Wire.beginTransmission(ID);
				Wire.write(DEBUG_TOGGLE);
				Wire.endTransmission();
			}
}

void debugVCC(int sID,bool debugState){
	if(debugState){
		debug+=20;
		if(debug>5000)
		{debug=0;}
		i2c_dataOut.command=DEBUG_VCC;
		i2c_dataOut.val1=(debug>>8);
		i2c_dataOut.val2=(debug&0xFF);

		debugARR[1]=(debug>>8);
		debugARR[2]=(debug&0xFF);
		Wire.beginTransmission(sID);
		Wire.write((byte *) &i2c_dataOut,sizeof(i2c_dataOut));
		//Wire.write(debugARR,sizeof(debugARR));
		Wire.endTransmission();

	}
}



void setup() {
	pinMode(SDA,INPUT_PULLUP);
	pinMode(SCL,INPUT_PULLUP);
	pinMode(10,OUTPUT);
	pinMode(13,OUTPUT);
	digitalWrite(10,HIGH);
	digitalWrite(13,LOW);
	pinMode(12,INPUT_PULLUP);
	
	Wire.setClock(100000); //100 000Hz
	Wire.begin();
	
	//detecting slave id's
	detectI2C();
	Serial.begin(115200);
	
	while (!Serial)
	delay(5);
	Serial.println();
	menu.load(MainMenu,MainMenuSize);
	menu.show();
	//wdt_enable(WDTO_1S);
}

void loop() {
	//wdt_reset();
	menu.run(50);
	for(unsigned int i=1;i<sizeof(i2c_address);i++){
		if(i2c_address[i]!=0){
			ID = i;
			debugtoggle(12);
			lastPinState = pinState;
			delay(3);

			// To update the flags
			Wire.requestFrom(ID,3);
			i2c_dataIn.flags = Wire.read();
			i2c_reading = ((Wire.read()<<8)|(Wire.read()&0xFF));


			debugVCC(ID,bitRead(i2c_dataIn.flags,7));
			delay(3);

			Wire.requestFrom(ID,3);
			i2c_dataIn.flags = Wire.read();
			i2c_reading = ((Wire.read()<<8)|(Wire.read()&0xFF));

			reading = getVCC(reading);
			reading -= VCal;
			/*
			if(bitRead(i2c_dataIn.flags,7))
			{
			Serial.print("WARNING!!! SLAVE DEBUG MODE  ");
			Serial.print(F("i2c ID: "));Serial.print(ID);Serial.print(F("  |  "));
			Serial.print(F("M v"));Serial.print((float)debug/1000);
			Serial.print(F("  :  S v"));Serial.print((float)i2c_reading/1000);
			}
			else
			{
			Serial.print(F("i2c ID: "));Serial.print(ID);Serial.print(F("  |  "));
			Serial.print(F("M v"));Serial.print((float)reading/1000);
			Serial.print(F("  :  S v"));Serial.print((float)i2c_reading/1000);
			}
			//print  i2c ID: XX  |  M vX.XX  :  S vX.XX  |  Flags: X
			Serial.print(F("  |  Flags: "));Serial.print(i2c_dataIn.flags,BIN);
			Serial.println();
			*/
			while (Wire.available())
			{
				Wire.read();
			}
		}
	}
		delay(150);

}
#endif
