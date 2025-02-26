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

#ifndef __SPRITE_H
#define __SPRITE_H

#include <windows.h>
#include <d2d1.h>
#include <tchar.h>

typedef struct _SPRITE *SPRITE;

SPRITE Sprite_LoadFromResource(ID2D1HwndRenderTarget *pRenderTarget,
                               LPCTSTR lpszName, LPCTSTR lpszType);

D2D1_POINT_2F Sprite_GetCenterPoint(SPRITE sprite);

VOID Sprite_SetPosition(SPRITE sprite, D2D1_POINT_2F position);
D2D1_POINT_2F Sprite_GetPosition(SPRITE sprite);

VOID Sprite_SetRotation(SPRITE sprite, FLOAT fAngle);
FLOAT Sprite_GetRotation(SPRITE sprite);

VOID Sprite_Draw(SPRITE sprite, ID2D1RenderTarget *pRenderTarget);

VOID Sprite_Destroy(SPRITE sprite);

#endif /* __SPRITE_H */