#include "game/pause.h"

#include "core/ui.h"
#include "game/game.h"
#include "game/scene_list.h"

enum pause_action : u8 {
    PAUSE_RESUME,
    PAUSE_SETTINGS,
    PAUSE_QUIT,
};

static u32 GetPauseItems(menu_item* Out) {
    u32 Count = 0;
    Out[Count++] = {"Resume", PAUSE_RESUME, ITEM_PRIMARY};
    Out[Count++] = {"Settings", PAUSE_SETTINGS, ITEM_BUTTON};
    Out[Count++] = {"Quit to menu", PAUSE_QUIT, ITEM_FOOTER};
    return Count;
}

void EnterPause() {
    Game->Pause = {};
}

void UpdatePause() {
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

void DrawPause() {
    menu_item Items[MAX_MENU_ITEMS];
    u32 Count = GetPauseItems(Items);
    DrawRectangleV({0.f, 0.f}, {UI_WIDTH, UI_HEIGHT}, Fade(BLACK, OVERLAY_DIM));
    DrawMenu(Game->Pause.Menu, "Paused", Items, Count, DEFAULT_MENU_THEME);
}
