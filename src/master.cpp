#ifdef ARDUINO_ARCH_RP2040
/*************/
/* LIBRARIES */
/*************/
#include <mbed.h>
#include <Wire.h>
MbedI2C i2c(2,3);
#include <multicore.h>
#define delay(x) sleep_ms(x)

#include "sensors.hpp"
#include "settings.hpp"
#include "commands.hpp"


#define SERIALMENU_DISABLE_HEARTBEAT_ON_IDLE true
#define SERIALMENU_DISABLE_SERIAL_SETUP true
#define SERIALMENU_DISABLE_PROGMEM_SUPPORT true
#define SERIALMENU_MINIMAL_FOOTPRINT true
#define SERIALMENU_DISABLE_MENU_ERROR true
#include <SerialMenu.hpp>
#include "SerialMenu/functions.hpp"
#include "SerialMenu/UI.hpp"

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3C 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &i2c, OLED_RESET);



/*************/
/* VARIABLES */
/*************/

SerialMenu& menu = SerialMenu::get();
extern const uint8_t MainMenuSize;
extern const uint8_t SettingsMenuSize;
extern const SerialMenuEntry MainMenu[];
extern const SerialMenuEntry SettingsMenu[];


byte i2c_address[127];
byte i2c_address_total=0;
uint16_t reading;
uint16_t i2c_reading;
uint16_t state=0;
uint16_t pinState=0;
uint16_t debounceTime=0;
uint16_t debounce=50;
uint16_t lastPinState=0;
uint16_t VCal = 0;
int ID;
struct i2c_datain
{
	int VCC = 0;
	byte flags = 0;
} i2c_dataIn;
struct i2c_dataout
{
	byte command;
	byte val1;
	byte val2;
} i2c_dataOut;


void detectI2C(){
	memset(i2c_address, 0, sizeof(i2c_address));
	i2c_address_total=0;
	for (int i = 1; i < 127; i++){
		i2c.beginTransmission (i);
		if (i2c.endTransmission () == 0)
		{
			//delay(1);
			i2c_address[i]=1; //making sure we have the 1st index be the 8th one
			i2c_address_total++;
		}
	}
	i2c_address[60]=0;
}
void setup() {
	Serial.begin(115200);
	i2c.begin();
	detectI2C();

	pinMode(7,INPUT_PULLUP);
	pinMode(8,INPUT_PULLUP);
	
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
	// Clear the buffer
  display.clearDisplay();
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(30, 30);
  display.println("EEEEEEEEEEEEEEEEEE");
  display.display();

	menu.load(MainMenu,MainMenuSize);
	menu.show();
	}
float	var;

void loop()
{
int i;
	menu.run(50);
	for(int i=1;i<127;i++){
		if(i2c_address[i]==1){
			if(digitalRead(8)==1)
			{i = 8; display.invertDisplay(0);}
			i2c.requestFrom(i,3);
			i2c_dataIn.flags = i2c.read();
			i2c_dataIn.VCC = (i2c.read()<<8)|i2c.read();		
			display.clearDisplay();
			display.setCursor(1, 1);
			display.print("ID: ");
			display.println(i);
			display.println();
			display.println(" ");
			display.print((float)i2c_dataIn.VCC/1000);
			display.display();
		}
	}
}
#endif