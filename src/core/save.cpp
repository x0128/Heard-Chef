#include "core/save.h"

#include <cstring>
#include <raylib.h>

#if defined(__EMSCRIPTEN__)
#include <emscripten/emscripten.h>

EM_JS(int, WebStoreBytes, (const char* Name, const unsigned char* Data, int Size), {
    var Text = "";
    for (var Index = 0; Index < Size; Index++) {
        Text += String.fromCharCode(HEAPU8[Data + Index]);
    }
    try {
        localStorage.setItem(UTF8ToString(Name), btoa(Text));
        return 1;
    } catch (Error) {
        return 0;
    }
});

EM_JS(int, WebStoredSize, (const char* Name), {
    try {
        var Stored = localStorage.getItem(UTF8ToString(Name));
        return Stored == null ? -1 : atob(Stored).length;
    } catch (Error) {
        return -1;
    }
});

EM_JS(void, WebLoadBytes, (const char* Name, unsigned char* Out, int Size), {
    var Text = atob(localStorage.getItem(UTF8ToString(Name)));
    for (var Index = 0; Index < Size; Index++) {
        HEAPU8[Out + Index] = Text.charCodeAt(Index);
    }
});

static bool WriteBytes(const char* Name, u8* Data, u32 Size) {
    return WebStoreBytes(Name, Data, (int)Size) != 0;
}

static u8* ReadBytes(const char* Name, int* Bytes) {
    int Size = WebStoredSize(Name);
    if (Size < 0) {
        return nullptr;
    }
    u8* Data = (u8*)MemAlloc((unsigned int)Size + 1);
    WebLoadBytes(Name, Data, Size);
    *Bytes = Size;
    return Data;
}

static void FreeBytes(u8* Data) {
    MemFree(Data);
}
#else
static const char* GetSavePath(const char* Name) {
    return TextFormat("%s%s", GetApplicationDirectory(), Name);
}

static bool WriteBytes(const char* Name, u8* Data, u32 Size) {
    return SaveFileData(GetSavePath(Name), Data, (int)Size);
}

static u8* ReadBytes(const char* Name, int* Bytes) {
    const char* Path = GetSavePath(Name);
    if (!FileExists(Path)) {
        return nullptr;
    }
    return LoadFileData(Path, Bytes);
}

static void FreeBytes(u8* Data) {
    UnloadFileData(Data);
}
#endif

bool IsValidHeader(const file_header& Header, u32 Magic, u32 Version, u32 Size, u32 FileBytes) {
    return Header.Magic == Magic && Header.Version == Version && Header.Size == Size && FileBytes == sizeof(file_header) + Size;
}

bool WriteVersionedFile(const char* Name, u32 Magic, u32 Version, const void* Data, u32 Size) {
    u32 Total = (u32)sizeof(file_header) + Size;
    u8* Buffer = (u8*)MemAlloc(Total);
    file_header Header = {Magic, Version, Size};
    memcpy(Buffer, &Header, sizeof(file_header));
    memcpy(Buffer + sizeof(file_header), Data, Size);
    bool Saved = WriteBytes(Name, Buffer, Total);
    MemFree(Buffer);
    return Saved;
}

bool ReadVersionedFile(const char* Name, u32 Magic, u32 Version, void* Data, u32 Size) {
    int Bytes = 0;
    u8* File = ReadBytes(Name, &Bytes);
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
    FreeBytes(File);
    return Valid;
}
