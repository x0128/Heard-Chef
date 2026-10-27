#include "core/game_api.h"

#include <emscripten/emscripten.h>

static void UpdateWebFrame()
{
  GameUpdate();
  if (!GameShouldRun()) {
    emscripten_cancel_main_loop();
    GameShutdown();
    GameShutdownWindow();
  }
}

int main()
{
  GameInitWindow();
  GameInit();
  emscripten_set_main_loop(UpdateWebFrame, 0, 1);
  return 0;
}
