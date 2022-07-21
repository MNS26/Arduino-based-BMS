#ifndef Settings_h
#define Settings_h

#ifndef Arduino_h
#include <Arduino.h>
#endif // Arduino_h

extern bool updateEEP;

struct DefaultSettings
{
	int Vmax = 4300;
	int Vfull = 4150;
	int Vlow = 3200;
	int Vmin = 2900;
	int VCal = 0;
}defualtSettings;
struct Settings
{
	int Vmax;
	int Vfull;
	int Vlow;
	int Vmin;
	int VCal;
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

