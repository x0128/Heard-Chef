#include "core/game_api.h"

#include <raylib.h>

int main()
{
  ChangeDirectory(GetApplicationDirectory());
  game_init_window();
  game_init();
  while (game_should_run()) { game_update(); }
  game_shutdown();
  game_shutdown_window();
  return 0;
}

#if defined(_WIN32)
extern "C" __declspec(dllexport) unsigned long NvOptimusEnablement = 1;
extern "C" __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
#endif
