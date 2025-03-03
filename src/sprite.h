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

D2D1_SIZE_U Sprite_GetSize(CONST PSPRITE pSprite);

D2D1_POINT_2F Sprite_GetCenterPoint(CONST PSPRITE pSprite);

VOID Sprite_Draw(CONST PSPRITE pSprite, ID2D1RenderTarget *pRenderTarget);

VOID Sprite_Destroy(PSPRITE pSprite);

static VOID Sprite_DrawHwnd(CONST PSPRITE pSprite,
                            ID2D1HwndRenderTarget *pHwndRenderTarget)
{
    ID2D1RenderTarget *pRenderTarget = NULL;

    if (pHwndRenderTarget == NULL) {
        return;
    }

    ID2D1HwndRenderTarget_QueryInterface(
        pHwndRenderTarget,
        &IID_ID2D1RenderTarget, (LPVOID*) &pRenderTarget);
    
    Sprite_Draw(pSprite, pRenderTarget);
}

#endif /* __SPRITE_H */