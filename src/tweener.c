/**
 * Copyright 2025 haloperidozz
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *           http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "tweener.h"

#include <math.h>

struct _TWEENER {
    FLOAT  fDuration;
    FLOAT  fStart;
    FLOAT  fTarget;
    FLOAT  fProgress;
    FLOAT  fValue;
    BOOL   bInverted;
    EASING fnEasing;
};

static FLOAT LinearEasing(FLOAT x)
{
    return x;
}

TWEENER Tweener_Create(FLOAT fDuration, FLOAT fStart, FLOAT fTarget)
{
    TWEENER tweener = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
                                sizeof(struct _TWEENER));

    if (tweener == NULL) {
        return NULL;
    }

    tweener->fDuration = fDuration;
    tweener->fStart = fStart;
    tweener->fTarget = fTarget;
    tweener->fnEasing = LinearEasing;
    tweener->fProgress = 0.0f;
    tweener->fValue = fStart;
    tweener->bInverted = FALSE;

    return tweener;
}

BOOL Tweener_Update(TWEENER tweener, FLOAT fDelta)
{
    FLOAT fProgressRatio, fEasedProgress, fDiff;

    if (tweener == NULL) {
        return FALSE;
    }

    if (tweener->fDuration <= 0.0f) {
        tweener->fValue = tweener->fTarget;
        tweener->fProgress = tweener->fDuration;
        return FALSE;
    }

    fDelta = (tweener->bInverted) ? -fDelta : fDelta;
    tweener->fProgress = tweener->fProgress + fDelta;

    if (tweener->fProgress < 0.0f) {
        tweener->fProgress = 0.0f;
    } else if (tweener->fProgress > tweener->fDuration) {
        tweener->fProgress = tweener->fDuration;
    }

    fProgressRatio = tweener->fProgress / tweener->fDuration;

    fEasedProgress = tweener->fnEasing(fProgressRatio);
    fDiff = tweener->fTarget - tweener->fStart;

    tweener->fValue = tweener->fStart + fDiff * fEasedProgress;

    return tweener->fProgress < tweener->fDuration;
}

VOID Tweener_Invert(TWEENER tweener, BOOL bInvert)
{
    if (tweener == NULL) {
        return;
    }

    tweener->bInverted = bInvert;
}

FLOAT Tweener_GetValue(TWEENER tweener)
{
    if (tweener == NULL) {
        return 0.0f;
    }

    return tweener->fValue;
}

VOID Tweener_SetEasing(TWEENER tweener, EASING fnEasing)
{
    if (tweener == NULL) {
        return;
    }

    tweener->fnEasing = fnEasing ? fnEasing : LinearEasing;

    Tweener_Reset(tweener);
}

VOID Tweener_Reset(TWEENER tweener)
{
    if (tweener == NULL) {
        return;
    }

    tweener->fProgress = 0;
    tweener->fValue = 0;
}

VOID Tweener_Destroy(TWEENER tweener)
{
    if (tweener == NULL) {
        return;
    }

    HeapFree(GetProcessHeap(), 0, tweener);
}