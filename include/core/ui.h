#pragma once

#include "core/types.h"

#include <raylib.h>

constexpr f32 UI_WIDTH = 1280.F;
constexpr f32 UI_HEIGHT = 720.F;

Camera2D GetUiCameraFor(f32 ScreenWidth, f32 ScreenHeight);
Camera2D GetUiCamera();
void DrawCenteredText(const char *Text, f32 PosY, int FontSize, Color Tint);
