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
int reg_pos=0;
int sett_size=sizeof(settings);///sizeof(uint16_t);
int sett_pos=0;
unsigned int CellV;
struct Attiny
{
	byte command;
	byte option;
} i2c_dataIn;

void requestEvent(){
	switch (mode)
	{
	case ON://enable get settings
		TinyWireS.send(settings[sett_pos]);
		sett_pos++;
		if(sett_pos>=sett_size)
		{sett_pos=0;}
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
			//i2c_dataIn.value=(TinyWireS.receive()<<8)|(TinyWireS.receive());count-=2;
			switch (i2c_dataIn.option)
			{
				case 1:
					settings[0]=TinyWireS.receive();settings[1]=TinyWireS.receive(); count-=2; updateEEP=true; break;
				case 2:
					settings[2]=TinyWireS.receive();settings[3]=TinyWireS.receive(); count-=2; updateEEP=true; break;
				case 3:
					settings[4]=TinyWireS.receive();settings[5]=TinyWireS.receive(); count-=2; updateEEP=true; break;
				case 4:
					settings[6]=TinyWireS.receive();settings[7]=TinyWireS.receive(); count-=2; updateEEP=true; break;
				case 5:
					settings[8]=TinyWireS.receive();settings[9]=TinyWireS.receive(); count-=2; updateEEP=true; break;
				default:
					break;
			}
		case CHANGE_ID:
				ID=TinyWireS.receive(); count--; updateEEP=true; break;
		case RESTART:
			while (1)
			{
				/* code */
			};
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
	wdt_enable(WDTO_1S);
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
	
	CellV-=(settings[8]<<8|settings[9]);
	i2c_data[1]=((byte)(CellV>>8));
	i2c_data[2]=((byte)(CellV&0xff));
	
	//CellV>=vmax
	if(CellV>=(settings[0]<<8|settings[1]))
	{bitSet(i2c_data[0],0);}
	else
	{bitClear(i2c_data[0],0);}
	//vmax>Cellv&&Cellv>=vfull
	if((settings[0]<<8|settings[1])>CellV&&CellV>=(settings[2]<<8|settings[3]))
	{bitSet(i2c_data[0],1);}
	else
	{bitClear(i2c_data[0],1);}
	//vlow<Cellv&&Cellv<=vfull
	if((settings[4]<<8|settings[5])<CellV&&CellV<=(settings[2]<<8|settings[3]))
	{bitSet(i2c_data[0],2);}
	else
	{bitClear(i2c_data[0],2);}
	//Cellv<=vmin
	if(CellV<=(settings[6]<<8|settings[7]))
	{bitSet(i2c_data[0],3);}
	else
	{bitClear(i2c_data[0],3);}
}
#endif