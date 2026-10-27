#pragma once

#include "core/menu.h"
#include "core/scenes.h"
#include "core/types.h"

#include <raylib.h>

enum scene : scene_id {
  SCENE_TITLE,
  SCENE_MAIN_MENU,
  SCENE_CONFIRM,
  SCENE_PLAY,
  SCENE_PAUSE,
  SCENE_SETTINGS,
  SCENE_COUNT,
};

struct title_scene
{ f32 Time = 0.F; };
void EnterTitle();
void UpdateTitle(f32 Deltatime);
void DrawTitle();

struct main_menu_scene
{
  menu_state Menu;
  bool HasSave = false;
};
void EnterMainMenu();
void UpdateMainMenu(f32 Deltatime);
void DrawMainMenu();

struct confirm_scene
{ menu_state Menu; };
void EnterConfirm();
void UpdateConfirm(f32 Deltatime);
void DrawConfirm();

struct play_scene
{
  Vector2 Position = {};
  Vector2 PreviousPosition = {};
  f32 SaveTimer = 0.F;
};
void EnterPlay();
void ExitPlay();
void UpdatePlay(f32 Deltatime);
void DrawPlay();
bool HasPlaySave();
void WriteNewPlaySave();

struct pause_scene
{ menu_state Menu; };
void EnterPause();
void UpdatePause(f32 Deltatime);
void DrawPause();

struct settings_scene
{ menu_state Menu; };
void EnterSettingsScreen();
void UpdateSettingsScreen(f32 Deltatime);
void DrawSettingsScreen();
