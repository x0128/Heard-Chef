#include "core/settings.h"

#include "core/save.h"

#include <cmath>
#include <cstring>
#include <raylib.h>

static f32 CleanVolume(f32 Value, f32 Default) {
    if (!std::isfinite(Value)) {
        return Default;
    }
    return fminf(fmaxf(Value, 0.f), 1.f);
}

static bool CleanBool(const bool& Value) {
    u8 Raw = 0;
    memcpy(&Raw, &Value, 1);
    return Raw != 0;
}

settings LoadSettings(const char* Name) {
    settings Loaded = {};
    if (!ReadVersionedFile(Name, SETTINGS_MAGIC, SETTINGS_VERSION, &Loaded, sizeof(settings))) {
        return settings{};
    }
    settings Clean = {};
    Clean.MasterVolume = CleanVolume(Loaded.MasterVolume, Clean.MasterVolume);
    Clean.MusicVolume = CleanVolume(Loaded.MusicVolume, Clean.MusicVolume);
    Clean.SfxVolume = CleanVolume(Loaded.SfxVolume, Clean.SfxVolume);
    Clean.Fullscreen = CleanBool(Loaded.Fullscreen);
    return Clean;
}

bool SaveSettings(const char* Name, const settings& Settings) {
    return WriteVersionedFile(Name, SETTINGS_MAGIC, SETTINGS_VERSION, &Settings, sizeof(settings));
}

void ApplySettings(const settings& Settings) {
    SetMasterVolume(Settings.MasterVolume);
#if !defined(__EMSCRIPTEN__)
    if (IsWindowState(FLAG_BORDERLESS_WINDOWED_MODE) != Settings.Fullscreen) {
        ToggleBorderlessWindowed();
    }
#endif
}
