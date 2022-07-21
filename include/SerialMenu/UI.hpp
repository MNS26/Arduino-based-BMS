extern SerialMenu& menu;
extern const uint8_t MainMenuSize;
extern const uint8_t SettingsMenuSize;
extern const SerialMenuEntry MainMenu[];
extern const SerialMenuEntry SettingsMenu[];

const SerialMenuEntry MainMenu[] ={
{"#######################", false, ' ',{}},
{"###    MAIN MENU    ###", false, ' ',{}},
{"#######################", false, ' ',{}},
{"# 1: Settings         #", false, '1', [](){Serial.println();menu.load(SettingsMenu,SettingsMenuSize);menu.show();}},
{"# 2:                  #", false, ' ', {}},
{"# 3:                  #", false, ' ', {}},
{"# 4:                  #", false, ' ', {}},
{"# 5:                  #", false, ' ', {}},
{"# 6:                  #", false, ' ', {}},
{"# 7:                  #", false, ' ', {}},
{"# 8:                  #", false, ' ', {}},
{"# 9:                  #", false, ' ', {}},
{"# 0: Show menu        #", false, '0', [](){Serial.println();menu.show();}},
{"#######################", false, ' ',{}}
};

const SerialMenuEntry SettingsMenu[] ={
{"#######################", false, ' ',{}},
{"###  SETTINGS MENU  ###", false, ' ',{}},
{"#######################", false, ' ',{}},
{"# 1: Select slave     #", false, '1', [](){Serial.println();menu.show();}},
{"# 2: Current          #", false, '2', [](){printCurrent();menu.show();}},
{"# 3: Default          #", false, '3', [](){reset();menu.show();}},
{"# 4: Restart          #", false, '4', [](){resetart();menu.show();}},
{"# 5:                  #", false, ' ', {}},
{"# 6:                  #", false, ' ', {}},
{"# 7:                  #", false, ' ', {}},
{"# 8:                  #", false, ' ', {}},
{"# 9:                  #", false, ' ', {}},
{"# 0: Back             #", false, '0', [](){Serial.println();menu.load(MainMenu,MainMenuSize);menu.show();}},
{"#######################", false, ' ',{}}
};

constexpr uint8_t MainMenuSize = GET_MENU_SIZE(MainMenu);
constexpr uint8_t SettingsMenuSize = GET_MENU_SIZE(SettingsMenu);
