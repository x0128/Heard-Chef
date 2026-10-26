#include "game/game.h"

#include "core/game_api.h"
#include "core/ui.h"

#include <raylib.h>

game_state* Game = nullptr;

GAME_EXPORT void game_init_window() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow((int)UI_WIDTH, (int)UI_HEIGHT, GAME_TITLE);
    SetWindowMinSize(640, 360);
    SetExitKey(KEY_NULL);
    InitAudioDevice();
}

GAME_EXPORT void game_init() {
    Game = new game_state{};
    EnterPlay();
}

GAME_EXPORT void game_update() {
    UpdatePlay(GetFrameTime());
    BeginDrawing();
    ClearBackground(BLACK);
    BeginMode2D(GetUiCamera());
    DrawPlay();
    EndMode2D();
    EndDrawing();
}

GAME_EXPORT bool game_should_run() {
#if !defined(__EMSCRIPTEN__)
    if (WindowShouldClose()) {
        return false;
    }
#endif
    return Game->Run;
}

GAME_EXPORT void game_shutdown() {
    delete Game;
    Game = nullptr;
}

GAME_EXPORT void game_shutdown_window() {
    CloseAudioDevice();
    CloseWindow();
}

GAME_EXPORT void* game_memory() {
    return Game;
}

GAME_EXPORT int game_memory_size() {
    return (int)sizeof(game_state);
}

GAME_EXPORT void game_hot_reloaded(void* Memory) {
    Game = (game_state*)Memory;
}

GAME_EXPORT bool game_force_reload() {
    return IsKeyPressed(KEY_F5);
}

GAME_EXPORT bool game_force_restart() {
    return IsKeyPressed(KEY_F6);
}

GAME_EXPORT void game_parent_window_size_changed(int Width, int Height) {
    SetWindowSize(Width, Height);
}
