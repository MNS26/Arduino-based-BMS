#ifdef __AVR_ATmega2560__
#include <Arduino.h>
#include <avr/wdt.h>
#include "sensors.h"
#include "settings.h"
#include "commands.h"
#include <Wire.h>


uint16_t reading;
uint16_t i2c_reading;
byte i2c_address[127]; //skipping first 8 addresses
byte i2c_address_total=0;
byte flags;
byte debugARR[3] = {DEBUG_VCC,0,0};
int state=0;
int pinState=0;
int debounceTime=0;
int debounce=100;
int lastPinState=0;
int debug=0;
bool debugON = false;
int ID;
int VCal = 0;

void detectI2C(){
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

void debugVCC(int Sid,bool debugState){
	if(debugState){
		debugARR[1]=(debug>>8);
		debugARR[2]=(debug&0xFF);
		//Serial.print("WARNING!!! DEBUG MODE ENABLED  ");
		Wire.beginTransmission(Sid);
		Wire.write(debugARR,sizeof(debugARR));
		Wire.endTransmission();
		debug+=20;
		if(debug>5000)
		{debug=0;}
	}
}

void setup() {
	pinMode(13,OUTPUT);
	digitalWrite(13,LOW);
	pinMode(12,INPUT_PULLUP);
	
	Wire.setClock(50000); //50 000Hz
	Wire.begin();
	
	//detecting slave id's
	detectI2C();
	Serial.begin(38400);
	
	while (!Serial)
	delay(1);

	Serial.println("index:address");
	for(int i =1;i<127;i++){
		Serial.print(i);Serial.print(":");Serial.print(i2c_address[i]);Serial.print(" ");
	}
	
	Serial.println();
	Serial.print("total i2c: ");Serial.println(i2c_address_total);
	
	wdt_enable(WDTO_1S);
}

void loop() {
	wdt_reset();

	for(int i=1;i<sizeof(i2c_address);i++){
		if(i2c_address[i]!=0){
			ID = i;
			debugtoggle(12);
			lastPinState = pinState;
			reading = getVCC(reading);
			reading -= VCal;
			if(debugON)
			Serial.print("WARNING!!! DEBUG MODE ENABLED  ");

			Serial.print(F("i2c ID: "));Serial.print(ID);Serial.print(F("  |  "));
			
			Wire.requestFrom(ID,3);
			i2c_reading = ((Wire.read()<<8)|(Wire.read()&0xFF));
			flags = Wire.read();
			
			if(bitRead(flags,7))
			{debugON=true;}
			else
			{debugON=false;}
			
			debugVCC(ID,debugON);

			if(debugON)
			{Serial.print(F("M v"));Serial.print((float)debug/1000);}
			else
			{Serial.print(F("M v"));Serial.print((float)reading/1000);}

			//print  i2c ID: XX  |  M vX.XX  :  S vX.XX  |  Flags: X
			Serial.print(F("  :  S v"));Serial.print((float)i2c_reading/1000);Serial.print(F("  |  Flags: "));Serial.print(flags,BIN);Serial.println();

			while (Wire.available())
			{
				Wire.read();
			}
		}
	}
}
#endif
