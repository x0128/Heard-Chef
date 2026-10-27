#include "core/menu.h"

#include "core/ui.h"

#include <cmath>
#include <raymath.h>

constexpr f32 MENU_ROUNDNESS = 0.18f;
constexpr int MENU_SEGMENTS = 6;
constexpr f32 FOCUS_THICKNESS = 3.f;
constexpr f32 TRACK_HEIGHT = 12.f;
constexpr f32 TOGGLE_SIZE = 24.f;

static u32 CountBodyItems(const menu_item *Items, u32 Count)
{
  u32 Body = 0;
  for (u32 Index = 0; Index < Count; ++Index) { Body += Items[Index].Kind != ITEM_FOOTER; }
  return Body;
}

static Rectangle GetPanelRect(const menu_item *Items, u32 Count)
{
  u32 Body = CountBodyItems(Items, Count);
  f32 Height = MENU_PADDING * 2.f + MENU_TITLE_HEIGHT;
  if (Body > 0) { Height += Body * MENU_ITEM_HEIGHT + (Body - 1) * MENU_ITEM_GAP; }
  return { (UI_WIDTH - MENU_WIDTH) * 0.5f, MENU_TOP, MENU_WIDTH, Height };
}

Rectangle GetMenuItemRect(const menu_item *Items, u32 Count, u32 Index)
{
  Rectangle Panel = GetPanelRect(Items, Count);
  u32 Slot = 0;
  for (u32 Before = 0; Before < Index; ++Before) { Slot += (Items[Before].Kind == ITEM_FOOTER) == (Items[Index].Kind == ITEM_FOOTER); }
  if (Items[Index].Kind == ITEM_FOOTER) {
    return { Panel.x + Slot * (MENU_FOOTER_WIDTH + MENU_ITEM_GAP), Panel.y + Panel.height + MENU_FOOTER_GAP, MENU_FOOTER_WIDTH, MENU_FOOTER_HEIGHT };
  }
  f32 Y = Panel.y + MENU_PADDING + MENU_TITLE_HEIGHT + Slot * (MENU_ITEM_HEIGHT + MENU_ITEM_GAP);
  return { Panel.x + MENU_PADDING, Y, Panel.width - MENU_PADDING * 2.f, MENU_ITEM_HEIGHT };
}

Rectangle GetSliderTrack(Rectangle Item)
{
  f32 Width = Item.width * 0.5f - MENU_PADDING;
  return { Item.x + Item.width - MENU_PADDING - Width, Item.y + (Item.height - TRACK_HEIGHT) * 0.5f, Width, TRACK_HEIGHT };
}

static f32 SnapToStep(f32 Value) { return Clamp(roundf(Value / SLIDER_STEP) * SLIDER_STEP, 0.f, 1.f); }

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

u8 UpdateMenu(menu_state &State, const menu_item *Items, u32 Count, const frame_input &Input, u8 BackAction)
{
  if (Count == 0) { return NO_ACTION; }
  if (State.Focus > Count - 1) { State.Focus = Count - 1; }
  if (WasPressed(Input, ACTION_BACK)) { return BackAction; }

  for (u32 Index = 0; Index < Count; ++Index) {
    Rectangle Rect = GetMenuItemRect(Items, Count, Index);
    if (!CheckCollisionPointRec(Input.Mouse, Rect)) { continue; }
    if (Input.MouseMoved || Input.MouseClicked) { State.Focus = Index; }
    if (Input.MouseClicked) { return ClickItem(Items[Index], Rect, Input.Mouse); }
  }

  if (WasPressed(Input, ACTION_UP)) { State.Focus = State.Focus == 0 ? Count - 1 : State.Focus - 1; }
  if (WasPressed(Input, ACTION_DOWN)) { State.Focus = State.Focus == Count - 1 ? 0 : State.Focus + 1; }

  const menu_item &Focused = Items[State.Focus];
  bool Left = WasPressed(Input, ACTION_LEFT);
  bool Right = WasPressed(Input, ACTION_RIGHT);
  if (Left || Right) {
    if (Focused.Kind == ITEM_SLIDER) {
      *Focused.Slider = SnapToStep(*Focused.Slider + (Right ? SLIDER_STEP : -SLIDER_STEP));
      return Focused.Action;
    }
    if (Focused.Kind == ITEM_TOGGLE) {
      *Focused.Toggle = !*Focused.Toggle;
      return Focused.Action;
    }
  }

  if (WasPressed(Input, ACTION_CONFIRM)) {
    if (Focused.Kind == ITEM_TOGGLE) {
      *Focused.Toggle = !*Focused.Toggle;
      return Focused.Action;
    }
    if (Focused.Kind != ITEM_SLIDER) { return Focused.Action; }
  }
  return NO_ACTION;
}

