#pragma once

#include "types.h"

struct rng
{ u32 State = 1; };

constexpr u32 NextRng(rng &Rng)
{
  Rng.State ^= Rng.State << 13U;
  Rng.State ^= Rng.State >> 17U;
  Rng.State ^= Rng.State << 5U;
  return Rng.State;
}
static_assert([] {
  rng Rng{ .State = 1 };
  return NextRng(Rng);
}() == 270369U);
static_assert([] {
  rng Rng{ .State = 1 };
  NextRng(Rng);
  return NextRng(Rng);
}() == 67634689U);

constexpr f32 RngFloat(rng &Rng) { return static_cast<f32>(NextRng(Rng) >> 8U) * (1.F / 16777216.F); }
static_assert([] {
  rng Rng{ .State = 1 };
  return RngFloat(Rng);
}() == 1056.F / 16777216.F);
