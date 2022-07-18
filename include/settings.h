#ifndef Settings_h
#define Settings_h

#ifndef Arduino_h
#include <Arduino.h>
#endif // Arduino_h

extern bool updateEEP;

struct DefaultSettings
{
	int RVmax = 4300;
	int RVfull = 4200;
	int RVlow = 3200;
	int RVempty  = 3000;
	int RVmin = 2800;
}defualtSettings;
struct Settings
{
	int RVmax;
	int RVfull;
	int RVlow;
	int RVempty;
	int RVmin;
}settings;


void settingsReset(){
settings.RVmax = defualtSettings.RVmax;
settings.RVfull = defualtSettings.RVfull;
settings.RVlow = defualtSettings.RVlow;
settings.RVempty = defualtSettings.RVempty;
settings.RVmin = defualtSettings.RVmin;
updateEEP = true;
}
#endif // Settings_h

