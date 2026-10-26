#pragma once

#include "core/menu.h"

struct settings_scene {
    menu_state Menu;
};

void EnterSettingsScreen();
void UpdateSettingsScreen();
void DrawSettingsScreen();
