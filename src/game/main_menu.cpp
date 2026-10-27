#include "game/main_menu.h"

#include "core/ui.h"
#include "game/game.h"
#include "game/scene_list.h"

enum main_menu_action : u8 {
  MAIN_CONTINUE,
  MAIN_NEW_GAME,
  MAIN_SETTINGS,
  MAIN_EXIT,
};

static u32 GetMainMenuItems(menu_item *Out)
{
  bool HasSave = Game->MainMenu.HasSave;
  u32 Count = 0;
  if (HasSave) { Out[Count++] = { .Label = "Continue", .Action = MAIN_CONTINUE, .Kind = ITEM_PRIMARY }; }
  Out[Count++] = { .Label = "New game", .Action = MAIN_NEW_GAME, .Kind = HasSave ? ITEM_BUTTON : ITEM_PRIMARY };
  Out[Count++] = { .Label = "Settings", .Action = MAIN_SETTINGS, .Kind = ITEM_BUTTON };
#if !defined(__EMSCRIPTEN__)
  Out[Count++] = { .Label = "Exit", .Action = MAIN_EXIT, .Kind = ITEM_FOOTER };
#endif
  return Count;
}

void EnterMainMenu()
{
  Game->MainMenu = {};
  Game->MainMenu.HasSave = HasPlaySave();
}

void UpdateMainMenu(f32 /*Deltatime*/)
{
  menu_item Items[MAX_MENU_ITEMS];
  u32 Count = GetMainMenuItems(Items);
  switch (UpdateMenu(Game->MainMenu.Menu, Items, Count, Game->Input, NO_ACTION)) {
  case MAIN_CONTINUE:
    ChangeScene(Game->Scenes, SCENE_PLAY);
    break;
  case MAIN_NEW_GAME:
    if (Game->MainMenu.HasSave) {
      PushScene(Game->Scenes, SCENE_CONFIRM);
    } else {
      WriteNewPlaySave();
      ChangeScene(Game->Scenes, SCENE_PLAY);
    }
    break;
  case MAIN_SETTINGS:
    PushScene(Game->Scenes, SCENE_SETTINGS);
    break;
  case MAIN_EXIT:
    Game->Run = false;
    break;
  default:
    break;
  }
}

void DrawMainMenu()
{
  menu_item Items[MAX_MENU_ITEMS];
  u32 Count = GetMainMenuItems(Items);
  DrawRectangleV({ .x = 0.F, .y = 0.F }, { .x = UI_WIDTH, .y = UI_HEIGHT }, MAIN_MENU_BACKGROUND);
  DrawCenteredText(GAME_TITLE, LOGO_Y, LOGO_FONT_SIZE, LOGO_TEXT);
  DrawMenu(Game->MainMenu.Menu, "Main menu", Items, Count, DEFAULT_MENU_THEME);
}
