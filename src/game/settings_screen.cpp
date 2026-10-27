#include "game/settings_screen.h"

#include "core/ui.h"
#include "game/game.h"

enum settings_action : u8 {
  SETTINGS_CHANGED,
  SETTINGS_BACK,
};

static u32 GetSettingsItems(menu_item *Out)
{
  settings &Settings = Game->Settings;
  u32 Count = 0;
  Out[Count++] = { "Master volume", SETTINGS_CHANGED, ITEM_SLIDER, &Settings.MasterVolume };
  Out[Count++] = { "Music", SETTINGS_CHANGED, ITEM_SLIDER, &Settings.MusicVolume };
  Out[Count++] = { "Sound effects", SETTINGS_CHANGED, ITEM_SLIDER, &Settings.SfxVolume };
#if !defined(__EMSCRIPTEN__)
  Out[Count++] = { "Fullscreen", SETTINGS_CHANGED, ITEM_TOGGLE, nullptr, &Settings.Fullscreen };
#endif
  Out[Count++] = { "Back", SETTINGS_BACK, ITEM_FOOTER };
  return Count;
}

void EnterSettingsScreen() { Game->SettingsScreen = {}; }

void UpdateSettingsScreen()
{
  menu_item Items[MAX_MENU_ITEMS];
  u32 Count = GetSettingsItems(Items);
  switch (UpdateMenu(Game->SettingsScreen.Menu, Items, Count, Game->Input, SETTINGS_BACK)) {
  case SETTINGS_CHANGED:
    ApplySettings(Game->Settings);
    SaveSettings(SETTINGS_FILE, Game->Settings);
    break;
  case SETTINGS_BACK:
    SaveSettings(SETTINGS_FILE, Game->Settings);
    PopScene(Game->Scenes);
    break;
  default:
    break;
  }
}

void DrawSettingsScreen()
{
  menu_item Items[MAX_MENU_ITEMS];
  u32 Count = GetSettingsItems(Items);
  DrawRectangleV({ 0.f, 0.f }, { UI_WIDTH, UI_HEIGHT }, Fade(BLACK, SOLID_OVERLAY_DIM));
  DrawMenu(Game->SettingsScreen.Menu, "Settings", Items, Count, DEFAULT_MENU_THEME);
}
