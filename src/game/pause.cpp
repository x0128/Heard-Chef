#include "core/ui.h"
#include "game/game.h"
#include "game/scene_list.h"

enum pause_action : u8 {
  PAUSE_RESUME,
  PAUSE_SETTINGS,
  PAUSE_QUIT,
};

static u32 GetPauseItems(menu_item *Out)
{
  u32 Count = 0;
  Out[Count++] = { .Label = "Resume", .Action = PAUSE_RESUME, .Kind = ITEM_PRIMARY };
  Out[Count++] = { .Label = "Settings", .Action = PAUSE_SETTINGS, .Kind = ITEM_BUTTON };
  Out[Count++] = { .Label = "Quit to menu", .Action = PAUSE_QUIT, .Kind = ITEM_FOOTER };
  return Count;
}

void EnterPause() { Game->Pause = {}; }

void UpdatePause(f32)
{
  menu_item Items[MAX_MENU_ITEMS];
  u32 Count = GetPauseItems(Items);
  switch (UpdateMenu(Game->Pause.Menu, Items, Count, Game->Input, PAUSE_RESUME)) {
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
  menu_item Items[MAX_MENU_ITEMS];
  u32 Count = GetPauseItems(Items);
  DrawRectangleV({ .x = 0.F, .y = 0.F }, { .x = UI_WIDTH, .y = UI_HEIGHT }, Fade(BLACK, OVERLAY_DIM));
  DrawMenu(Game->Pause.Menu, "Paused", Items, Count, DEFAULT_MENU_THEME);
}
