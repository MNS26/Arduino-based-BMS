#ifndef Settings_h
#define Settings_h

#ifndef Arduino_h
#include <Arduino.h>
#endif // Arduino_h


extern bool updateEEP;

struct DefaultSettings
{
 uint16_t Vmax = 4300;
 uint16_t Vfull = 4200;
 uint16_t Vlow = 3100;
 uint16_t Vmin = 2900;
 uint16_t VCal = 0;
}defualtSettings;
byte settings[10];
/*
struct Settings
{
	uint16_t Vmax;
	uint16_t Vfull;
	uint16_t Vlow;
	uint16_t Vmin;
	uint16_t VCal;
}settings;
*/

void settingsReset(){
settings[0] = defualtSettings.Vmax>>8;
settings[1] = defualtSettings.Vmax&0xFF;

settings[2] = defualtSettings.Vfull>>8;
settings[3] = defualtSettings.Vfull&0xFF;

settings[4] = defualtSettings.Vlow>>8;
settings[5] = defualtSettings.Vlow&0xFF;

settings[6] = defualtSettings.Vmin>>8;
settings[7] = defualtSettings.Vmin&0xFF;

settings[8] = defualtSettings.VCal>>8;
settings[9] = defualtSettings.VCal&0xFF;

updateEEP = true;
}
#endif // Settings_h

