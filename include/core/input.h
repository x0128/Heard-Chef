#pragma once

#include "core/types.h"

#include <raylib.h>

enum input_action : u8 {
  ACTION_UP,
  ACTION_DOWN,
  ACTION_LEFT,
  ACTION_RIGHT,
  ACTION_CONFIRM,
  ACTION_BACK,
  ACTION_PAUSE,
  ACTION_COUNT,
};

struct frame_input
{
  bool Pressed[ACTION_COUNT] = {};
  bool AnyPressed = false;
  Vector2 Mouse = {};
  bool MouseMoved = false;
  bool MouseClicked = false;
};

struct stick_state
{
  s8 X = 0;
  s8 Y = 0;
};

constexpr f32 STICK_THRESHOLD = 0.5f;

frame_input ReadFrameInput(stick_state &Stick, const Camera2D &UiCamera);
void MergeInput(frame_input &Pending, const frame_input &Frame);
frame_input TakeInput(frame_input &Pending);
bool WasPressed(const frame_input &Input, input_action Action);
