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

#ifndef __TWEENER_H
#define __TWEENER_H

#include <windows.h>

typedef FLOAT (*PFNEASING)(FLOAT x);

typedef struct _TWEENER TWEENER, *PTWEENER;

PTWEENER Tweener_Create(FLOAT fDuration, FLOAT fStart, FLOAT fTarget);

BOOL Tweener_Update(PTWEENER pTweener, FLOAT fDeltaTime);

VOID Tweener_Invert(PTWEENER pTweener, BOOL bInvert);

FLOAT Tweener_GetValue(CONST PTWEENER pTweener);

VOID Tweener_SetEasing(PTWEENER pTweener, PFNEASING pfnEasing);

VOID Tweener_Reset(PTWEENER pTweener);

VOID Tweener_Destroy(PTWEENER pTweener);

#endif /* __TWEENER_H */