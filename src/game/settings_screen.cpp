#include "core/ui.h"
#include "game/game.h"

enum settings_action : u8 {
  SETTINGS_CHANGED,
  SETTINGS_BACK,
};

static std::span<const menu_item> GetSettingsItems(menu_item_list &Out)
{
  settings &Settings = Game->Settings;
  u32 Count = 0;
  Out[Count++] = { .Label = "Master volume", .Action = SETTINGS_CHANGED, .Kind = ITEM_SLIDER, .Slider = &Settings.MasterVolume };
  Out[Count++] = { .Label = "Music", .Action = SETTINGS_CHANGED, .Kind = ITEM_SLIDER, .Slider = &Settings.MusicVolume };
  Out[Count++] = { .Label = "Sound effects", .Action = SETTINGS_CHANGED, .Kind = ITEM_SLIDER, .Slider = &Settings.SfxVolume };
#if !defined(__EMSCRIPTEN__)
  Out[Count++] = { .Label = "Fullscreen", .Action = SETTINGS_CHANGED, .Kind = ITEM_TOGGLE, .Slider = nullptr, .Toggle = &Settings.Fullscreen };
#endif
  Out[Count++] = { .Label = "Back", .Action = SETTINGS_BACK, .Kind = ITEM_BUTTON };
  return std::span(Out).first(Count);
}

void EnterSettingsScreen() { Game->SettingsScreen = {}; }

void UpdateSettingsScreen(f32)
{
  menu_item_list Items;
  switch (UpdateMenu(Game->SettingsScreen.Menu, GetSettingsItems(Items), Game->Input, SETTINGS_BACK)) {
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
  menu_item_list Items;
  DrawRectangleV({ .x = 0.F, .y = 0.F }, { .x = UI_WIDTH, .y = UI_HEIGHT }, Fade(BLACK, SOLID_OVERLAY_DIM));
  DrawMenu(Game->SettingsScreen.Menu, "Settings", GetSettingsItems(Items), DEFAULT_MENU_THEME);
}
