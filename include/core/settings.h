#pragma once

#include "core/types.h"

constexpr u32 SETTINGS_MAGIC = 0x53455454;
constexpr u32 SETTINGS_VERSION = 1;

constexpr f32 DEFAULT_AUDIO_VOLUME = 0.85F;

struct settings
{
  f32 MasterVolume = DEFAULT_AUDIO_VOLUME;
  f32 MusicVolume = DEFAULT_AUDIO_VOLUME;
  f32 SfxVolume = DEFAULT_AUDIO_VOLUME;
  bool Fullscreen = false;
};

settings LoadSettings(const char *Name);
bool SaveSettings(const char *Name, const settings &Settings);
void ApplySettings(const settings &Settings);
