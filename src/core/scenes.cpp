#include "core/scenes.h"

#include <raylib.h>

static void ExitAllScenes(scene_stack& Stack) {
    while (Stack.Count > 0) {
        --Stack.Count;
        ExitScene(Stack.Ids[Stack.Count]);
    }
}

void StartScenes(scene_stack& Stack, scene_id First) {
    Stack = {};
    Stack.Ids[0] = First;
    Stack.Count = 1;
    EnterScene(First);
    Stack.Fade = FADE_IN;
}

void ChangeScene(scene_stack& Stack, scene_id Next) {
    if (IsFading(Stack)) {
        return;
    }
    Stack.Fade = FADE_OUT;
    Stack.FadeTimer = 0.f;
    Stack.Next = Next;
}

void PushScene(scene_stack& Stack, scene_id Scene) {
    if (IsFading(Stack)) {
        return;
    }
    if (Stack.Count == MAX_SCENE_DEPTH) {
        TraceLog(LOG_WARNING, "scenes: stack is full, scene %d not pushed", Scene);
        return;
    }
    Stack.Ids[Stack.Count++] = Scene;
    EnterScene(Scene);
}

void PopScene(scene_stack& Stack) {
    if (IsFading(Stack)) {
        return;
    }
    if (Stack.Count <= 1) {
        TraceLog(LOG_WARNING, "scenes: cannot pop the base scene");
        return;
    }
    --Stack.Count;
    ExitScene(Stack.Ids[Stack.Count]);
}

void UpdateScenes(scene_stack& Stack, f32 Dt) {
    switch (Stack.Fade) {
        case FADE_OUT:
            Stack.FadeTimer += Dt;
            if (Stack.FadeTimer >= FADE_SECONDS) {
                ExitAllScenes(Stack);
                Stack.Ids[0] = Stack.Next;
                Stack.Count = 1;
                EnterScene(Stack.Next);
                Stack.Fade = FADE_IN;
                Stack.FadeTimer = 0.f;
            }
            break;
        case FADE_IN:
            Stack.FadeTimer += Dt;
            if (Stack.FadeTimer >= FADE_SECONDS) {
                Stack.Fade = FADE_NONE;
                Stack.FadeTimer = 0.f;
            }
            break;
        case FADE_NONE:
            if (Stack.Count > 0) {
                UpdateScene(GetTopScene(Stack), Dt);
            }
            break;
    }
}

void DrawScenes(const scene_stack& Stack) {
    for (u32 Index = 0; Index < Stack.Count; ++Index) {
        DrawScene(Stack.Ids[Index]);
    }
}

void DrawFadeOverlay(const scene_stack& Stack) {
    f32 Amount = GetFadeAmount(Stack);
    if (Amount > 0.f) {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, Amount));
    }
}

scene_id GetTopScene(const scene_stack& Stack) {
    return Stack.Ids[Stack.Count > 0 ? Stack.Count - 1 : 0];
}

f32 GetFadeAmount(const scene_stack& Stack) {
    switch (Stack.Fade) {
        case FADE_OUT:
            return Stack.FadeTimer / FADE_SECONDS;
        case FADE_IN:
            return 1.f - Stack.FadeTimer / FADE_SECONDS;
        case FADE_NONE:
            break;
    }
    return 0.f;
}

bool IsFading(const scene_stack& Stack) {
    return Stack.Fade != FADE_NONE;
}
