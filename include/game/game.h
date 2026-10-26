#pragma once

#include "core/input.h"
#include "core/loop.h"
#include "core/types.h"
#include "game/play.h"

constexpr const char* GAME_TITLE = "GAME TITLE";
constexpr const char* SAVE_FILE = "game_save.bin";
constexpr const char* SETTINGS_FILE = "game_settings.bin";

struct game_state {
    bool Run = true;
    fixed_clock Clock;
    stick_state Stick;
    frame_input PendingInput;
    frame_input Input;
    play_scene Play;
};

extern game_state* Game;
