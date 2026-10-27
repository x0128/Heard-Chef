#pragma once

#include "core/input.h"
#include "core/loop.h"
#include "core/scenes.h"
#include "core/settings.h"
#include "game/scene_list.h"

constexpr const char *GAME_TITLE = "GAME TITLE";
constexpr const char *SAVE_FILE = "game_save.bin";
constexpr const char *SETTINGS_FILE = "game_settings.bin";

struct game_state
{
  bool Run = true;
  fixed_clock Clock;
  stick_state Stick;
  frame_input PendingInput;
  frame_input Input;
  settings Settings;

  scene_stack Scenes;
  title_scene Title;
  main_menu_scene MainMenu;
  play_scene Play;
  pause_scene Pause;
  confirm_scene Confirm;
  settings_scene SettingsScreen;
};

extern game_state *Game;
