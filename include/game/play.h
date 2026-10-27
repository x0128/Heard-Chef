#pragma once

#include "core/types.h"
#include "core/ui.h"

#include <raylib.h>

constexpr f32 PLAY_SPEED = 300.F;
constexpr f32 SQUARE_SIZE = 40.F;
constexpr f32 STICK_DEADZONE = 0.2F;
constexpr Vector2 PLAY_START = { .x = UI_WIDTH * 0.5F, .y = UI_HEIGHT * 0.5F };
constexpr Color PLAY_BACKGROUND = { .r = 38, .g = 44, .b = 58, .a = 255 };
constexpr Color SQUARE_COLOR = { .r = 242, .g = 179, .b = 61, .a = 255 };

constexpr u32 PLAY_SAVE_MAGIC = 0x504c4159;
constexpr u32 PLAY_SAVE_VERSION = 1;
constexpr f32 PLAY_SAVE_SECONDS = 5.F;

struct play_save
{ Vector2 Position; };

struct play_scene
{
  Vector2 Position = {};
  Vector2 PreviousPosition = {};
  f32 SaveTimer = 0.F;
};

void EnterPlay();
void ExitPlay();
void UpdatePlay(f32 Deltatime);
void DrawPlay();

bool HasPlaySave();
void WriteNewPlaySave();
void SavePlay();
