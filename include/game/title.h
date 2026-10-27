#pragma once

#include "core/types.h"

#include <raylib.h>

struct title_scene
{ f32 Time = 0.F; };

void EnterTitle();
void UpdateTitle(f32 Deltatime);
void DrawTitle();
