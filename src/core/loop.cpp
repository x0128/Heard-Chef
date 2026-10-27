#include "core/loop.h"
#include "core/config.h"

constexpr u32 MAX_STEPS_PER_FRAME = 5;

u32 AdvanceClock(fixed_clock &Clock, f32 FrameTime)
{
  Clock.Accumulator += FrameTime;
  u32 Steps = 0;
  while (Clock.Accumulator >= FIXED_STEP && Steps < MAX_STEPS_PER_FRAME) {
    Clock.Accumulator -= FIXED_STEP;
    ++Steps;
  }
  if (Steps == MAX_STEPS_PER_FRAME) { Clock.Accumulator = 0.F; }
  return Steps;
}

f32 GetClockBlend(const fixed_clock &Clock) { return Clock.Accumulator / FIXED_STEP; }
