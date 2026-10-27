#pragma once

#include "core/types.h"

struct fixed_clock
{ f32 Accumulator = 0.F; };

u32 AdvanceClock(fixed_clock &Clock, f32 FrameTime);
f32 GetClockBlend(const fixed_clock &Clock);
