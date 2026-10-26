#pragma once

#include "core/types.h"

using scene_id = u8;

constexpr u32 MAX_SCENE_DEPTH = 4;
constexpr f32 FADE_SECONDS = 0.2f;

enum fade_phase : u8 {
    FADE_NONE,
    FADE_OUT,
    FADE_IN,
};

struct scene_stack {
    scene_id Ids[MAX_SCENE_DEPTH] = {};
    u32 Count = 0;
    fade_phase Fade = FADE_NONE;
    f32 FadeTimer = 0.f;
    scene_id Next = 0;
};

void StartScenes(scene_stack& Stack, scene_id First);
void ChangeScene(scene_stack& Stack, scene_id Next);
void PushScene(scene_stack& Stack, scene_id Scene);
void PopScene(scene_stack& Stack);

void UpdateScenes(scene_stack& Stack, f32 Dt);
void DrawScenes(const scene_stack& Stack);
void DrawFadeOverlay(const scene_stack& Stack);

scene_id GetTopScene(const scene_stack& Stack);
f32 GetFadeAmount(const scene_stack& Stack);
bool IsFading(const scene_stack& Stack);

void EnterScene(scene_id Scene);
void ExitScene(scene_id Scene);
void UpdateScene(scene_id Scene, f32 Dt);
void DrawScene(scene_id Scene);
