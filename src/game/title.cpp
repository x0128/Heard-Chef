#include "core/ui.h"
#include "game/game.h"
#include "game/scene_list.h"

constexpr int TITLE_FONT_SIZE = 72;
constexpr int PROMPT_FONT_SIZE = 24;
constexpr f32 TITLE_Y = UI_HEIGHT * 0.38F;
constexpr f32 PROMPT_Y = UI_HEIGHT * 0.62F;
constexpr f32 PROMPT_BLINK_SECONDS = 1.F;
constexpr f32 PROMPT_VISIBLE_SECONDS = 0.6F;
constexpr Color TITLE_BACKGROUND = { .r = 20, .g = 22, .b = 30, .a = 255 };
constexpr Color TITLE_TEXT = { .r = 236, .g = 232, .b = 220, .a = 255 };
constexpr Color PROMPT_TEXT = { .r = 150, .g = 156, .b = 170, .a = 255 };

void EnterTitle() { Game->Title = {}; }

void UpdateTitle(f32 Deltatime)
{
  Game->Title.Time += Deltatime;
  if (Game->Input.AnyPressed) { ChangeScene(Game->Scenes, SCENE_MAIN_MENU); }
}

void DrawTitle()
{
  DrawRectangleV({ .x = 0.F, .y = 0.F }, { .x = UI_WIDTH, .y = UI_HEIGHT }, TITLE_BACKGROUND);
  DrawCenteredText(GAME_TITLE, TITLE_Y, TITLE_FONT_SIZE, TITLE_TEXT);
  if (fmodf(Game->Title.Time, PROMPT_BLINK_SECONDS) < PROMPT_VISIBLE_SECONDS) {
    DrawCenteredText("press any key", PROMPT_Y, PROMPT_FONT_SIZE, PROMPT_TEXT);
  }
}
