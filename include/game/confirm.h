#pragma once

#include "core/menu.h"

constexpr f32 CONFIRM_TEXT_Y = 360.f;
constexpr Color CONFIRM_TEXT = {236, 232, 220, 255};

struct confirm_scene {
    menu_state Menu;
};

void EnterConfirm();
void UpdateConfirm();
void DrawConfirm();
