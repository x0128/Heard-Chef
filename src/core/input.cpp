#include "core/input.h"

static bool IsKeyTapped(KeyboardKey Key) { return IsKeyPressed(Key) || IsKeyPressedRepeat(Key); }

static s8 GetStickDirection(f32 Value)
{
  if (Value <= -STICK_THRESHOLD) { return -1; }
  if (Value >= STICK_THRESHOLD) { return 1; }
  return 0;
}

static bool IsAnyGamepadButtonPressed()
{
  for (int Button = GAMEPAD_BUTTON_LEFT_FACE_UP; Button <= GAMEPAD_BUTTON_RIGHT_THUMB; ++Button) {
    if (IsGamepadButtonPressed(0, Button)) { return true; }
  }
  return false;
}

frame_input ReadFrameInput(stick_state &Stick, const Camera2D &UiCamera)
{
  frame_input Input = {};
  Input.Pressed[ACTION_UP] = IsKeyTapped(KEY_UP) || IsKeyTapped(KEY_W);
  Input.Pressed[ACTION_DOWN] = IsKeyTapped(KEY_DOWN) || IsKeyTapped(KEY_S);
  Input.Pressed[ACTION_LEFT] = IsKeyTapped(KEY_LEFT) || IsKeyTapped(KEY_A);
  Input.Pressed[ACTION_RIGHT] = IsKeyTapped(KEY_RIGHT) || IsKeyTapped(KEY_D);
  Input.Pressed[ACTION_CONFIRM] = IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE);
  Input.Pressed[ACTION_BACK] = IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_BACKSPACE);
  Input.Pressed[ACTION_PAUSE] = IsKeyPressed(KEY_ESCAPE);

  while (GetKeyPressed() != 0) { Input.AnyPressed = true; }

  if (IsGamepadAvailable(0)) {
    Input.Pressed[ACTION_UP] |= IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_UP);
    Input.Pressed[ACTION_DOWN] |= IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN);
    Input.Pressed[ACTION_LEFT] |= IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT);
    Input.Pressed[ACTION_RIGHT] |= IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT);
    Input.Pressed[ACTION_CONFIRM] |= IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN);
    Input.Pressed[ACTION_BACK] |= IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_MIDDLE_RIGHT);
    Input.Pressed[ACTION_PAUSE] |= IsGamepadButtonPressed(0, GAMEPAD_BUTTON_MIDDLE_RIGHT);

    s8 X = GetStickDirection(GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X));
    s8 Y = GetStickDirection(GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y));
    if (Y != Stick.Y) {
      Input.Pressed[ACTION_UP] |= Y < 0;
      Input.Pressed[ACTION_DOWN] |= Y > 0;
    }
    if (X != Stick.X) {
      Input.Pressed[ACTION_LEFT] |= X < 0;
      Input.Pressed[ACTION_RIGHT] |= X > 0;
    }
    Stick = { X, Y };
    Input.AnyPressed |= IsAnyGamepadButtonPressed();
  } else {
    Stick = {};
  }

  Input.MouseClicked = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
  Input.AnyPressed |= Input.MouseClicked;
  Input.Mouse = GetScreenToWorld2D(GetMousePosition(), UiCamera);
  Vector2 Delta = GetMouseDelta();
  Input.MouseMoved = Delta.x != 0.f || Delta.y != 0.f;
  return Input;
}

void MergeInput(frame_input &Pending, const frame_input &Frame)
{
  for (u32 Action = 0; Action < ACTION_COUNT; ++Action) { Pending.Pressed[Action] |= Frame.Pressed[Action]; }
  Pending.AnyPressed |= Frame.AnyPressed;
  Pending.MouseMoved |= Frame.MouseMoved;
  Pending.MouseClicked |= Frame.MouseClicked;
  Pending.Mouse = Frame.Mouse;
}

frame_input TakeInput(frame_input &Pending)
{
  frame_input Taken = Pending;
  Pending = {};
  Pending.Mouse = Taken.Mouse;
  return Taken;
}

bool WasPressed(const frame_input &Input, input_action Action) { return Input.Pressed[Action]; }
