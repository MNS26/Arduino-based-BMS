#include <Arduino.h>
#include <avr/wdt.h>
#ifdef ATTINY_CORE
#include <EEPROM.h>
#include "TinyWireS.h"
#include "commands.h"
#include "sensors.h"
#include "settings.h"

bool updateEEP = false;

extern byte i2c_sensors[SensorCount];
byte ID;
int reg_size = sizeof(i2c_sensors);
int reg_pos;
int time;
int CellV;
int debugCellV;
int VCal = 0;
//extern int VCal;

bool debugMode = false;
int debugVCC;


void requestEvent(){
	TinyWireS.send(i2c_sensors[reg_pos]);
	reg_pos++;
  if (reg_pos >= reg_size)
  {
    reg_pos = 0;
  }
}
void receiveEvent(byte count){
	int val;
	byte opt;
	byte check;
	if(count>=1)
	{check = TinyWireS.receive();count--;}

	switch (check)
	{
	case RESET:// Command
		settingsReset();
		break;
	case DEBUG_TOGGLE:
		debugMode=!debugMode;
		break;
	case DEBUG_VCC:
		debugVCC = ((TinyWireS.receive()<<8)|TinyWireS.receive());
		count-=2;
		break;
	case SETTINGS_CHANGE:
		opt = TinyWireS.receive();
		count--;
		// Check if setting is within range
		if(1>opt||opt>5) {return;}
		val = (TinyWireS.receive()<<8)|(TinyWireS.receive());
		count-=2;
		switch (opt)
		{
		case 1:
			settings.RVmax = val; updateEEP = true; break;
		case 2:
			settings.RVfull = val; updateEEP = true; break;
		case 3:
			settings.RVlow = val; updateEEP = true; break;
		case 4:
			settings.RVempty = val; updateEEP = true; break;
		case 5:
			settings.RVmin = val; updateEEP = true; break;
		default:
			break;
		}
		case ID_CHANGE:
			ID = TinyWireS.receive(); updateEEP = true; break;
	default:
		break;
	}
	// Making sure there are no bytes left in case we drop out early
	while (count--)
	{
		TinyWireS.receive();
	}
}

void setup() {
	wdt_enable(WDTO_500MS);
	//only triggers when eeprom is blank
	if(EEPROM.read(0) == 255){
		EEPROM.put(0,defualtSettings);
	}
	EEPROM.get(0,settings);

	//jut store a defualt value in there
	if(EEPROM.read(sizeof(settings)+1) == 255){
		EEPROM.put(sizeof(settings)+1,ID);
	}
	EEPROM.get(sizeof(settings)+1,ID);
	TinyWireS.begin(ID);

	TinyWireS.onReceive(receiveEvent);
	TinyWireS.onRequest(requestEvent);
	pinMode(LED_BUILTIN,OUTPUT);
	digitalWrite(LED_BUILTIN,0);
	time = millis();
	i2c_sensors[2]=0;
}

void loop() {
	wdt_reset();
	TinyWireS.stateCheck();
	if(updateEEP){
		EEPROM.put(0,settings);
		EEPROM.put(sizeof(settings)+1,ID);
		updateEEP = false;
	}

	if(debugMode){
		bitSet(i2c_sensors[2],7);
	}
	else{
		bitClear(i2c_sensors[2],7);
	}
	//when debug mode isnt true(defualt) use vcc
	if(debugMode==false)
	{CellV = getVCC(CellV);}
	else
	{CellV = debugVCC;}
	
	CellV -= VCal;
	i2c_sensors[0] = ((byte)(CellV >> 8));
	i2c_sensors[1] = ((byte)(CellV & 0xff));
	
	if(CellV>=settings.RVfull)
	{}
	/*flag master to diable battery cus something is wrong*/
	if(CellV>=settings.RVmax)
	{bitSet(i2c_sensors[2],0);digitalWrite(LED_BUILTIN,1);}else{bitClear(i2c_sensors[2],0);digitalWrite(LED_BUILTIN,0);}

	if(CellV<=settings.RVlow)
	{}
	if(CellV<=settings.RVempty)
	{}
	/*flag master to diable battery cus something is wrong*/
	if(CellV<=settings.RVmin)
	{bitSet(i2c_sensors[2],1);;digitalWrite(LED_BUILTIN,1);}else{bitClear(i2c_sensors[2],1);digitalWrite(LED_BUILTIN,0);}
	

}
#endif