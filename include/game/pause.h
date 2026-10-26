#pragma once

#include "core/menu.h"

constexpr f32 PAUSE_DIM = 0.55f;

struct pause_scene {
    menu_state Menu;
};

void EnterPause();
void UpdatePause();
void DrawPause();
