#include "core/game_api.h"

#include <raylib.h>

int main()
{
  ChangeDirectory(GetApplicationDirectory());
  GameInitWindow();
  GameInit();
  while (GameShouldRun()) { GameUpdate(); }
  GameShutdown();
  GameShutdownWindow();
  return 0;
}

#if defined(_WIN32)
extern "C" __declspec(dllexport) unsigned long NvOptimusEnablement = 1;
extern "C" __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
#endif
