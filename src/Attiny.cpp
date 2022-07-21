#ifdef ATTINY_CORE
#include <Arduino.h>

#include <avr/wdt.h>
#include <EEPROM.h>
#include "TinyWireS.h"
#include "commands.hpp"
#include "sensors.hpp"
#include "settings.hpp"


#define SensorCount 3

/*************/
/* VARIALBES */
/*************/
byte ID;
byte i2c_data[SensorCount];
bool updateEEP=false;
bool debugMode=false;
int reg_size=sizeof(i2c_data);
int reg_pos;
int time;
int CellV;
struct Attiny
{
	int value;
	byte option;
	byte command;
} i2c_dataIn;



void requestEvent(){
	TinyWireS.send(i2c_data[reg_pos]);
	reg_pos++;
	if (reg_pos>=reg_size)
	{reg_pos=0;}
}
void receiveEvent(byte count){
	i2c_dataIn.command=TinyWireS.receive();
	count--;
	switch (i2c_dataIn.command)
	{
		case RESET:
			while (1)
			break;			
		case SETTINGS_RESET:// Command
			settingsReset();
			break;
		case DEBUG_TOGGLE:
			bitWrite(i2c_data[0],7,!bitRead(i2c_data[0],7));
			break;
		case DEBUG_VCC:
			CellV=((TinyWireS.receive()<<8)|TinyWireS.receive());count-=2;
			break;
		case SETTINGS_CHANGE:
			i2c_dataIn.option=TinyWireS.receive();count--;
			i2c_dataIn.value=(TinyWireS.receive()<<8)|(TinyWireS.receive());count-=2;
			switch (i2c_dataIn.option)
			{
				case 1:
					settings.Vmax=i2c_dataIn.value; updateEEP=true; break;
				case 2:
					settings.Vfull=i2c_dataIn.value; updateEEP=true; break;
				case 3:
					settings.Vlow=i2c_dataIn.value; updateEEP=true; break;
				case 4:
					settings.Vmin=i2c_dataIn.value; updateEEP=true; break;
				case 5:
					settings.VCal=i2c_dataIn.value; updateEEP=true; break;
				default:
					break;
			}
			case ID_CHANGE:
				ID=TinyWireS.receive(); updateEEP=true; break;
		default:
			break;
	}
	//Making sure there are no bytes left in case we drop out early
	while (count--)
	{
		TinyWireS.receive();
	}
}

void setup() {
	pinMode(SDA,INPUT_PULLUP);
	pinMode(SCL,INPUT_PULLUP);
	//only triggers when eeprom is blank
	if(EEPROM.read(0)==255)
	{EEPROM.put(0,defualtSettings);}
	EEPROM.get(0,settings);
	if(EEPROM.read(sizeof(settings)+1)==255)
	{EEPROM.put(sizeof(settings)+1,1);}
	EEPROM.get(sizeof(settings)+1,ID);
	TinyWireS.begin(ID);
	TinyWireS.onReceive(receiveEvent);
	TinyWireS.onRequest(requestEvent);
	pinMode(LED_BUILTIN,OUTPUT);
	digitalWrite(LED_BUILTIN,0);
	time=millis();
	wdt_enable(WDTO_500MS);
}

void loop() {
	wdt_reset();
	TinyWireS.stateCheck();
	//just to be sure to not wear out eeprom
	if(updateEEP){
		EEPROM.put(0,settings);
		EEPROM.put(sizeof(settings)+1,ID);
		updateEEP=false;
	}
	//when debug mode isnt true(defualt) use vcc
	if(!bitRead(i2c_data[0],7))
	{CellV=getVCC(CellV);}
	
	CellV-=settings.VCal;
	i2c_data[1]=((byte)(CellV>>8));
	i2c_data[2]=((byte)(CellV&0xff));
	
	if(CellV>=settings.Vmax)
	{bitSet(i2c_data[0],0);}
	else
	{bitClear(i2c_data[0],0);}

	if(settings.Vmax>CellV&&CellV>=settings.Vfull)
	{bitSet(i2c_data[0],1);}
	else
	{bitClear(i2c_data[0],1);}

	if(settings.Vmin<CellV&&CellV<=settings.Vlow)
	{bitSet(i2c_data[0],2);}
	else
	{bitClear(i2c_data[0],2);}
	
	if(CellV<=settings.Vmin)
	{bitSet(i2c_data[0],3);}
	else
	{bitClear(i2c_data[0],3);}
}

#endif