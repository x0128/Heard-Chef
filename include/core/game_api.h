#pragma once

#if defined(_WIN32) && defined(GAME_DLL)
#define GAME_EXPORT extern "C" __declspec(dllexport)
#else
#define GAME_EXPORT extern "C"
#endif

GAME_EXPORT void GameInitWindow();
GAME_EXPORT void GameInit();
GAME_EXPORT void GameUpdate();
GAME_EXPORT bool GameShouldRun();
GAME_EXPORT void GameShutdown();
GAME_EXPORT void GameShutdownWindow();
GAME_EXPORT void *GameMemory();
GAME_EXPORT int GameMemorySize();
GAME_EXPORT void GameHotReloaded(void *Memory);
GAME_EXPORT bool GameForceReload();
GAME_EXPORT bool GameForceRestart();
GAME_EXPORT void GameParentWindowSizeChanged(int Width, int Height);
