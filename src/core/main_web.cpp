#include "core/game_api.h"

#include <emscripten/emscripten.h>

static void UpdateWebFrame() {
    game_update();
    if (!game_should_run()) {
        emscripten_cancel_main_loop();
        game_shutdown();
        game_shutdown_window();
    }
}

int main() {
    game_init_window();
    game_init();
    emscripten_set_main_loop(UpdateWebFrame, 0, 1);
    return 0;
}
