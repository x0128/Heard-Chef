#include "game/game.h"

#include "core/config.h"
#include "core/game_api.h"
#include "core/ui.h"
#include "game/scene_list.h"
#include <array>
#include <raylib.h>

game_state *Game = nullptr;

constexpr int MIN_WINDOW_WIDTH = 640;
constexpr int MIN_WINDOW_HEIGHT = 360;

struct scene_funcs
{
  scene Id;
  void (*Enter)();
  void (*Exit)();
  void (*Update)(f32 Deltatime);
  void (*Draw)();
};

static constexpr std::array<scene_funcs, scene::SCENE_COUNT> SCENE_FUNCS = { {
  { .Id = SCENE_TITLE, .Enter = EnterTitle, .Update = UpdateTitle, .Draw = DrawTitle },
  { .Id = SCENE_MAIN_MENU, .Enter = EnterMainMenu, .Update = UpdateMainMenu, .Draw = DrawMainMenu },
  { .Id = SCENE_CONFIRM, .Enter = EnterConfirm, .Update = UpdateConfirm, .Draw = DrawConfirm },
  { .Id = SCENE_PLAY, .Enter = EnterPlay, .Exit = ExitPlay, .Update = UpdatePlay, .Draw = DrawPlay },
  { .Id = SCENE_PAUSE, .Enter = EnterPause, .Update = UpdatePause, .Draw = DrawPause },
  { .Id = SCENE_SETTINGS, .Enter = EnterSettingsScreen, .Update = UpdateSettingsScreen, .Draw = DrawSettingsScreen },
} };

static constexpr bool SceneFuncsMatchEnum()
{
  if (std::size(SCENE_FUNCS) != SCENE_COUNT) { return false; }
  for (u32 Index = 0; Index < SCENE_COUNT; ++Index) {
    if (SCENE_FUNCS.at(Index).Id != Index) { return false; }
  }
  return true;
}
static_assert(SceneFuncsMatchEnum(), "SCENE_FUNCS needs one row per scene, in enum scene order");

void EnterScene(scene_id Scene) { SCENE_FUNCS.at(Scene).Enter(); }

void ExitScene(scene_id Scene)
{
  if (SCENE_FUNCS.at(Scene).Exit != nullptr) { SCENE_FUNCS.at(Scene).Exit(); }
}

void UpdateScene(scene_id Scene, f32 Deltatime) { SCENE_FUNCS.at(Scene).Update(Deltatime); }

void DrawScene(scene_id Scene) { SCENE_FUNCS.at(Scene).Draw(); }

GAME_EXPORT void GameInitWindow()
{
  SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
  InitWindow((int)UI_WIDTH, (int)UI_HEIGHT, GAME_TITLE);
#if !defined(__EMSCRIPTEN__)
  SetWindowMinSize(MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT);
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

GAME_EXPORT void GameHotReloaded(void *Memory) { Game = static_cast<game_state *>(Memory); }

GAME_EXPORT bool GameForceReload() { return IsKeyPressed(KEY_F5); }

GAME_EXPORT bool GameForceRestart() { return IsKeyPressed(KEY_F6); }

GAME_EXPORT void GameParentWindowSizeChanged(int Width, int Height) { SetWindowSize(Width, Height); }
