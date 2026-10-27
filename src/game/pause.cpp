#include "core/ui.h"
#include "game/game.h"
#include "game/scene_list.h"

enum pause_action : u8 {
  PAUSE_RESUME,
  PAUSE_SETTINGS,
  PAUSE_QUIT,
};

static std::span<const menu_item> GetPauseItems(menu_item_list &Out)
{
  u32 Count = 0;
  Out[Count++] = { .Label = "Resume", .Action = PAUSE_RESUME, .Kind = ITEM_PRIMARY };
  Out[Count++] = { .Label = "Settings", .Action = PAUSE_SETTINGS, .Kind = ITEM_BUTTON };
  Out[Count++] = { .Label = "Quit to menu", .Action = PAUSE_QUIT, .Kind = ITEM_BUTTON };
  return std::span(Out).first(Count);
}

void EnterPause() { Game->Pause = {}; }

void UpdatePause(f32)
{
  menu_item_list Items;
  switch (UpdateMenu(Game->Pause.Menu, GetPauseItems(Items), Game->Input, PAUSE_RESUME)) {
  case PAUSE_RESUME:
    PopScene(Game->Scenes);
    break;
  case PAUSE_SETTINGS:
    PushScene(Game->Scenes, SCENE_SETTINGS);
    break;
  case PAUSE_QUIT:
    ChangeScene(Game->Scenes, SCENE_MAIN_MENU);
    break;
  default:
    break;
  }
}

void DrawPause()
{
  menu_item_list Items;
  DrawRectangleV({ .x = 0.F, .y = 0.F }, { .x = UI_WIDTH, .y = UI_HEIGHT }, Fade(BLACK, OVERLAY_DIM));
  DrawMenu(Game->Pause.Menu, "Paused", GetPauseItems(Items), DEFAULT_MENU_THEME);
}
