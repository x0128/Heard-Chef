#pragma once

#include "core/types.h"

constexpr f32 FIXED_STEP = 1.f / 60.f;
constexpr u32 MAX_STEPS_PER_FRAME = 5;

struct fixed_clock {
    f32 Accumulator = 0.f;
};

u32 AdvanceClock(fixed_clock& Clock, f32 FrameTime);
f32 GetClockBlend(const fixed_clock& Clock);
