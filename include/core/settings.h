#pragma once

#include "core/types.h"

constexpr u32 SETTINGS_MAGIC = 0x53455454;
constexpr u32 SETTINGS_VERSION = 1;

struct settings
{
  f32 MasterVolume = 0.8f;
  f32 MusicVolume = 0.8f;
  f32 SfxVolume = 0.8f;
  bool Fullscreen = false;
};

settings LoadSettings(const char *Name);
bool SaveSettings(const char *Name, const settings &Settings);
void ApplySettings(const settings &Settings);
