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
  Out[Count++] = { .Label = "Master volume", .Action = SETTINGS_CHANGED, .Kind = ITEM_SLIDER, .Slider = &Settings.MasterVolume };
  Out[Count++] = { .Label = "Music", .Action = SETTINGS_CHANGED, .Kind = ITEM_SLIDER, .Slider = &Settings.MusicVolume };
  Out[Count++] = { .Label = "Sound effects", .Action = SETTINGS_CHANGED, .Kind = ITEM_SLIDER, .Slider = &Settings.SfxVolume };
#if !defined(__EMSCRIPTEN__)
  Out[Count++] = { .Label = "Fullscreen", .Action = SETTINGS_CHANGED, .Kind = ITEM_TOGGLE, .Slider = nullptr, .Toggle = &Settings.Fullscreen };
#endif
  Out[Count++] = { .Label = "Back", .Action = SETTINGS_BACK, .Kind = ITEM_FOOTER };
  return Count;
}

void EnterSettingsScreen() { Game->SettingsScreen = {}; }

void UpdateSettingsScreen(f32 /*Deltatime*/)
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
  DrawRectangleV({ .x = 0.F, .y = 0.F }, { .x = UI_WIDTH, .y = UI_HEIGHT }, Fade(BLACK, SOLID_OVERLAY_DIM));
  DrawMenu(Game->SettingsScreen.Menu, "Settings", Items, Count, DEFAULT_MENU_THEME);
}
