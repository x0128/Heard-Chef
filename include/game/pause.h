#pragma once

#include "core/menu.h"

struct pause_scene {
    menu_state Menu;
};

void EnterPause();
void UpdatePause(f32 Deltatime);
void DrawPause();
