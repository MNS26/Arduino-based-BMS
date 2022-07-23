#ifndef Commands_h
#define Commands_h
/************/
/* COMMANDS */
/************/
#define CHANGE_SETTINGS 0X01
#define CHANGE_ID 0x02
#define RESET_SETTINGS 0x11
#define RESET_ID 0x12
#define GET_SETTINGS 0x12
#define GET_ID 0X22
#define DEBUG_TOGGLE 0xE1
#define DEBUG_VCC 0xE2
#define RESTART 0xFF


/***********/
/* OPTIONS */
/***********/

/* SETTINGS */
#define ALL 0x01
#define VMAX 0x02
#define VFULL 0x03
#define VLOW 0x04
#define VMIN 0x05
#define VCAL 0x06




#endif //Command_h