
#ifndef ARDUINO_ARCH_RP2040
#ifndef Sensors_h
#define Sensors_h

#ifndef Arduino_h 
#include <Arduino.h>
#endif // arduino_h

#ifdef ATTINY_CORE
#ifdef TinyWireS_h
#define delay(x) tws_delay(x) // replaces the delay with tws_delay
#define delayMicroseconds(x) tws_delay_micros(x) // replaces delayMicroseconds with tws_delay_micros
#endif
#endif

uint16_t getVCC() {
  //reads internal 1V1 reference against VCC
  #if defined(__AVR_ATtiny84__) || defined(__AVR_ATtiny44__)
	ADMUX = _BV(MUX5) | _BV(MUX0); // For ATtiny84
  #elif defined(__AVR_ATtiny85__) || defined(__AVR_ATtiny45__)
	ADMUX = _BV(MUX3) | _BV(MUX2); // For ATtiny85/45
  #elif defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega2560__)
	ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);  // For ATmega1284 or ATmega2560 variants
  #else
	ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);  // For ATmega328
  #endif
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA, ADSC));
  byte low = ADCL;
  unsigned int val = (ADCH << 8) | low;
  //discard previous result
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA, ADSC));
  low = ADCL;
  val = (ADCH << 8) | low;  
  return ((long)1024*1100)/val;  
}

uint16_t getVCC(int oldVCC){
	if(oldVCC!=0)
	{return (oldVCC +=getVCC())/2;}
	else
	{return getVCC();}
}
uint16_t getVCC(int oldVCC,int count){
	for(int i=0;i<count;i++)
	{
		if(oldVCC!=0)
		{oldVCC +=getVCC();oldVCC/=2;}
		else
		{oldVCC = getVCC();}
	}
	return oldVCC;
}
#endif
#endif