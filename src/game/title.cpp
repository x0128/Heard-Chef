#include "game/title.h"

#include "core/ui.h"
#include "game/game.h"
#include "game/scene_list.h"

#include <cmath>

void EnterTitle() {
    Game->Title = {};
}

void UpdateTitle(f32 Dt) {
    Game->Title.Time += Dt;
    if (Game->Input.AnyPressed) {
        ChangeScene(Game->Scenes, SCENE_MAIN_MENU);
    }
}

void DrawTitle() {
    DrawRectangleV({0.f, 0.f}, {UI_WIDTH, UI_HEIGHT}, TITLE_BACKGROUND);
    DrawCenteredText(GAME_TITLE, UI_HEIGHT * 0.38f, TITLE_FONT_SIZE, TITLE_TEXT);
    if (fmodf(Game->Title.Time, PROMPT_BLINK_SECONDS) < PROMPT_VISIBLE_SECONDS) {
        DrawCenteredText("press any key", UI_HEIGHT * 0.62f, PROMPT_FONT_SIZE, PROMPT_TEXT);
    }
}
