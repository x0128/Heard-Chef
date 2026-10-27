#include "core/game_api.h"
#include "core/types.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <cstdio>
#include <cstring>
#include <windows.h>

constexpr const char *GAME_DLL_NAME = "game.dll";
constexpr const char *GAME_LOCK_NAME = "game.lock";
constexpr u32 MAX_OLD_APIS = 64;

struct game_api_table
{
  HMODULE Library;
  decltype(&game_init_window) InitWindow;
  decltype(&game_init) Init;
  decltype(&game_update) Update;
  decltype(&game_should_run) ShouldRun;
  decltype(&game_shutdown) Shutdown;
  decltype(&game_shutdown_window) ShutdownWindow;
  decltype(&game_memory) Memory;
  decltype(&game_memory_size) MemorySize;
  decltype(&game_hot_reloaded) HotReloaded;
  decltype(&game_force_reload) ForceReload;
  decltype(&game_force_restart) ForceRestart;
  FILETIME WriteTime;
  u32 Version;
};

static bool GetWriteTime(const char *Path, FILETIME &Time)
{
  WIN32_FILE_ATTRIBUTE_DATA Data;
  if (!GetFileAttributesExA(Path, GetFileExInfoStandard, &Data)) { return false; }
  Time = Data.ftLastWriteTime;
  return true;
}

static bool IsBuildLocked() { return GetFileAttributesA(GAME_LOCK_NAME) != INVALID_FILE_ATTRIBUTES; }

static void GetCopyName(u32 Version, char *Out, umm Size) { snprintf(Out, Size, "game_%u.dll", Version); }

template<typename T> static bool LoadSymbol(HMODULE Library, const char *Name, T &Out)
{
  Out = (T)(void *)GetProcAddress(Library, Name);
  if (!Out) { printf("hot reload: missing symbol %s\n", Name); }
  return Out != nullptr;
}

static bool LoadGameApi(u32 Version, game_api_table &Api)
{
  game_api_table New = {};
  if (!GetWriteTime(GAME_DLL_NAME, New.WriteTime)) { return false; }
  char CopyName[64];
  GetCopyName(Version, CopyName, sizeof(CopyName));
  if (!CopyFileA(GAME_DLL_NAME, CopyName, FALSE)) { return false; }
  New.Library = LoadLibraryA(CopyName);
  if (!New.Library) {
    DeleteFileA(CopyName);
    return false;
  }
  bool Loaded = LoadSymbol(New.Library, "game_init_window", New.InitWindow) && LoadSymbol(New.Library, "game_init", New.Init)
                && LoadSymbol(New.Library, "game_update", New.Update) && LoadSymbol(New.Library, "game_should_run", New.ShouldRun)
                && LoadSymbol(New.Library, "game_shutdown", New.Shutdown) && LoadSymbol(New.Library, "game_shutdown_window", New.ShutdownWindow)
                && LoadSymbol(New.Library, "game_memory", New.Memory) && LoadSymbol(New.Library, "game_memory_size", New.MemorySize)
                && LoadSymbol(New.Library, "game_hot_reloaded", New.HotReloaded) && LoadSymbol(New.Library, "game_force_reload", New.ForceReload)
                && LoadSymbol(New.Library, "game_force_restart", New.ForceRestart);
  if (!Loaded) {
    FreeLibrary(New.Library);
    DeleteFileA(CopyName);
    return false;
  }
  New.Version = Version;
  Api = New;
  return true;
}

static void UnloadGameApi(game_api_table &Api)
{
  if (Api.Library) { FreeLibrary(Api.Library); }
  char CopyName[64];
  GetCopyName(Api.Version, CopyName, sizeof(CopyName));
  DeleteFileA(CopyName);
  Api = {};
}

static void DeleteStaleCopies()
{
  WIN32_FIND_DATAA Found;
  HANDLE Search = FindFirstFileA("game_*.dll", &Found);
  if (Search == INVALID_HANDLE_VALUE) { return; }
  do {
    DeleteFileA(Found.cFileName);
  } while (FindNextFileA(Search, &Found));
  FindClose(Search);
}

static void MoveToExeDirectory()
{
  char Path[MAX_PATH];
  GetModuleFileNameA(nullptr, Path, MAX_PATH);
  char *Slash = strrchr(Path, '\\');
  if (Slash) { *Slash = 0; }
  SetCurrentDirectoryA(Path);
}

int main()
{
  MoveToExeDirectory();
  DeleteStaleCopies();

  u32 NextVersion = 0;
  game_api_table Api = {};
  if (!LoadGameApi(NextVersion++, Api)) {
    printf("hot reload: could not load %s\n", GAME_DLL_NAME);
    return 1;
  }
  Api.InitWindow();
  Api.Init();

  game_api_table OldApis[MAX_OLD_APIS] = {};
  u32 OldCount = 0;

  while (Api.ShouldRun()) {
    Api.Update();
    bool ForceReload = Api.ForceReload();
    bool ForceRestart = Api.ForceRestart();
    FILETIME WriteTime;
    bool Changed = GetWriteTime(GAME_DLL_NAME, WriteTime) && CompareFileTime(&WriteTime, &Api.WriteTime) != 0;
    if (!(Changed || ForceReload || ForceRestart) || IsBuildLocked()) { continue; }
    game_api_table New = {};
    if (!LoadGameApi(NextVersion, New)) { continue; }
    NextVersion++;
    bool Restart = ForceRestart || OldCount == MAX_OLD_APIS || Api.MemorySize() != New.MemorySize();
    if (Restart) {
      Api.Shutdown();
      for (u32 Index = 0; Index < OldCount; ++Index) { UnloadGameApi(OldApis[Index]); }
      OldCount = 0;
      UnloadGameApi(Api);
      Api = New;
      Api.Init();
      printf("hot reload: restarted with game_%u.dll\n", Api.Version);
    } else {
      void *Memory = Api.Memory();
      OldApis[OldCount++] = Api;
      Api = New;
      Api.HotReloaded(Memory);
      printf("hot reload: reloaded game_%u.dll\n", Api.Version);
    }
    fflush(stdout);
  }

  Api.Shutdown();
  for (u32 Index = 0; Index < OldCount; ++Index) { UnloadGameApi(OldApis[Index]); }
  Api.ShutdownWindow();
  UnloadGameApi(Api);
  return 0;
}

extern "C" __declspec(dllexport) unsigned long NvOptimusEnablement = 1;
extern "C" __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
