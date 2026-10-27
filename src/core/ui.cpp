#include "core/ui.h"

#include <cmath>

Camera2D GetUiCameraFor(f32 ScreenWidth, f32 ScreenHeight)
{
  f32 Zoom = fminf(ScreenWidth / UI_WIDTH, ScreenHeight / UI_HEIGHT);
  Camera2D Camera = {};
  Camera.offset = { .x = (ScreenWidth - UI_WIDTH * Zoom) * HALF, .y = (ScreenHeight - UI_HEIGHT * Zoom) * HALF };
  Camera.zoom = Zoom;
  return Camera;
}

Camera2D GetUiCamera() { return GetUiCameraFor(static_cast<f32>(GetScreenWidth()), static_cast<f32>(GetScreenHeight())); }

void DrawCenteredText(const char *Text, f32 PosY, int FontSize, Color Tint)
{
  int Width = MeasureText(Text, FontSize);
  DrawText(Text, static_cast<int>(UI_WIDTH * HALF) - Width / 2, static_cast<int>(PosY), FontSize, Tint);
}
