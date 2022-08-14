#if ARDUINO_ARCH_RP2040
#ifndef _MBED_WRAP_H_
#include <mbed.h>
#endif
#endif
extern byte i2c_address[127];
extern uint16_t i2c_VCC[127];
extern unsigned int standby;
extern byte i2c_count;
extern uint16_t VCC;
extern MbedI2C i2c;
extern unsigned int debounce;

void detectI2C(){
	memset(i2c_address, 0, sizeof(i2c_address));
	for (int i = 8; i < 127; i++){
		i2c.beginTransmission (i);
		delay(5);
		if (i2c.endTransmission () == 0)
		{
			if(i!=60)
			i2c_count++;
			i2c_address[i]=1; //making sure we have the 1st index be the 8th one

		}
	}
	i2c_address[SCREEN1_ADDRESS]=0;
	i2c_address[SCREEN2_ADDRESS]=0;
}

void reseti2c(int id)
{
	i2c.beginTransmission(id);
	delay(5);
	i2c.write(RESTART);
	i2c.endTransmission();
}
void reseti2c()
{
	for(int i = 8; i<127;i++)
	{
		delay(5);
		if(i!=60)
		{
			i2c.beginTransmission(i);	
			i2c.write(RESTART);
			i2c.endTransmission();
		}
	}
}

void setVlow(int direction){
		if(direction==0)
		{Vlow--;}
		if(direction==1)
		{Vlow++;}

}

void buttons()
{
	if(digitalRead(BT_A)!=1)
	{
		if(millis()>standby+(1000*60)){standby=millis();return;}//wake from any button
		if(millis()>debounce+50)
			{
				standby=debounce=millis();
				VCC-=10;
			}
		}
	if(digitalRead(BT_B)!=1)
	{
		if(millis()>standby+(1000*60)){standby=millis();return;}//wake from any button
		if(millis()>debounce+50)
		{
			standby=debounce=millis();
			VCC+=10;
		}
	}
	if(digitalRead(BT_C)!=1)
	{
		if(millis()>standby+(1000*60)){standby=millis();return;}//wake from any button
		if(millis()>debounce+50)
		{
			standby=debounce=millis();
			setVlow(0);
		}
	}
	if(digitalRead(BT_D)!=1)
	{
		if(millis()>standby+(1000*60)){standby=millis();return;}//wake from any button
		if(millis()>debounce+50)
		{
			standby=debounce=millis();
			setVlow(1);
		}
	}
}
void getI2CData(){
for(int i=8;i<127;i++)
		{
			if(i2c_address[i]==1)
			{
				i2c.requestFrom(i,3);
				i2c_dataIn.flags = i2c.read();
				i2c_VCC[i] = (i2c.read()<<8)|i2c.read();
				i2c_dataIn.VCC = (i2c.read()<<8)|i2c.read();		
			}
		}
		for(int i=8;i<127;i++)
		{
			if (VCC>4500)
			{
				VCC/=2;
			}
			if(i2c_address[i]==1)
			{
				VCC+=i2c_VCC[i];
			}
		}
}