#ifdef ARDUINO_ARCH_RP2040
/*************/
/* LIBRARIES */
/*************/

#define SERIALMENU_DISABLE_HEARTBEAT_ON_IDLE true
#define SERIALMENU_DISABLE_SERIAL_SETUP true
#define SERIALMENU_DISABLE_PROGMEM_SUPPORT true
#define SERIALMENU_MINIMAL_FOOTPRINT true
#define SERIALMENU_DISABLE_MENU_ERROR true
#define SCREEN1_ADDRESS 0x3C
#define SCREEN2_ADDRESS 0x3D
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET    -1 // Reset pin # (or -1 if sharing Arduino reset pin)

#define BT_A 5
#define BT_B 6
#define BT_C 7
#define BT_D 8

#include <mbed.h>
#include <multicore.h>
#define delay(x) sleep_ms(x)
#include <Wire.h>
#include "master/settings.hpp"
#include "bitmaps.hpp"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "commands.hpp"
struct i2c_datain
{
	int VCC = 0;
	byte flags = 0;
} i2c_dataIn;
#include "master/functions.hpp"
#include "master/chart/chart.hpp"
//#include "settings.hpp"
#include <SerialMenu.hpp>
#include "SerialMenu/functions.hpp"
#include "SerialMenu/UI.hpp"


MbedI2C i2c(2,3);

struct i2c_dataout
{
	byte command;
	byte val1;
	byte val2;
} i2c_dataOut;
bool idle_cursor_toggle, warning;
int counter;
unsigned int idle_cursor, debounce, low_batt, standby, standby_timeout=30, lcd_update;
unsigned long Time;




Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &i2c, OLED_RESET,1000000UL,1000000UL);
Adafruit_SSD1306 display2(SCREEN_WIDTH, SCREEN_HEIGHT, &i2c, OLED_RESET,1000000UL,1000000UL);

SerialMenu& menu = SerialMenu::get();
extern const uint8_t MainMenuSize, SettingsMenuSize;
extern const SerialMenuEntry MainMenu[], SettingsMenu[];
int i;
byte i2c_address[127];
byte i2c_count;
uint16_t i2c_VCC[127];
uint16_t VCC;
uint16_t i2c_reading;

void setup() {
	pinMode(BT_A,INPUT_PULLUP);
	pinMode(BT_B,INPUT_PULLUP);
	pinMode(BT_C,INPUT_PULLUP);
	pinMode(BT_D,INPUT_PULLUP);
	pinMode(2,INPUT_PULLUP);
	pinMode(3,INPUT_PULLUP);
	
	Serial.begin(115200);
	i2c.begin();
	i2c.setClock(10000);
	// SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
	//SSD1306_EXTERNALVCC = Use external display voltage source

  display.begin(SSD1306_SWITCHCAPVCC, SCREEN1_ADDRESS);
	display.setTextWrap(0);
	// Clear the buffer
  display.clearDisplay();
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.println("Initializing");
	display.display();
	delay(10);
  display.clearDisplay();
	display.setCursor(0,0);
	display.println("detecting I2C devices");
	display.display();
	delay(10);
	detectI2C();
  display.clearDisplay();
	display.setCursor(0,0);
  display.println("resetting I2C devices");
	display.display();
	delay(10);
	reseti2c();
	initgraph(display2,SCREEN2_ADDRESS);
	graphbegin();
	graphUpdate(VCCmin);
	//initgraph(display2);
	menu.load(MainMenu,MainMenuSize);
	menu.show();
}

void loop()
{
	buttons();
	if(VCC<VCCmin)
	VCC=VCCmin;
	if(VCC>VCCmax)
	VCC=VCCmax;
	if(0>Vlow)
	Vlow=0;
	if(Vlow>100)
	Vlow=100;
	if(millis()-low_batt>=1000)
	{low_batt=millis();warning=!warning;}
	if(millis()-idle_cursor>=300)
	{idle_cursor=millis();idle_cursor_toggle=!idle_cursor_toggle;}
	if(millis()-Time>=(1000*10))//5 minutes
	{
		Time=millis();
		graphUpdate(VCC);
	}
	menu.run(1);	
	if(millis()-500>lcd_update)
	{
		lcd_update = millis();
		getI2CData();
	}
	//graphClear();
	display.clearDisplay();
	if(millis()-standby<(1000*standby_timeout))
	{
		graphDisplay();
		display.drawBitmap(0,0,main_screen,128,64,1);
		display.setCursor(110, 3);
		//display.print((int)(counter/1.2));
		display.print((int)map(VCC,VCCmin,VCCmax,0,100));
		for(int i=0;i<Vlow*1.2;i++)
		{
			if(bitRead(i,0)==0)
			display.drawBitmap(4+i,21,lowv,1,6,WHITE);
			else
			display.drawBitmap(4+i,20,lowv,1,6,WHITE);
		}
		//display.fillRect(4,20,counter,6,WHITE);
		display.fillRect(4,20,map(VCC,VCCmin,VCCmax,0,120),6,WHITE);
		display.setCursor(90, 30);
		display.print((float)VCC/1000);
	}
	else
	{
		graphStandby(idle_cursor_toggle);
		if(idle_cursor_toggle)
		{
			display.drawLine(2,7,9,7,WHITE);
			display.drawLine(2,8,9,8,WHITE);
		}
	}
	if(((VCC/1000)*1.2)>=Vlow/2&&Vlow>((VCC/1000)*1.2))	
	{
		if(standby_timeout!=30) standby_timeout=30;
		if(warning)
		display.drawBitmap(92,0,warn,9,10,WHITE);
	}
	if(((VCC/100)*1.2)<Vlow/2)
	{
		if(standby_timeout!=15) standby_timeout=15;
		display.drawBitmap(92,0,warn,9,10,WHITE);
	}

	display.display();
}
#endif