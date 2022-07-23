extern SerialMenu& menu;
//extern uint8_t slaveID;
const char banner[] = "#######################";
extern const uint8_t MainMenuSize;
extern const uint8_t SettingsMenuSize;
extern const uint8_t DebugMenuSize;
extern const SerialMenuEntry MainMenu[];
extern const SerialMenuEntry SettingsMenu[];
extern const SerialMenuEntry DebugMenu[];

const SerialMenuEntry MainMenu[] ={
{banner										, false, ' ',{}},
{"###    MAIN MENU    ###", false, ' ',{}},
{banner										, false, ' ',{}},
{"# 1: Scan I2C         #", false, '1', [](){detectI2C();listI2C();}},
{"# 2: Select Slave ID  #", false, '2', [](){selID = menu.getNumber<uint8_t>("ID: "); if(selectID(selID)){slaveID = selID;}; menu.show();}},
{"# 3: Settings         #", false, '3', [](){menu.load(SettingsMenu,SettingsMenuSize);menu.show();}},
{"# 4: Debug            #", false, '4', [](){menu.load(DebugMenu,DebugMenuSize);menu.show();}},
{"# 5:                  #", false, ' ', {}},
{"# 6:                  #", false, ' ', {}},
{"# 7:                  #", false, ' ', {}},
{"# 8:                  #", false, ' ', {}},
{"# 9:                  #", false, ' ', {}},
{"# 0: Show menu        #", false, '0', [](){Serial.println();menu.show();}},
{banner										, false, ' ',{}}
};
const SerialMenuEntry SettingsMenu[] ={
{banner										, false, ' ',{}},
{"###  SETTINGS MENU  ###", false, ' ',{}},
{banner										, false, ' ',{}},
{"# 1: Current Settings #", false, '1', [](){CurrentSettings();menu.show();}},
{"# 2: Set ID           #", false, '2', [](){newID = menu.getNumber<uint8_t>("ID: ");if(changeID(slaveID,newID)){;}; menu.show();}},
{"# 3:                  #", false, ' ', [](){Serial.println();menu.show();}},
{"# 4:                  #", false, ' ', {}},
{"# 5:                  #", false, ' ', {}},
{"# 6:                  #", false, ' ', {}},
{"# 7:                  #", false, ' ', {}},
{"# 8: Restart          #", false, '8', [](){restart(slaveID);Serial.println();menu.show();}},
{"# 9: Reset            #", false, '9', [](){reset(slaveID);Serial.println();menu.show();}},
{"# 0: Back             #", false, '0', [](){Serial.println();menu.load(MainMenu,MainMenuSize);menu.show();}},
{banner										, false, ' ',{}}
};
const SerialMenuEntry DebugMenu[] ={
{banner										, false, ' ',{}},
{"###      DEBUG      ###", false, ' ',{}},
{banner										, false, ' ',{}},
{"# 1:                  #", false, ' ', [](){Serial.println();menu.show();}},
{"# 2:                  #", false, ' ', {}},
{"# 3:                  #", false, ' ', {}},
{"# 4:                  #", false, ' ', {}},
{"# 5:                  #", false, ' ', {}},
{"# 6:                  #", false, ' ', {}},
{"# 7:                  #", false, ' ', {}},
{"# 8:                  #", false, '8', [](){Serial.println();menu.show();}},
{"# 9:                  #", false, '9', [](){Serial.println();menu.show();}},
{"# 0: Back             #", false, '0', [](){Serial.println();menu.load(MainMenu,MainMenuSize);menu.show();}},
{banner										, false, ' ',{}}
};
constexpr uint8_t MainMenuSize = GET_MENU_SIZE(MainMenu);
constexpr uint8_t SettingsMenuSize = GET_MENU_SIZE(SettingsMenu);
constexpr uint8_t DebugMenuSize = GET_MENU_SIZE(DebugMenu);
