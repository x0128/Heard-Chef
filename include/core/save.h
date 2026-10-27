#pragma once

#include "core/types.h"

struct file_header
{
  u32 Magic;
  u32 Version;
  u32 Size;
};

bool IsValidHeader(const file_header &Header, u32 Magic, u32 Version, u32 Size, u32 FileBytes);
bool WriteVersionedFile(const char *Name, u32 Magic, u32 Version, const void *Data, u32 Size);
bool ReadVersionedFile(const char *Name, u32 Magic, u32 Version, void *Data, u32 Size);
