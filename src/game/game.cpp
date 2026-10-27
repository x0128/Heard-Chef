#include "game/game.h"

#include "core/config.h"
#include "core/game_api.h"
#include "core/ui.h"
#include "game/scene_list.h"

#include <raylib.h>

game_state *Game = nullptr;

void EnterScene(scene_id Scene)
{
  switch (Scene) {
  case SCENE_TITLE:
    EnterTitle();
    break;
  case SCENE_MAIN_MENU:
    EnterMainMenu();
    break;
  case SCENE_PAUSE:
    EnterPause();
    break;
  case SCENE_CONFIRM:
    EnterConfirm();
    break;
  case SCENE_SETTINGS:
    EnterSettingsScreen();
    break;
  case SCENE_PLAY:
    EnterPlay();
    break;
  default:
    break;
  }
}

void ExitScene(scene_id Scene)
{
  switch (Scene) {
  case SCENE_PLAY:
    ExitPlay();
    break;
  default:
    break;
  }
}

void UpdateScene(scene_id Scene, f32 Dt)
{
  switch (Scene) {
  case SCENE_TITLE:
    UpdateTitle(Dt);
    break;
  case SCENE_MAIN_MENU:
    UpdateMainMenu();
    break;
  case SCENE_PAUSE:
    UpdatePause();
    break;
  case SCENE_CONFIRM:
    UpdateConfirm();
    break;
  case SCENE_SETTINGS:
    UpdateSettingsScreen();
    break;
  case SCENE_PLAY:
    UpdatePlay(Dt);
    break;
  default:
    break;
  }
}

void DrawScene(scene_id Scene)
{
  switch (Scene) {
  case SCENE_TITLE:
    DrawTitle();
    break;
  case SCENE_MAIN_MENU:
    DrawMainMenu();
    break;
  case SCENE_PAUSE:
    DrawPause();
    break;
  case SCENE_CONFIRM:
    DrawConfirm();
    break;
  case SCENE_SETTINGS:
    DrawSettingsScreen();
    break;
  case SCENE_PLAY:
    DrawPlay();
    break;
  default:
    break;
  }
}

GAME_EXPORT void GameInitWindow()
{
  SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
  InitWindow((int)UI_WIDTH, (int)UI_HEIGHT, GAME_TITLE);
#if !defined(__EMSCRIPTEN__)
  SetWindowMinSize(640, 360);
#endif
  SetExitKey(KEY_NULL);
  InitAudioDevice();
}

GAME_EXPORT void GameInit()
{
  Game = new game_state{};
  Game->Settings = LoadSettings(SETTINGS_FILE);
  ApplySettings(Game->Settings);
  StartScenes(Game->Scenes, SCENE_TITLE);
}

GAME_EXPORT void GameUpdate()
{
  Camera2D UiCamera = GetUiCamera();
  MergeInput(Game->PendingInput, ReadFrameInput(Game->Stick, UiCamera));
  u32 Steps = AdvanceClock(Game->Clock, GetFrameTime());
  for (u32 Step = 0; Step < Steps; ++Step) {
    Game->Input = TakeInput(Game->PendingInput);
    UpdateScenes(Game->Scenes, FIXED_STEP);
  }
  BeginDrawing();
  ClearBackground(BLACK);
  BeginMode2D(UiCamera);
  DrawScenes(Game->Scenes);
  EndMode2D();
  DrawFadeOverlay(Game->Scenes);
  EndDrawing();
}

GAME_EXPORT bool GameShouldRun()
{
#if !defined(__EMSCRIPTEN__)
  if (WindowShouldClose()) { return false; }
#endif
  return Game->Run;
}

GAME_EXPORT void GameShutdown()
{
  StopScenes(Game->Scenes);
  delete Game;
  Game = nullptr;
}

GAME_EXPORT void GameShutdownWindow()
{
  CloseAudioDevice();
  CloseWindow();
}

GAME_EXPORT void *GameMemory() { return Game; }

GAME_EXPORT int GameMemorySize() { return (int)sizeof(game_state); }

GAME_EXPORT void GameHotReloaded(void *Memory) { Game = (game_state *)Memory; }

GAME_EXPORT bool GameForceReload() { return IsKeyPressed(KEY_F5); }

GAME_EXPORT bool GameForceRestart() { return IsKeyPressed(KEY_F6); }

GAME_EXPORT void GameParentWindowSizeChanged(int Width, int Height) { SetWindowSize(Width, Height); }
