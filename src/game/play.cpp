#include "core/save.h"
#include "core/ui.h"
#include "game/game.h"
#include "game/scene_list.h"

#include <cmath>
#include <raymath.h>

constexpr f32 PLAY_SPEED = 300.F;
constexpr f32 SQUARE_SIZE = 40.F;
constexpr f32 HALF_SQUARE_SIZE = SQUARE_SIZE * HALF;
constexpr f32 STICK_DEADZONE = 0.2F;
constexpr Vector2 PLAY_START = { .x = UI_WIDTH * HALF, .y = UI_HEIGHT * HALF };
constexpr Color PLAY_BACKGROUND = { .r = 38, .g = 44, .b = 58, .a = 255 };
constexpr Color SQUARE_COLOR = { .r = 242, .g = 179, .b = 61, .a = 255 };

constexpr u32 PLAY_SAVE_MAGIC = 0x504c4159;
constexpr u32 PLAY_SAVE_VERSION = 1;
constexpr f32 PLAY_SAVE_SECONDS = 5.F;

struct play_save
{ Vector2 Position; };

static f32 ReadStickAxis(GamepadAxis Axis)
{
  f32 Value = GetGamepadAxisMovement(0, Axis);
  return fabsf(Value) < STICK_DEADZONE ? 0.F : Value;
}

static Vector2 ReadMoveDirection()
{
  Vector2 Direction = {};
  if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) { Direction.x -= 1.F; }
  if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) { Direction.x += 1.F; }
  if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) { Direction.y -= 1.F; }
  if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) { Direction.y += 1.F; }
  if (IsGamepadAvailable(0)) {
    Direction.x += ReadStickAxis(GAMEPAD_AXIS_LEFT_X);
    Direction.y += ReadStickAxis(GAMEPAD_AXIS_LEFT_Y);
  }
  if (Vector2Length(Direction) > 1.F) { Direction = Vector2Normalize(Direction); }
  return Direction;
}

static bool ReadPlaySave(play_save &Save) { return ReadVersionedFile(SAVE_FILE, PLAY_SAVE_MAGIC, PLAY_SAVE_VERSION, &Save, sizeof(play_save)); }

static void WritePlaySave(const play_save &Save) { WriteVersionedFile(SAVE_FILE, PLAY_SAVE_MAGIC, PLAY_SAVE_VERSION, &Save, sizeof(play_save)); }

bool HasPlaySave()
{
  play_save Scratch = {};
  return ReadPlaySave(Scratch);
}

void WriteNewPlaySave() { WritePlaySave({ PLAY_START }); }

static void SavePlay() { WritePlaySave({ Game->Play.Position }); }

static Vector2 ClampToArena(Vector2 Position)
{
  if (!std::isfinite(Position.x) || !std::isfinite(Position.y)) { return PLAY_START; }
  return { .x = Clamp(Position.x, HALF_SQUARE_SIZE, UI_WIDTH - HALF_SQUARE_SIZE), .y = Clamp(Position.y, HALF_SQUARE_SIZE, UI_HEIGHT - HALF_SQUARE_SIZE) };
}

void EnterPlay()
{
  play_save Save = { PLAY_START };
  ReadPlaySave(Save);
  Vector2 Start = ClampToArena(Save.Position);
  Game->Play = {};
  Game->Play.Position = Start;
  Game->Play.PreviousPosition = Start;
}

void ExitPlay() { SavePlay(); }

void UpdatePlay(f32 Deltatime)
{
  play_scene &Play = Game->Play;
  Play.PreviousPosition = Play.Position;
  if (WasPressed(Game->Input, ACTION_PAUSE)) {
    PushScene(Game->Scenes, SCENE_PAUSE);
    return;
  }
  Play.SaveTimer += Deltatime;
  if (Play.SaveTimer >= PLAY_SAVE_SECONDS) {
    Play.SaveTimer = 0.F;
    SavePlay();
  }
  Play.Position = ClampToArena(Play.Position + ReadMoveDirection() * (PLAY_SPEED * Deltatime));
}

void DrawPlay()
{
  const play_scene &Play = Game->Play;
  Vector2 Drawn = Vector2Lerp(Play.PreviousPosition, Play.Position, GetClockBlend(Game->Clock));
  DrawRectangleV({ .x = 0.F, .y = 0.F }, { .x = UI_WIDTH, .y = UI_HEIGHT }, PLAY_BACKGROUND);
  DrawRectangleV({ .x = Drawn.x - HALF_SQUARE_SIZE, .y = Drawn.y - HALF_SQUARE_SIZE }, { .x = SQUARE_SIZE, .y = SQUARE_SIZE }, SQUARE_COLOR);
}
