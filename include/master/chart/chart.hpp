#include "CircularBuffer.h"
#include "bitmap.hpp"
extern bool idle_cursor_toggle;
CircularBuffer<byte,12> buffer;
Adafruit_SSD1306 *Display;

int lastPointY=0;
int lastPointX=200;
const int minPosX=14;
const int minPosY=53;
const int maxPosX=124;
const int maxPosY=3;
const int gap = 10;
bool gridShow=true;
void initgraph(Adafruit_SSD1306 &_Display,int address)
{
	Display = &_Display;
	Display->begin(SSD1306_SWITCHCAPVCC,address);
}
void graphbegin(){
	Display->setTextWrap(0);
	Display->clearDisplay();
  Display->setTextSize(1);             // Normal 1:1 pixel scale
  Display->setTextColor(SSD1306_WHITE);        // Draw white text
	Display->drawBitmap(0,0,chart,128,64,SSD1306_WHITE);
	if(gridShow)
	{Display->drawBitmap(0,0,grid,128,64,SSD1306_WHITE);}
	Display->display();
}

void graphUpdate(int _point)
{

	
	int point = map(_point,VCCmin,VCCmax,minPosY,maxPosY);
	buffer.unshift(point);

}

void graphDisplay()
{
	if(lastPointX<minPosX)
	lastPointX=minPosX;
	if(lastPointY<minPosY)
	lastPointY=minPosY;
	if(lastPointX>maxPosX)
	lastPointX=maxPosX;
	if(lastPointY>maxPosY)
	lastPointY=maxPosY;
	Display->clearDisplay();	
	Display->drawBitmap(0,0,chart,128,64,SSD1306_WHITE);
	if(gridShow)
	{Display->drawBitmap(0,0,grid,128,64,SSD1306_WHITE);}
	using index_t = decltype(buffer)::index_t;
	for (index_t i = 1; i < buffer.size(); i++) {
			if(buffer[i]!=0)
			{
				Display->drawLine(lastPointX,buffer[i-1]+1,lastPointX-gap,buffer[i]+1,1);
				Display->drawLine(lastPointX,buffer[i-1],  lastPointX-gap,buffer[i],  1);
			}
			lastPointX -= gap;
		}
	Display->display();
	lastPointX =maxPosX;
}
void graphStandby()
{
	Display->clearDisplay();
	Display->drawLine(2,7,9,7,WHITE);
	Display->drawLine(2,8,9,8,WHITE);
	Display->display();
}void graphStandby(bool toggle)
{
	Display->clearDisplay();
	if(toggle)
	{
	Display->drawLine(2,7,9,7,WHITE);
	Display->drawLine(2,8,9,8,WHITE);
	}
	Display->display();
}