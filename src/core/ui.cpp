#include "core/ui.h"

#include <cmath>

Camera2D GetUiCameraFor(f32 ScreenWidth, f32 ScreenHeight) {
    f32 Zoom = fminf(ScreenWidth / UI_WIDTH, ScreenHeight / UI_HEIGHT);
    Camera2D Camera = {};
    Camera.offset = {(ScreenWidth - UI_WIDTH * Zoom) * 0.5f, (ScreenHeight - UI_HEIGHT * Zoom) * 0.5f};
    Camera.zoom = Zoom;
    return Camera;
}

Camera2D GetUiCamera() {
    return GetUiCameraFor((f32)GetScreenWidth(), (f32)GetScreenHeight());
}

void DrawCenteredText(const char* Text, f32 Y, int FontSize, Color Tint) {
    int Width = MeasureText(Text, FontSize);
    DrawText(Text, (int)(UI_WIDTH * 0.5f) - Width / 2, (int)Y, FontSize, Tint);
}
