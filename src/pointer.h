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

#ifndef __POINTER_H
#define __POINTER_H

#include <Windows.h>
#include <d2d1.h>

#include "sprite.h"
#include "Audio.h"
#include "tweener.h"

class Pointer {
public:
    Pointer();
    ~Pointer();

    HRESULT InitializeResources(
        ID2D1HwndRenderTarget* pRenderTarget,
        HINSTANCE              hInstance);

    VOID Update(FLOAT fDelta);
    VOID Draw(ID2D1RenderTarget* pRenderTarget);

    D2D1_POINT_2F GetPosition();
    VOID SetPosition(CONST D2D1_POINT_2F& position);

    FLOAT GetScale() CONST;
    VOID SetScale(FLOAT fScale);

    D2D1_SIZE_F GetSize() CONST;

    VOID OnPress();
    VOID OnRelease();

    VOID ToggleMarker();

private:
    Sprite*                 _pSprite;
    Audio*                  _pEffect;
    Audio*                  _pEffectMove;
    Tweener                 _tweener;

    ID2D1SolidColorBrush*   _markerBrush;

    D2D1::ColorF            _markerColor;
    D2D1_POINT_2F           _position;
    D2D1_POINT_2F           _lastPosition;
    D2D1_POINT_2F           _markerPosition;

    FLOAT                   _fScale;
    BOOL                    _bPressed;
    BOOL                    _bShowMarker;
};

#endif // __POINTER_H