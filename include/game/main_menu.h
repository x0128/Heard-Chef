#pragma once

#include "core/menu.h"

constexpr int LOGO_FONT_SIZE = 64;
constexpr f32 LOGO_Y = 44.f;
constexpr Color MAIN_MENU_BACKGROUND = {20, 22, 30, 255};
constexpr Color LOGO_TEXT = {236, 232, 220, 255};

struct main_menu_scene {
    menu_state Menu;
    bool HasSave;
};

void EnterMainMenu();
void UpdateMainMenu();
void DrawMainMenu();
