#include "core/game_api.h"
#include "core/types.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <array>
#include <cstdio>
#include <cstring>
#include <format>
#include <iostream>
#include <string>
#include <windows.h>

constexpr const char *GAME_DLL_NAME = "game.dll";
constexpr const char *GAME_LOCK_NAME = "game.lock";
constexpr u32 MAX_OLD_APIS = 64;

struct game_api_table
{
  HMODULE Library;
  decltype(&GameInitWindow) InitWindow;
  decltype(&GameInit) Init;
  decltype(&GameUpdate) Update;
  decltype(&GameShouldRun) ShouldRun;
  decltype(&GameShutdown) Shutdown;
  decltype(&GameShutdownWindow) ShutdownWindow;
  decltype(&GameMemory) Memory;
  decltype(&GameMemorySize) MemorySize;
  decltype(&GameHotReloaded) HotReloaded;
  decltype(&GameForceReload) ForceReload;
  decltype(&GameForceRestart) ForceRestart;
  FILETIME WriteTime;
  u32 Version;
};

static bool GetWriteTime(const char *Path, FILETIME &Time)
{
  WIN32_FILE_ATTRIBUTE_DATA Data;
  if (GetFileAttributesExA(Path, GetFileExInfoStandard, &Data) == FALSE) { return false; }
  Time = Data.ftLastWriteTime;
  return true;
}

static bool IsBuildLocked() { return GetFileAttributesA(GAME_LOCK_NAME) != INVALID_FILE_ATTRIBUTES; }

static std::string GetCopyName(u32 Version) { return std::format("game_{}.dll", Version); }

template<typename T> static bool LoadSymbol(HMODULE Library, const char *Name, T &Out)
{
  Out = reinterpret_cast<T>(GetProcAddress(Library, Name));
  if (Out == nullptr) { std::cout << "hot reload: missing symbol " << Name << '\n'; }
  return Out != nullptr;
}

static bool LoadGameApi(u32 Version, game_api_table &Api)
{
  game_api_table New = {};
  if (!GetWriteTime(GAME_DLL_NAME, New.WriteTime)) { return false; }
  const std::string CopyName = GetCopyName(Version);
  if (CopyFileA(GAME_DLL_NAME, CopyName.c_str(), FALSE) == FALSE) { return false; }
  New.Library = LoadLibraryA(CopyName.c_str());
  if (New.Library == nullptr) {
    DeleteFileA(CopyName.c_str());
    return false;
  }
  const bool Loaded = LoadSymbol(New.Library, "GameInitWindow", New.InitWindow) && LoadSymbol(New.Library, "GameInit", New.Init)
                      && LoadSymbol(New.Library, "GameUpdate", New.Update) && LoadSymbol(New.Library, "GameShouldRun", New.ShouldRun)
                      && LoadSymbol(New.Library, "GameShutdown", New.Shutdown) && LoadSymbol(New.Library, "GameShutdownWindow", New.ShutdownWindow)
                      && LoadSymbol(New.Library, "GameMemory", New.Memory) && LoadSymbol(New.Library, "GameMemorySize", New.MemorySize)
                      && LoadSymbol(New.Library, "GameHotReloaded", New.HotReloaded) && LoadSymbol(New.Library, "GameForceReload", New.ForceReload)
                      && LoadSymbol(New.Library, "GameForceRestart", New.ForceRestart);
  if (!Loaded) {
    FreeLibrary(New.Library);
    DeleteFileA(CopyName.c_str());
    return false;
  }
  New.Version = Version;
  Api = New;
  return true;
}

static void UnloadGameApi(game_api_table &Api)
{
  if (Api.Library != nullptr) { FreeLibrary(Api.Library); }
  DeleteFileA(GetCopyName(Api.Version).c_str());
  Api = {};
}

static void DeleteStaleCopies()
{
  WIN32_FIND_DATAA Found;
  HANDLE Search = FindFirstFileA("game_*.dll", &Found);
  if (Search == INVALID_HANDLE_VALUE) { return; }
  bool More = true;
  while (More) {
    DeleteFileA(&Found.cFileName[0]);
    More = FindNextFileA(Search, &Found) != FALSE;
  }
  FindClose(Search);
}

static void MoveToExeDirectory()
{
  std::array<char, MAX_PATH> Path = {};
  GetModuleFileNameA(nullptr, Path.data(), MAX_PATH);
  char *Slash = strrchr(Path.data(), '\\');
  if (Slash != nullptr) { *Slash = 0; }
  SetCurrentDirectoryA(Path.data());
}

int main()
try {
  MoveToExeDirectory();
  DeleteStaleCopies();

  u32 NextVersion = 0;
  game_api_table Api = {};
  if (!LoadGameApi(NextVersion++, Api)) {
    std::cout << "hot reload: could not load " << GAME_DLL_NAME << '\n';
    return 1;
  }
  Api.InitWindow();
  Api.Init();

  std::array<game_api_table, MAX_OLD_APIS> OldApis = {};
  u32 OldCount = 0;

  while (Api.ShouldRun()) {
    Api.Update();
    const bool ForceReload = Api.ForceReload();
    const bool ForceRestart = Api.ForceRestart();
    FILETIME WriteTime;
    const bool Changed = GetWriteTime(GAME_DLL_NAME, WriteTime) && CompareFileTime(&WriteTime, &Api.WriteTime) != 0;
    if (!(Changed || ForceReload || ForceRestart) || IsBuildLocked()) { continue; }
    game_api_table New = {};
    if (!LoadGameApi(NextVersion, New)) { continue; }
    NextVersion++;
    const bool Restart = ForceRestart || OldCount == MAX_OLD_APIS || Api.MemorySize() != New.MemorySize();
    if (Restart) {
      Api.Shutdown();
      for (u32 Index = 0; Index < OldCount; ++Index) { UnloadGameApi(OldApis.at(Index)); }
      OldCount = 0;
      UnloadGameApi(Api);
      Api = New;
      Api.Init();
      std::cout << "hot reload: restarted with " << GetCopyName(Api.Version) << '\n';
    } else {
      void *Memory = Api.Memory();
      OldApis.at(OldCount++) = Api;
      Api = New;
      Api.HotReloaded(Memory);
      std::cout << "hot reload: reloaded " << GetCopyName(Api.Version) << '\n';
    }
    std::cout.flush();
  }

  Api.Shutdown();
  for (u32 Index = 0; Index < OldCount; ++Index) { UnloadGameApi(OldApis.at(Index)); }
  Api.ShutdownWindow();
  UnloadGameApi(Api);
  return 0;
} catch (...) {
  static_cast<void>(std::fputs("hot reload: unexpected error\n", stderr));
  return 1;
}

extern "C" __declspec(dllexport) unsigned long NvOptimusEnablement = 1;
extern "C" __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
