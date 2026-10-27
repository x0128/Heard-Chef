#include "core/ui.h"
#include "game/game.h"
#include "game/scene_list.h"

constexpr f32 CONFIRM_TEXT_Y = 360.F;
constexpr Color CONFIRM_TEXT = { .r = 236, .g = 232, .b = 220, .a = 255 };

enum confirm_action : u8 {
  CONFIRM_YES,
  CONFIRM_NO,
};

static u32 GetConfirmItems(menu_item *Out)
{
  u32 Count = 0;
  Out[Count++] = { .Label = "Yes, start over", .Action = CONFIRM_YES, .Kind = ITEM_PRIMARY };
  Out[Count++] = { .Label = "No", .Action = CONFIRM_NO, .Kind = ITEM_FOOTER };
  return Count;
}

void EnterConfirm()
{
  Game->Confirm = {};
  Game->Confirm.Menu.Focus = 1;
}

void UpdateConfirm(f32)
{
  menu_item Items[MAX_MENU_ITEMS];
  u32 Count = GetConfirmItems(Items);
  switch (UpdateMenu(Game->Confirm.Menu, Items, Count, Game->Input, CONFIRM_NO)) {
  case CONFIRM_YES:
    WriteNewPlaySave();
    ChangeScene(Game->Scenes, SCENE_PLAY);
    break;
  case CONFIRM_NO:
    PopScene(Game->Scenes);
    break;
  default:
    break;
  }
}

void DrawConfirm()
{
  menu_item Items[MAX_MENU_ITEMS];
  u32 Count = GetConfirmItems(Items);
  DrawRectangleV({ .x = 0.F, .y = 0.F }, { .x = UI_WIDTH, .y = UI_HEIGHT }, Fade(BLACK, SOLID_OVERLAY_DIM));
  DrawCenteredText("Your save will be replaced.", CONFIRM_TEXT_Y, MENU_FONT_SIZE, CONFIRM_TEXT);
  DrawMenu(Game->Confirm.Menu, "Start a new game?", Items, Count, DEFAULT_MENU_THEME);
}
