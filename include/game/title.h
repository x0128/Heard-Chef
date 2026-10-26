#pragma once

#include "core/types.h"

#include <raylib.h>

constexpr int TITLE_FONT_SIZE = 72;
constexpr int PROMPT_FONT_SIZE = 24;
constexpr f32 PROMPT_BLINK_SECONDS = 1.f;
constexpr f32 PROMPT_VISIBLE_SECONDS = 0.6f;
constexpr Color TITLE_BACKGROUND = {20, 22, 30, 255};
constexpr Color TITLE_TEXT = {236, 232, 220, 255};
constexpr Color PROMPT_TEXT = {150, 156, 170, 255};

struct title_scene {
    f32 Time;
};

void EnterTitle();
void UpdateTitle(f32 Dt);
void DrawTitle();
