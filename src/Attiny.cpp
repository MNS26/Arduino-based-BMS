#include <Arduino.h>
#ifdef ATTINY_CORE
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
byte mode;
byte ID;
byte i2c_data[SensorCount];
bool updateEEP=false;
bool debugMode=false;
int reg_size=sizeof(i2c_data);
int reg_pos;
unsigned int CellV;
struct Attiny
{
	byte command;
	byte option;
	int value;
} i2c_dataIn;

//void(* resetFunc) (void) = 0; //declare reset function @ address 0

void requestEvent(){
	switch (mode)
	{
		TinyWireS.send(settings.Vmax);
		TinyWireS.send(settings.Vfull);		
		TinyWireS.send(settings.Vlow);
		TinyWireS.send(settings.Vmin);
		TinyWireS.send(settings.VCal);
		mode=0;
		break;
	case VMAX:
		TinyWireS.send(settings.Vmax);
		mode=0;
		break;
	case VFULL:
		TinyWireS.send(settings.Vfull);		
		mode=0;
		break;
	case VLOW:
		TinyWireS.send(settings.Vlow);
		mode=0;
		break;
	case VMIN:
		TinyWireS.send(settings.Vmin);
		mode=0;
		break;
	case VCAL:
		TinyWireS.send(settings.VCal);
		mode=0;
		break;
	default:
		TinyWireS.send(i2c_data[reg_pos]);
		reg_pos++;
		if (reg_pos>=reg_size)
		{reg_pos=0;}
		break;
	}
}
void receiveEvent(byte count){
	i2c_dataIn.command=TinyWireS.receive();
	count--;
	switch (i2c_dataIn.command)
	{
		case GET_SETTINGS:
			mode = TinyWireS.receive();
			count--;
			break;
		case CHANGE_SETTINGS:
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
		case CHANGE_ID:
				ID=TinyWireS.receive(); updateEEP=true; break;
		case RESTART:
			//digitalWrite(LED_BUILTIN,1);
			//delay(100);
			//digitalWrite(LED_BUILTIN,0);
			//resetFunc();
			exit(0);
			break;			
		case RESET_SETTINGS:// Command
			settingsReset();
			break;
		case DEBUG_TOGGLE:
			bitWrite(i2c_data[0],7,!bitRead(i2c_data[0],7));
			break;
		case DEBUG_VCC:
			CellV=((TinyWireS.receive()<<8)|TinyWireS.receive());count-=2;
			break;
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
	pinMode(LED_BUILTIN,OUTPUT);
	digitalWrite(LED_BUILTIN,1);
	pinMode(SDA,INPUT_PULLUP);
	pinMode(SCL,INPUT_PULLUP);
	//only triggers when eeprom is blank
	if(EEPROM.read(0)==255)
	{EEPROM.put(0,defualtSettings);}
	EEPROM.get(0,settings);
	if(EEPROM.read(sizeof(settings)+1)==255)
	{EEPROM.put(sizeof(settings)+1,1);}
	EEPROM.get(sizeof(settings)+1,ID);
	delay(10);
	digitalWrite(LED_BUILTIN,0);
	TinyWireS.begin(ID);
	TinyWireS.onReceive(receiveEvent);
	TinyWireS.onRequest(requestEvent);
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