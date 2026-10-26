#include "game/play.h"

#include "core/save.h"
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

static bool ReadPlaySave(play_save& Save) {
    return ReadVersionedFile(SAVE_FILE, PLAY_SAVE_MAGIC, PLAY_SAVE_VERSION, &Save, sizeof(play_save));
}

static void WritePlaySave(const play_save& Save) {
    WriteVersionedFile(SAVE_FILE, PLAY_SAVE_MAGIC, PLAY_SAVE_VERSION, &Save, sizeof(play_save));
}

bool HasPlaySave() {
    play_save Scratch = {};
    return ReadPlaySave(Scratch);
}

void WriteNewPlaySave() {
    WritePlaySave({PLAY_START});
}

void SavePlay() {
    WritePlaySave({Game->Play.Position});
}

static Vector2 ClampToArena(Vector2 Position) {
    if (!std::isfinite(Position.x) || !std::isfinite(Position.y)) {
        return PLAY_START;
    }
    f32 Half = SQUARE_SIZE * 0.5f;
    return {Clamp(Position.x, Half, UI_WIDTH - Half), Clamp(Position.y, Half, UI_HEIGHT - Half)};
}

void EnterPlay() {
    play_save Save = {PLAY_START};
    ReadPlaySave(Save);
    Vector2 Start = ClampToArena(Save.Position);
    Game->Play = {};
    Game->Play.Position = Start;
    Game->Play.PreviousPosition = Start;
}

void ExitPlay() {
    SavePlay();
}

void UpdatePlay(f32 Dt) {
    play_scene& Play = Game->Play;
    Play.PreviousPosition = Play.Position;
    if (WasPressed(Game->Input, ACTION_PAUSE)) {
        PushScene(Game->Scenes, SCENE_PAUSE);
        return;
    }
    Play.SaveTimer += Dt;
    if (Play.SaveTimer >= PLAY_SAVE_SECONDS) {
        Play.SaveTimer = 0.f;
        SavePlay();
    }
    Play.Position = ClampToArena(Play.Position + ReadMoveDirection() * (PLAY_SPEED * Dt));
}

void DrawPlay() {
    const play_scene& Play = Game->Play;
    f32 Half = SQUARE_SIZE * 0.5f;
    Vector2 Drawn = Vector2Lerp(Play.PreviousPosition, Play.Position, GetClockBlend(Game->Clock));
    DrawRectangleV({0.f, 0.f}, {UI_WIDTH, UI_HEIGHT}, PLAY_BACKGROUND);
    DrawRectangleV({Drawn.x - Half, Drawn.y - Half}, {SQUARE_SIZE, SQUARE_SIZE}, SQUARE_COLOR);
}
