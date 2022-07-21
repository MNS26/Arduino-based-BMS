extern SerialMenu& menu;
extern const uint8_t MainMenuSize;
extern const uint8_t SettingsMenuSize;
extern const SerialMenuEntry MainMenu[];
extern const SerialMenuEntry SettingsMenu[];



const SerialMenuEntry MainMenu[] ={
{"#####################", false, ' ',{}},
{"###   MAIN MENU   ###", false, ' ',{}},
{"#####################", false, ' ',{}},
{"# 1: Settings       #", false, '1', [](){menu.load(SettingsMenu,SettingsMenuSize);menu.show();}},
{"# 2:                #", false, '0', [](){Serial.println("");menu.show();}},
{"# 3:                #", false, ' ', [](){Serial.println("");menu.show();}},
{"# 4:                #", false, ' ', [](){Serial.println("");menu.show();}},
{"# 5:                #", false, ' ', [](){Serial.println("");menu.show();}},
{"# 6:                #", false, ' ', [](){Serial.println("");menu.show();}},
{"# 7:                #", false, ' ', [](){Serial.println("");menu.show();}},
{"# 8:                #", false, ' ', [](){Serial.println("");menu.show();}},
{"# 9:                #", false, ' ', [](){Serial.println("");menu.show();}},
{"# 0: Show menu      #", false, '0', [](){Serial.println();menu.show();}},
{"#####################", false, ' ',{}}
};

const SerialMenuEntry SettingsMenu[] ={
{"#####################", false, ' ',{}},
{"### SETTINGS MENU ###", false, ' ',{}},
{"#####################", false, ' ',{}},
{"# 1: Current        #", false, '1', [](){Serial.println("");menu.show();}},
{"# 2: Default        #", false, '2', [](){Serial.println("");menu.show();}},
{"# 3: Reset          #", false, '3', [](){Serial.println("");menu.show();}},
{"# 4:                #", false, ' ', [](){Serial.println("");menu.show();}},
{"# 5:                #", false, ' ', [](){Serial.println("");menu.show();}},
{"# 6:                #", false, ' ', [](){Serial.println("");menu.show();}},
{"# 7:                #", false, ' ', [](){Serial.println("");menu.show();}},
{"# 8:                #", false, ' ', [](){Serial.println("");menu.show();}},
{"# 9:                #", false, ' ', [](){Serial.println("");menu.show();}},
{"# 0: Back           #", false, '0', [](){menu.load(MainMenu,MainMenuSize);menu.show();}},
{"#####################", false, ' ',{}}
};




constexpr uint8_t MainMenuSize = GET_MENU_SIZE(MainMenu);
constexpr uint8_t SettingsMenuSize = GET_MENU_SIZE(SettingsMenu);
