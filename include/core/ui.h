#pragma once

#include "core/types.h"

#include <raylib.h>

constexpr f32 UI_WIDTH = 1280.f;
constexpr f32 UI_HEIGHT = 720.f;

Camera2D GetUiCameraFor(f32 ScreenWidth, f32 ScreenHeight);
Camera2D GetUiCamera();
