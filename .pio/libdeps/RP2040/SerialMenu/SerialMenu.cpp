///////////////////////////////////////////////////////////////////////////////
// Serial port Menus
///////////////////////////////////////////////////////////////////////////////
#if ARDUINO_ARCH_RP2040
#ifndef _MBED_WRAP_H_
#include <mbed.h>
#endif
#else
#ifndef Arduino_h
#include <Arduino.h>
#endif
#endif

#include <SerialMenu.hpp>

// Instantiate the singleton menu instance. It is initialized when called
//SerialMenu SerialMenu::singleton;
SerialMenu* SerialMenu::singleton = nullptr;
const SerialMenuEntry* SerialMenu::menu = nullptr;
uint16_t SerialMenu::waiting = uint16_t(0);
uint8_t SerialMenu::size = uint8_t(0);