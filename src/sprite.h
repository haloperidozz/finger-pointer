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

typedef struct _SPRITE SPRITE, *PSPRITE;

PSPRITE Sprite_CreateFromResource(ID2D1HwndRenderTarget *pRenderTarget,
                                  LPCTSTR lpszName,
                                  LPCTSTR lpszType);

VOID Sprite_SetPosition(PSPRITE pSprite, D2D1_POINT_2F position);
D2D1_POINT_2F Sprite_GetPosition(CONST PSPRITE pSprite);

VOID Sprite_SetRotation(PSPRITE pSprite, FLOAT fAngle);
FLOAT Sprite_GetRotation(CONST PSPRITE pSprite);

VOID Sprite_SetRotationCenter(PSPRITE pSprite, D2D1_POINT_2F center);
D2D1_POINT_2F Sprite_GetRotationCenter(CONST PSPRITE pSprite);

VOID Sprite_SetScale(PSPRITE pSprite, D2D1_SIZE_F scale);
D2D1_SIZE_F Sprite_GetScale(CONST PSPRITE pSprite);

VOID Sprite_SetScaleCenter(PSPRITE pSprite, D2D1_POINT_2F center);
D2D1_POINT_2F Sprite_GetScaleCenter(CONST PSPRITE pSprite);

D2D1_SIZE_U Sprite_GetBitmapSize(CONST PSPRITE pSprite);

VOID Sprite_Draw(CONST PSPRITE pSprite, ID2D1HwndRenderTarget *pRenderTarget);

VOID Sprite_Destroy(PSPRITE pSprite);

#endif /* __SPRITE_H */