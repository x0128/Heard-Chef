#pragma once

#if defined(_WIN32) && defined(GAME_DLL)
#define GAME_EXPORT extern "C" __declspec(dllexport)
#else
#define GAME_EXPORT extern "C"
#endif

GAME_EXPORT void game_init_window();
GAME_EXPORT void game_init();
GAME_EXPORT void game_update();
GAME_EXPORT bool game_should_run();
GAME_EXPORT void game_shutdown();
GAME_EXPORT void game_shutdown_window();
GAME_EXPORT void* game_memory();
GAME_EXPORT int game_memory_size();
GAME_EXPORT void game_hot_reloaded(void* Memory);
GAME_EXPORT bool game_force_reload();
GAME_EXPORT bool game_force_restart();
GAME_EXPORT void game_parent_window_size_changed(int Width, int Height);
