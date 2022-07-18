#include <Arduino.h>
#include <avr/wdt.h>
#ifdef __AVR_ATmega2560__
#include "sensors.h"
#include "settings.h"
#include "commands.h"
#include <Wire.h>

int VCal = 0;
byte i2c_address[127]; //skipping first 8 addresses
byte i2c_address_total=0;
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

bool debugmode = false;
int state;
uint16_t reading;
uint16_t i2c_reading;
byte flags;

void setup() {

	pinMode(13,OUTPUT);
	digitalWrite(13,LOW);
	pinMode(5,INPUT_PULLUP);
	state = digitalRead(5);
	//Wire.setClock(50000); //50 000z
	Wire.begin();
	//detecting slave id's
	detectI2C();
	Serial.begin(115200);
	while (!Serial)
	delay(1);
	Serial.println("index:address");
	for(int i =0;i<127;i++){
		Serial.print(i+1);Serial.print(":");Serial.print(i2c_address[i]);Serial.print(" ");
	}
	Serial.println();
	Serial.print("total i2c: ");Serial.println(i2c_address_total);
	wdt_enable(WDTO_1S);
}
int debug=0;
int ID;
byte debugARR[3] = {DEBUG_VCC,0,0};
void loop() {
	wdt_reset();
	for(int i=1;i<sizeof(i2c_address);i++){

		if(i2c_address[i]!=0){
			ID = i;

			digitalWrite(13,debugmode);
			if (state != digitalRead(5)){
				state = digitalRead(5);
				debugmode = !debugmode;
				Wire.beginTransmission(ID);
				Wire.write(DEBUG_TOGGLE);
				Wire.endTransmission();
			}
			if(debugmode){
				debugARR[1]=(debug>>8);
				debugARR[2]=(debug&0xFF);
				Serial.print("WARNING!!! DEBUG MODE ENABLED   ");
				Wire.beginTransmission(ID);
				Wire.write(debugARR,sizeof(debugARR));
				Wire.endTransmission();
				i+=20;
				if(i>10000)
				{i=0;}

			}
			Serial.print("i2c ID: ");
			Serial.print(ID);
			Serial.print("  |  ");
			reading = getVCC(reading);
			reading -= VCal;
			Wire.requestFrom(10,3);
			i2c_reading = ((Wire.read()<<8)|(Wire.read()&0xFF));
			flags = Wire.read();
			Serial.print("M v");
			if(debugmode){
			Serial.print((float)debug/1000);
			}else{
			Serial.print((float)reading/1000);
			}
			//Serial.print(" | ");
			//Serial.print(Wire.read());
			Serial.print("  :  S v");
			Serial.print((float)i2c_reading/1000);
			Serial.print("  |  Flags: ");
			Serial.print(flags,BIN);
			Serial.println();
			while (Wire.available())
			{
				Wire.read();
			}
			delay(100);
		}
	}
}
#endif
