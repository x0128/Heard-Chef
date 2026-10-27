#include "core/menu.h"

#include "core/ui.h"

#include <cmath>
#include <raymath.h>

constexpr f32 MENU_ROUNDNESS = 0.18F;
constexpr int MENU_SEGMENTS = 6;
constexpr f32 FOCUS_THICKNESS = 3.F;
constexpr f32 TRACK_HEIGHT = 12.F;
constexpr f32 TOGGLE_SIZE = 24.F;
constexpr f32 TOGGLE_FILL_INSET = 5.F;
constexpr f32 TRACK_ROUNDNESS = 0.5F;
constexpr f32 PANEL_ROUNDNESS = 0.06F;
constexpr f32 PANEL_EDGE_THICKNESS = 2.F;

static Rectangle GetPanelRect(std::span<const menu_item> Items)
{
  const u32 Count = static_cast<u32>(Items.size());
  f32 Height = MENU_PADDING * 2 + MENU_TITLE_HEIGHT;
  if (Count > 0) { Height += static_cast<f32>(Count) * MENU_ITEM_HEIGHT + static_cast<f32>(Count - 1) * MENU_ITEM_GAP; }
  return { .x = (UI_WIDTH - MENU_WIDTH) * HALF, .y = MENU_TOP, .width = MENU_WIDTH, .height = Height };
}

Rectangle GetMenuItemRect(std::span<const menu_item> Items, u32 Index)
{
  Rectangle Panel = GetPanelRect(Items);
  f32 PosY = Panel.y + MENU_PADDING + MENU_TITLE_HEIGHT + static_cast<f32>(Index) * (MENU_ITEM_HEIGHT + MENU_ITEM_GAP);
  return { .x = Panel.x + MENU_PADDING, .y = PosY, .width = Panel.width - MENU_PADDING * 2, .height = MENU_ITEM_HEIGHT };
}

Rectangle GetSliderTrack(Rectangle Item)
{
  f32 Width = Item.width * HALF - MENU_PADDING;
  return { .x = Item.x + Item.width - MENU_PADDING - Width, .y = Item.y + (Item.height - TRACK_HEIGHT) * HALF, .width = Width, .height = TRACK_HEIGHT };
}

static f32 SnapToStep(f32 Value) { return Clamp(roundf(Value / SLIDER_STEP) * SLIDER_STEP, 0.F, 1.F); }

static u8 ClickItem(const menu_item &Item, Rectangle Rect, Vector2 Mouse)
{
  switch (Item.Kind) {
  case ITEM_SLIDER: {
    Rectangle Track = GetSliderTrack(Rect);
    if (Mouse.x < Track.x || Mouse.x > Track.x + Track.width) { return NO_ACTION; }
    *Item.Slider = SnapToStep((Mouse.x - Track.x) / Track.width);
    return Item.Action;
  }
  case ITEM_TOGGLE:
    *Item.Toggle = !*Item.Toggle;
    return Item.Action;
  default:
    return Item.Action;
  }
}

static u8 PressItem(const menu_item &Item, const frame_input &Input)
{
  const bool Left = WasPressed(Input, ACTION_LEFT);
  const bool Right = WasPressed(Input, ACTION_RIGHT);
  if (Left || Right) {
    if (Item.Kind == ITEM_SLIDER) {
      *Item.Slider = SnapToStep(*Item.Slider + (Right ? SLIDER_STEP : -SLIDER_STEP));
      return Item.Action;
    }
    if (Item.Kind == ITEM_TOGGLE) {
      *Item.Toggle = !*Item.Toggle;
      return Item.Action;
    }
  }

  if (WasPressed(Input, ACTION_CONFIRM)) {
    if (Item.Kind == ITEM_TOGGLE) {
      *Item.Toggle = !*Item.Toggle;
      return Item.Action;
    }
    if (Item.Kind != ITEM_SLIDER) { return Item.Action; }
  }
  return NO_ACTION;
}

u8 UpdateMenu(menu_state &State, std::span<const menu_item> Items, const frame_input &Input, u8 BackAction)
{
  if (Items.empty()) { return NO_ACTION; }
  const u32 Count = static_cast<u32>(Items.size());
  if (State.Focus > Count - 1) { State.Focus = Count - 1; }
  if (WasPressed(Input, ACTION_BACK)) { return BackAction; }

  for (u32 Index = 0; Index < Count; ++Index) {
    Rectangle Rect = GetMenuItemRect(Items, Index);
    if (!CheckCollisionPointRec(Input.Mouse, Rect)) { continue; }
    if (Input.MouseMoved || Input.MouseClicked) { State.Focus = Index; }
    if (Input.MouseClicked) { return ClickItem(Items[Index], Rect, Input.Mouse); }
  }

  if (WasPressed(Input, ACTION_UP)) { State.Focus = State.Focus == 0 ? Count - 1 : State.Focus - 1; }
  if (WasPressed(Input, ACTION_DOWN)) { State.Focus = State.Focus == Count - 1 ? 0 : State.Focus + 1; }

  return PressItem(Items[State.Focus], Input);
}

