#include "game/confirm.h"

#include "core/ui.h"
#include "game/game.h"
#include "game/scene_list.h"

enum confirm_action : u8 {
    CONFIRM_YES,
    CONFIRM_NO,
};

static u32 GetConfirmItems(menu_item* Out) {
    u32 Count = 0;
    Out[Count++] = {"Yes, start over", CONFIRM_YES, ITEM_PRIMARY};
    Out[Count++] = {"No", CONFIRM_NO, ITEM_FOOTER};
    return Count;
}

void EnterConfirm() {
    Game->Confirm = {};
    Game->Confirm.Menu.Focus = 1;
}

void UpdateConfirm() {
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

void DrawConfirm() {
    menu_item Items[MAX_MENU_ITEMS];
    u32 Count = GetConfirmItems(Items);
    DrawRectangleV({0.f, 0.f}, {UI_WIDTH, UI_HEIGHT}, Fade(BLACK, SOLID_OVERLAY_DIM));
    DrawCenteredText("Your save will be replaced.", CONFIRM_TEXT_Y, MENU_FONT_SIZE, CONFIRM_TEXT);
    DrawMenu(Game->Confirm.Menu, "Start a new game?", Items, Count, DEFAULT_MENU_THEME);
}
