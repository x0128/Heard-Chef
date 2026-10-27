#pragma once

#include "core/menu.h"

constexpr int LOGO_FONT_SIZE = 64;
constexpr f32 LOGO_Y = 44.F;
constexpr Color MAIN_MENU_BACKGROUND = { .r = 20, .g = 22, .b = 30, .a = 255 };
constexpr Color LOGO_TEXT = { .r = 236, .g = 232, .b = 220, .a = 255 };

struct main_menu_scene
{
  menu_state Menu;
  bool HasSave = false;
};

void EnterMainMenu();
void UpdateMainMenu(f32 Deltatime);
void DrawMainMenu();
