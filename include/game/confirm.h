#pragma once

#include "core/menu.h"

constexpr f32 CONFIRM_TEXT_Y = 360.F;
constexpr Color CONFIRM_TEXT = { .r = 236, .g = 232, .b = 220, .a = 255 };

struct confirm_scene
{ menu_state Menu; };

void EnterConfirm();
void UpdateConfirm();
void DrawConfirm();
