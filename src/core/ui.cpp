#include "core/ui.h"

#include <cmath>

Camera2D GetUiCameraFor(f32 ScreenWidth, f32 ScreenHeight)
{
  f32 Zoom = fminf(ScreenWidth / UI_WIDTH, ScreenHeight / UI_HEIGHT);
  Camera2D Camera = {};
  Camera.offset = { .x = (ScreenWidth - UI_WIDTH * Zoom) * 0.5F, .y = (ScreenHeight - UI_HEIGHT * Zoom) * 0.5F };
  Camera.zoom = Zoom;
  return Camera;
}

Camera2D GetUiCamera() { return GetUiCameraFor((f32)GetScreenWidth(), (f32)GetScreenHeight()); }

void DrawCenteredText(const char *Text, f32 PosY, int FontSize, Color Tint)
{
  int Width = MeasureText(Text, FontSize);
  DrawText(Text, (int)(UI_WIDTH * 0.5F) - Width / 2, (int)PosY, FontSize, Tint);
}