static void DrawLabel(const char *Label, Rectangle Rect, bool Centered, Color Tint)
{
  int Width = MeasureText(Label, MENU_FONT_SIZE);
  f32 X = Centered ? Rect.x + (Rect.width - Width) * 0.5f : Rect.x + MENU_PADDING;
  DrawText(Label, (int)X, (int)(Rect.y + (Rect.height - MENU_FONT_SIZE) * 0.5f), MENU_FONT_SIZE, Tint);
}

static void DrawItem(const menu_item &Item, Rectangle Rect, const menu_theme &Theme)
{
  switch (Item.Kind) {
  case ITEM_PRIMARY:
    DrawRectangleRounded(Rect, MENU_ROUNDNESS, MENU_SEGMENTS, Theme.Primary);
    DrawLabel(Item.Label, Rect, true, Theme.PrimaryText);
    break;
  case ITEM_FOOTER:
    DrawRectangleRounded(Rect, MENU_ROUNDNESS, MENU_SEGMENTS, Theme.Footer);
    DrawLabel(Item.Label, Rect, true, Theme.FooterText);
    break;
  case ITEM_SLIDER: {
    DrawRectangleRounded(Rect, MENU_ROUNDNESS, MENU_SEGMENTS, Theme.Button);
    DrawLabel(Item.Label, Rect, false, Theme.ButtonText);
    Rectangle Track = GetSliderTrack(Rect);
    Rectangle Filled = { Track.x, Track.y, Track.width * *Item.Slider, Track.height };
    DrawRectangleRounded(Track, 0.5f, MENU_SEGMENTS, Theme.Track);
    DrawRectangleRounded(Filled, 0.5f, MENU_SEGMENTS, Theme.Fill);
    break;
  }
  case ITEM_TOGGLE: {
    DrawRectangleRounded(Rect, MENU_ROUNDNESS, MENU_SEGMENTS, Theme.Button);
    DrawLabel(Item.Label, Rect, false, Theme.ButtonText);
    Rectangle Box = { Rect.x + Rect.width - MENU_PADDING - TOGGLE_SIZE, Rect.y + (Rect.height - TOGGLE_SIZE) * 0.5f, TOGGLE_SIZE, TOGGLE_SIZE };
    DrawRectangleRec(Box, Theme.Track);
    if (*Item.Toggle) { DrawRectangleRec({ Box.x + 5.f, Box.y + 5.f, Box.width - 10.f, Box.height - 10.f }, Theme.Fill); }
    break;
  }
  case ITEM_BUTTON:
    DrawRectangleRounded(Rect, MENU_ROUNDNESS, MENU_SEGMENTS, Theme.Button);
    DrawLabel(Item.Label, Rect, true, Theme.ButtonText);
    break;
  }
}

void DrawMenu(const menu_state &State, const char *Title, const menu_item *Items, u32 Count, const menu_theme &Theme)
{
  Rectangle Panel = GetPanelRect(Items, Count);
  DrawRectangleRounded(Panel, 0.06f, MENU_SEGMENTS, Theme.Panel);
  DrawRectangleRoundedLinesEx(Panel, 0.06f, MENU_SEGMENTS, 2.f, Theme.Edge);
  f32 TitleY = Panel.y + MENU_PADDING + (MENU_TITLE_HEIGHT - MENU_TITLE_FONT_SIZE) * 0.5f - MENU_ITEM_GAP * 0.5f;
  DrawText(Title, (int)(Panel.x + MENU_PADDING), (int)TitleY, MENU_TITLE_FONT_SIZE, Theme.Title);
  for (u32 Index = 0; Index < Count; ++Index) {
    Rectangle Rect = GetMenuItemRect(Items, Count, Index);
    DrawItem(Items[Index], Rect, Theme);
    if (Index == State.Focus) { DrawRectangleRoundedLinesEx(Rect, MENU_ROUNDNESS, MENU_SEGMENTS, FOCUS_THICKNESS, Theme.Focus); }
  }
}
