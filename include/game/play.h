#pragma once

#include "core/types.h"
#include "core/ui.h"

#include <raylib.h>

constexpr f32 PLAY_SPEED = 300.f;
constexpr f32 SQUARE_SIZE = 40.f;
constexpr f32 STICK_DEADZONE = 0.2f;
constexpr Vector2 PLAY_START = { UI_WIDTH * 0.5f, UI_HEIGHT * 0.5f };
constexpr Color PLAY_BACKGROUND = { 38, 44, 58, 255 };
constexpr Color SQUARE_COLOR = { 242, 179, 61, 255 };

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
