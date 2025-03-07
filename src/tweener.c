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

#include "helper.h"

struct _TWEENER {
    FLOAT       fDuration;
    FLOAT       fStart;
    FLOAT       fTarget;
    FLOAT       fProgress;
    FLOAT       fValue;
    BOOL        bInverted;
    PFNEASING   pfnEasing;
};

PTWEENER Tweener_Create(FLOAT fDuration, FLOAT fStart, FLOAT fTarget)
{
    PTWEENER pTweener = NULL;
    
    pTweener = SafeAllocSizeof(TWEENER);
    
    if (pTweener == NULL) {
        return NULL;
    }

    pTweener->fDuration = fDuration;
    pTweener->fStart    = fStart;
    pTweener->fTarget   = fTarget;
    pTweener->pfnEasing = Easing_Linear;
    pTweener->fValue    = fStart;

    return pTweener;
}

BOOL Tweener_Update(PTWEENER pTweener, FLOAT fDeltaTime)
{
    FLOAT fRatio, fEasedValue, fValueDiff;

    if (pTweener == NULL) {
        return FALSE;
    }

    if (pTweener->fDuration <= 0.0f) {
        pTweener->fValue = pTweener->fTarget;
        pTweener->fProgress = pTweener->fDuration;
        return FALSE;
    }

    pTweener->fProgress += (pTweener->bInverted ? -fDeltaTime : fDeltaTime);

    if (pTweener->fProgress < 0.0f) {
        pTweener->fProgress = 0.0f;
    } else if (pTweener->fProgress > pTweener->fDuration) {
        pTweener->fProgress = pTweener->fDuration;
    }

    fRatio = pTweener->fProgress / pTweener->fDuration;
    fEasedValue = pTweener->pfnEasing(fRatio);
    fValueDiff = pTweener->fTarget - pTweener->fStart;

    pTweener->fValue = pTweener->fStart + fValueDiff * fEasedValue;

    return pTweener->fProgress < pTweener->fDuration;
}

VOID Tweener_Invert(PTWEENER pTweener, BOOL bInvert)
{
    if (pTweener != NULL) {
        pTweener->bInverted = bInvert;
    }
}

FLOAT Tweener_GetValue(CONST PTWEENER pTweener)
{
    return (pTweener != NULL) ? pTweener->fValue : 0.0f;
}

VOID Tweener_SetEasing(PTWEENER pTweener, PFNEASING pfnEasing)
{
    if (pTweener == NULL) {
        return;
    }

    pTweener->pfnEasing = (pfnEasing != NULL) ? pfnEasing : Easing_Linear;

    Tweener_Reset(pTweener);
}

VOID Tweener_Reset(PTWEENER pTweener)
{
    if (pTweener != NULL) {
        pTweener->fProgress = pTweener->fValue = 0.0f;
    }
}

VOID Tweener_Destroy(PTWEENER pTweener)
{
    SafeFree(pTweener);
}