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

typedef FLOAT (*EASING)(FLOAT x);

typedef struct _TWEENER *TWEENER;

TWEENER Tweener_Create(FLOAT fDuration, FLOAT fStart, FLOAT fTarget);

BOOL Tweener_Update(TWEENER tweener, FLOAT fDelta);

VOID Tweener_Invert(TWEENER tweener, BOOL bInvert);

FLOAT Tweener_GetValue(TWEENER tweener);

VOID Tweener_SetEasing(TWEENER tweener, EASING fnEasing);

VOID Tweener_Reset(TWEENER tweener);

VOID Tweener_Destroy(TWEENER tweener);

#endif /* __TWEENER_H */