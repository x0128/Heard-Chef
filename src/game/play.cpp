#include "game/play.h"

#include "game/game.h"
#include "game/scene_list.h"

#include <cmath>
#include <raymath.h>

static f32 ReadStickAxis(GamepadAxis Axis) {
    f32 Value = GetGamepadAxisMovement(0, Axis);
    return fabsf(Value) < STICK_DEADZONE ? 0.f : Value;
}

static Vector2 ReadMoveDirection() {
    Vector2 Direction = {};
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
        Direction.x -= 1.f;
    }
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
        Direction.x += 1.f;
    }
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
        Direction.y -= 1.f;
    }
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
        Direction.y += 1.f;
    }
    if (IsGamepadAvailable(0)) {
        Direction.x += ReadStickAxis(GAMEPAD_AXIS_LEFT_X);
        Direction.y += ReadStickAxis(GAMEPAD_AXIS_LEFT_Y);
    }
    if (Vector2Length(Direction) > 1.f) {
        Direction = Vector2Normalize(Direction);
    }
    return Direction;
}

void EnterPlay() {
    Game->Play = {};
    Game->Play.Position = PLAY_START;
    Game->Play.PreviousPosition = PLAY_START;
}

void ExitPlay() {}

void UpdatePlay(f32 Dt) {
    play_scene& Play = Game->Play;
    f32 Half = SQUARE_SIZE * 0.5f;
    Play.PreviousPosition = Play.Position;
    if (WasPressed(Game->Input, ACTION_PAUSE)) {
        PushScene(Game->Scenes, SCENE_PAUSE);
        return;
    }
    Play.Position += ReadMoveDirection() * (PLAY_SPEED * Dt);
    Play.Position.x = Clamp(Play.Position.x, Half, UI_WIDTH - Half);
    Play.Position.y = Clamp(Play.Position.y, Half, UI_HEIGHT - Half);
}

void DrawPlay() {
    const play_scene& Play = Game->Play;
    f32 Half = SQUARE_SIZE * 0.5f;
    Vector2 Drawn = Vector2Lerp(Play.PreviousPosition, Play.Position, GetClockBlend(Game->Clock));
    DrawRectangleV({0.f, 0.f}, {UI_WIDTH, UI_HEIGHT}, PLAY_BACKGROUND);
    DrawRectangleV({Drawn.x - Half, Drawn.y - Half}, {SQUARE_SIZE, SQUARE_SIZE}, SQUARE_COLOR);
}
