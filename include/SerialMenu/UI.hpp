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
{"#######################", false, '`', {}},
{"###    MAIN MENU    ###", false, '`', {}},
{"#######################", false, '`', {}},
{"# 1: Scan I2C         #", false, '1', [](){detectI2C();listI2C();}},
{"# 2: Select Slave ID  #", false, '2', [](){selID = menu.getNumber<uint8_t>("ID: "); if(selectID(selID)){slaveID = selID;}; menu.show();}},
{"# 3: Settings         #", false, '3', [](){menu.load(SettingsMenu,SettingsMenuSize);Serial.println();menu.show();}},
{"# 4: Restart slaves   #", false, '4', [](){restartAll();}},
{"# 5: Debug            #", false, '5', [](){menu.load(DebugMenu,DebugMenuSize);Serial.println();menu.show();}},
{"# 6:                  #", false, '`', {}},
{"# 7:                  #", false, '`', {}},
{"# 8:                  #", false, '`', {}},
{"# 9:                  #", false, '`', {}},
{"# 0: Show menu        #", false, '0', [](){Serial.println();menu.show();}},
{"#######################", false, '`', {}}
};
const SerialMenuEntry SettingsMenu[] ={
{"#######################", false, '`', {}},
{"###  SETTINGS MENU  ###", false, '`', {}},
{"#######################", false, '`', {}},
{"# 1: Current Settings #", false, '1', [](){CurrentSettings();}},
{"# 2: Set ID           #", false, '2', [](){newID = menu.getNumber<uint8_t>("ID: ");changeID(newID); menu.show();}},
{"# 3:                  #", false, '`', [](){Serial.println();menu.show();}},
{"# 4:                  #", false, '`', {}},
{"# 5:                  #", false, '`', {}},
{"# 6:                  #", false, '`', {}},
{"# 7:                  #", false, '`', {}},
{"# 8: Restart          #", false, '8', [](){restart(slaveID);Serial.println();menu.show();}},
{"# 9: Reset            #", false, '9', [](){reset(slaveID);Serial.println();menu.show();}},
{"# 0: Back             #", false, '0', [](){Serial.println();menu.load(MainMenu,MainMenuSize);menu.show();}},
{"#######################", false, '`', {}}
};
const SerialMenuEntry DebugMenu[] ={
{"#######################", false, '`', {}},
{"###      DEBUG      ###", false, '`', {}},
{"#######################", false, '`', {}},
{"# 1:                  #", false, ' ', [](){Serial.println();menu.show();}},
{"# 2:                  #", false, '`', {}},
{"# 3:                  #", false, '`', {}},
{"# 4:                  #", false, '`', {}},
{"# 5:                  #", false, '`', {}},
{"# 6:                  #", false, '`', {}},
{"# 7:                  #", false, '`', {}},
{"# 8:                  #", false, '8', [](){Serial.println();menu.show();}},
{"# 9:                  #", false, '9', [](){Serial.println();menu.show();}},
{"# 0: Back             #", false, '0', [](){Serial.println();menu.load(MainMenu,MainMenuSize);menu.show();}},
{"#######################", false, '`', {}}
};
constexpr uint8_t MainMenuSize = GET_MENU_SIZE(MainMenu);
constexpr uint8_t SettingsMenuSize = GET_MENU_SIZE(SettingsMenu);
constexpr uint8_t DebugMenuSize = GET_MENU_SIZE(DebugMenu);