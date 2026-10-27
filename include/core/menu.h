#pragma once

#include "core/input.h"
#include "core/types.h"

#include <raylib.h>

constexpr u8 NO_ACTION = 0xff;
constexpr u32 MAX_MENU_ITEMS = 8;
constexpr f32 SLIDER_STEP = 0.1F;
constexpr f32 OVERLAY_DIM = 0.55F;
constexpr f32 SOLID_OVERLAY_DIM = 0.92F;

constexpr f32 MENU_WIDTH = 440.F;
constexpr f32 MENU_TOP = 140.F;
constexpr f32 MENU_PADDING = 16.F;
constexpr f32 MENU_TITLE_HEIGHT = 40.F;
constexpr f32 MENU_ITEM_HEIGHT = 56.F;
constexpr f32 MENU_ITEM_GAP = 8.F;
constexpr f32 MENU_FOOTER_GAP = 20.F;
constexpr f32 MENU_FOOTER_WIDTH = 180.F;
constexpr f32 MENU_FOOTER_HEIGHT = 48.F;
constexpr int MENU_TITLE_FONT_SIZE = 28;
constexpr int MENU_FONT_SIZE = 24;

enum menu_item_kind : u8 {
  ITEM_BUTTON,
  ITEM_PRIMARY,
  ITEM_FOOTER,
  ITEM_SLIDER,
  ITEM_TOGGLE,
};

struct menu_item
{
  const char *Label = "";
  u8 Action = NO_ACTION;
  menu_item_kind Kind = ITEM_BUTTON;
  f32 *Slider = nullptr;
  bool *Toggle = nullptr;
};

struct menu_state
{ u32 Focus = 0; };

struct menu_theme
{
  Color Panel;
  Color Edge;
  Color Title;
  Color Button;
  Color ButtonText;
  Color Primary;
  Color PrimaryText;
  Color Footer;
  Color FooterText;
  Color Focus;
  Color Track;
  Color Fill;
};

constexpr menu_theme DEFAULT_MENU_THEME = {
  .Panel = { .r = 29, .g = 32, .b = 41, .a = 240 },
  .Edge = { .r = 58, .g = 63, .b = 78, .a = 255 },
  .Title = { .r = 150, .g = 156, .b = 170, .a = 255 },
  .Button = { .r = 44, .g = 48, .b = 61, .a = 255 },
  .ButtonText = { .r = 226, .g = 222, .b = 210, .a = 255 },
  .Primary = { .r = 242, .g = 179, .b = 61, .a = 255 },
  .PrimaryText = { .r = 32, .g = 24, .b = 12, .a = 255 },
  .Footer = { .r = 92, .g = 46, .b = 40, .a = 255 },
  .FooterText = { .r = 240, .g = 200, .b = 190, .a = 255 },
  .Focus = { .r = 255, .g = 230, .b = 192, .a = 255 },
  .Track = { .r = 24, .g = 26, .b = 34, .a = 255 },
  .Fill = { .r = 242, .g = 179, .b = 61, .a = 255 },
};

Rectangle GetMenuItemRect(const menu_item *Items, u32 Count, u32 Index);
Rectangle GetSliderTrack(Rectangle Item);
u8 UpdateMenu(menu_state &State, const menu_item *Items, u32 Count, const frame_input &Input, u8 BackAction);
void DrawMenu(const menu_state &State, const char *Title, const menu_item *Items, u32 Count, const menu_theme &Theme);
