#include "core/ui.h"
#include "game/game.h"
#include "game/scene_list.h"

constexpr f32 CONFIRM_TEXT_Y = 360.F;
constexpr Color CONFIRM_TEXT = { .r = 236, .g = 232, .b = 220, .a = 255 };

enum confirm_action : u8 {
  CONFIRM_YES,
  CONFIRM_NO,
};

static std::span<const menu_item> GetConfirmItems(menu_item_list &Out)
{
  u32 Count = 0;
  Out[Count++] = { .Label = "Yes, start over", .Action = CONFIRM_YES, .Kind = ITEM_PRIMARY };
  Out[Count++] = { .Label = "No", .Action = CONFIRM_NO, .Kind = ITEM_BUTTON };
  return std::span(Out).first(Count);
}

void EnterConfirm()
{
  Game->Confirm = {};
  Game->Confirm.Menu.Focus = 1;
}

void UpdateConfirm(f32)
{
  menu_item_list Items;
  switch (UpdateMenu(Game->Confirm.Menu, GetConfirmItems(Items), Game->Input, CONFIRM_NO)) {
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
  menu_item_list Items;
  DrawRectangleV({ .x = 0.F, .y = 0.F }, { .x = UI_WIDTH, .y = UI_HEIGHT }, Fade(BLACK, SOLID_OVERLAY_DIM));
  DrawCenteredText("Your save will be replaced.", CONFIRM_TEXT_Y, MENU_FONT_SIZE, CONFIRM_TEXT);
  DrawMenu(Game->Confirm.Menu, "Start a new game?", GetConfirmItems(Items), DEFAULT_MENU_THEME);
}
