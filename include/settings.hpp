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
 uint16_t Vlow = 3200;
 uint16_t Vmin = 2900;
 uint16_t VCal = 0;
}defualtSettings;
struct Settings
{
	uint16_t Vmax;
	uint16_t Vfull;
	uint16_t Vlow;
	uint16_t Vmin;
	uint16_t VCal;
}settings;


void settingsReset(){
settings.Vmax = defualtSettings.Vmax;
settings.Vfull = defualtSettings.Vfull;
settings.Vlow = defualtSettings.Vlow;
settings.Vmin = defualtSettings.Vmin;
settings.VCal = defualtSettings.VCal;
updateEEP = true;
}
#endif // Settings_h

