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
{"#######################", false, NULL, nullptr},
{"###    MAIN MENU    ###", false, NULL, nullptr},
{"#######################", false, NULL, nullptr},
{"# 1: Scan I2C         #", false, '1', [](){detectI2C();listI2C();}},
{"# 2: Select Slave ID  #", false, '2', [](){selID = menu.getNumber<uint8_t>("ID: "); if(selectID(selID)){slaveID = selID;}; menu.show();}},
{"# 3: Settings         #", false, '3', [](){menu.load(SettingsMenu,SettingsMenuSize);Serial.println();menu.show();}},
{"# 4: Debug            #", false, '4', [](){menu.load(DebugMenu,DebugMenuSize);Serial.println();menu.show();}},
{"# 5:                  #", false, NULL, nullptr},
{"# 6:                  #", false, NULL, nullptr},
{"# 7:                  #", false, NULL, nullptr},
{"# 8:                  #", false, NULL, nullptr},
{"# 9:                  #", false, NULL, nullptr},
{"# 0: Show menu        #", false, '0', [](){Serial.println();menu.show();}},
{"#######################", false, NULL, nullptr}
};
const SerialMenuEntry SettingsMenu[] ={
{"#######################", false, NULL, nullptr},
{"###  SETTINGS MENU  ###", false, NULL, nullptr},
{"#######################", false, NULL, nullptr},
{"# 1: Current Settings #", false, '1', [](){CurrentSettings();}},
{"# 2: Set ID           #", false, '2', [](){newID = menu.getNumber<uint8_t>("ID: ");changeID(newID); menu.show();}},
{"# 3:                  #", false, ' ', [](){Serial.println();menu.show();}},
{"# 4:                  #", false, NULL, nullptr},
{"# 5:                  #", false, NULL, nullptr},
{"# 6:                  #", false, NULL, nullptr},
{"# 7:                  #", false, NULL, nullptr},
{"# 8: Restart          #", false, '8', [](){restart(slaveID);Serial.println();menu.show();}},
{"# 9: Reset            #", false, '9', [](){reset(slaveID);Serial.println();menu.show();}},
{"# 0: Back             #", false, '0', [](){Serial.println();menu.load(MainMenu,MainMenuSize);menu.show();}},
{"#######################", false, ' ',{}}
};
const SerialMenuEntry DebugMenu[] ={
{"#######################", false, NULL, nullptr},
{"###      DEBUG      ###", false, NULL, nullptr},
{"#######################", false, NULL, nullptr},
{"# 1:                  #", false, ' ', [](){Serial.println();menu.show();}},
{"# 2:                  #", false, NULL, nullptr},
{"# 3:                  #", false, NULL, nullptr},
{"# 4:                  #", false, NULL, nullptr},
{"# 5:                  #", false, NULL, nullptr},
{"# 6:                  #", false, NULL, nullptr},
{"# 7:                  #", false, NULL, nullptr},
{"# 8:                  #", false, '8', [](){Serial.println();menu.show();}},
{"# 9:                  #", false, '9', [](){Serial.println();menu.show();}},
{"# 0: Back             #", false, '0', [](){Serial.println();menu.load(MainMenu,MainMenuSize);menu.show();}},
{"#######################", false, NULL, nullptr}
};
constexpr uint8_t MainMenuSize = GET_MENU_SIZE(MainMenu);
constexpr uint8_t SettingsMenuSize = GET_MENU_SIZE(SettingsMenu);
constexpr uint8_t DebugMenuSize = GET_MENU_SIZE(DebugMenu);