static void DrawLabel(const char *Label, Rectangle Rect, bool Centered, Color Tint)
{
  int Width = MeasureText(Label, MENU_FONT_SIZE);
  f32 XPos = Centered ? Rect.x + (Rect.width - static_cast<f32>(Width)) * HALF : Rect.x + MENU_PADDING;
  DrawText(Label, static_cast<int>(XPos), static_cast<int>(Rect.y + (Rect.height - MENU_FONT_SIZE) * HALF), MENU_FONT_SIZE, Tint);
}

static void DrawSliderTrack(Rectangle Rect, f32 Value, const menu_theme &Theme)
{
  const Rectangle Track = GetSliderTrack(Rect);
  const Rectangle Filled = { .x = Track.x, .y = Track.y, .width = Track.width * Value, .height = Track.height };
  DrawRectangleRounded(Track, TRACK_ROUNDNESS, MENU_SEGMENTS, Theme.Track);
  DrawRectangleRounded(Filled, TRACK_ROUNDNESS, MENU_SEGMENTS, Theme.Fill);
}

static void DrawToggleBox(Rectangle Rect, bool Checked, const menu_theme &Theme)
{
  const Rectangle Box = {
    .x = Rect.x + Rect.width - MENU_PADDING - TOGGLE_SIZE, .y = Rect.y + (Rect.height - TOGGLE_SIZE) * HALF, .width = TOGGLE_SIZE, .height = TOGGLE_SIZE
  };
  DrawRectangleRec(Box, Theme.Track);
  if (Checked) {
    const Rectangle Rec = {
      .x = Box.x + TOGGLE_FILL_INSET, .y = Box.y + TOGGLE_FILL_INSET, .width = Box.width - TOGGLE_FILL_INSET * 2, .height = Box.height - TOGGLE_FILL_INSET * 2
    };
    DrawRectangleRec(Rec, Theme.Fill);
  }
}

static void DrawItem(const menu_item &Item, Rectangle Rect, const menu_theme &Theme)
{
  const bool Primary = Item.Kind == ITEM_PRIMARY;
  const bool HasControl = Item.Kind == ITEM_SLIDER || Item.Kind == ITEM_TOGGLE;
  DrawRectangleRounded(Rect, MENU_ROUNDNESS, MENU_SEGMENTS, Primary ? Theme.Primary : Theme.Button);
  DrawLabel(Item.Label, Rect, !HasControl, Primary ? Theme.PrimaryText : Theme.ButtonText);
  if (Item.Kind == ITEM_SLIDER) { DrawSliderTrack(Rect, *Item.Slider, Theme); }
  if (Item.Kind == ITEM_TOGGLE) { DrawToggleBox(Rect, *Item.Toggle, Theme); }
}

void DrawMenu(const menu_state &State, const char *Title, std::span<const menu_item> Items, const menu_theme &Theme)
{
  Rectangle Panel = GetPanelRect(Items);
  DrawRectangleRounded(Panel, PANEL_ROUNDNESS, MENU_SEGMENTS, Theme.Panel);
  DrawRectangleRoundedLinesEx(Panel, PANEL_ROUNDNESS, MENU_SEGMENTS, PANEL_EDGE_THICKNESS, Theme.Edge);
  f32 TitleY = Panel.y + MENU_PADDING + (MENU_TITLE_HEIGHT - MENU_TITLE_FONT_SIZE) * HALF - MENU_ITEM_GAP * HALF;
  DrawText(Title, static_cast<int>(Panel.x + MENU_PADDING), static_cast<int>(TitleY), MENU_TITLE_FONT_SIZE, Theme.Title);
  for (u32 Index = 0; Index < Items.size(); ++Index) {
    Rectangle Rect = GetMenuItemRect(Items, Index);
    DrawItem(Items[Index], Rect, Theme);
    if (Index == State.Focus) { DrawRectangleRoundedLinesEx(Rect, MENU_ROUNDNESS, MENU_SEGMENTS, FOCUS_THICKNESS, Theme.Focus); }
  }
}
