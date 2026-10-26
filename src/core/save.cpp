#include "core/save.h"

#include <cstring>
#include <raylib.h>

static const char* GetSavePath(const char* Name) {
    return TextFormat("%s%s", GetApplicationDirectory(), Name);
}

bool IsValidHeader(const file_header& Header, u32 Magic, u32 Version, u32 Size, u32 FileBytes) {
    return Header.Magic == Magic && Header.Version == Version && Header.Size == Size && FileBytes == sizeof(file_header) + Size;
}

bool WriteVersionedFile(const char* Name, u32 Magic, u32 Version, const void* Data, u32 Size) {
    u32 Total = (u32)sizeof(file_header) + Size;
    u8* Buffer = (u8*)MemAlloc(Total);
    file_header Header = {Magic, Version, Size};
    memcpy(Buffer, &Header, sizeof(file_header));
    memcpy(Buffer + sizeof(file_header), Data, Size);
    bool Saved = SaveFileData(GetSavePath(Name), Buffer, (int)Total);
    MemFree(Buffer);
    return Saved;
}

bool ReadVersionedFile(const char* Name, u32 Magic, u32 Version, void* Data, u32 Size) {
    const char* Path = GetSavePath(Name);
    if (!FileExists(Path)) {
        return false;
    }
    int Bytes = 0;
    u8* File = LoadFileData(Path, &Bytes);
    if (!File) {
        return false;
    }
    bool Valid = false;
    if (Bytes >= (int)sizeof(file_header)) {
        file_header Header;
        memcpy(&Header, File, sizeof(file_header));
        Valid = IsValidHeader(Header, Magic, Version, Size, (u32)Bytes);
    }
    if (Valid) {
        memcpy(Data, File + sizeof(file_header), Size);
    }
    UnloadFileData(File);
    return Valid;
}
