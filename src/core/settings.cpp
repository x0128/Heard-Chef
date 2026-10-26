#include "core/settings.h"

#include "core/save.h"

#include <raylib.h>

settings LoadSettings(const char* Name) {
    settings Loaded = {};
    if (!ReadVersionedFile(Name, SETTINGS_MAGIC, SETTINGS_VERSION, &Loaded, sizeof(settings))) {
        return settings{};
    }
    return Loaded;
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
