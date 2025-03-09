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

#include <Windows.h>
#include <wincodec.h>
#include <d2d1.h>

class Sprite
{
public:
    static HRESULT CreateSpriteFromResource(
        ID2D1HwndRenderTarget*  pHwndRenderTarget,
        LPCTSTR                 lpszName,
        LPCTSTR                 lpszType,
        Sprite**                ppSprite);

    static HRESULT CreateSpriteFromIWICBitmapSource(
        ID2D1RenderTarget*  pRenderTarget,
        IWICBitmapSource*   pBitmapSource,
        Sprite**            ppSprite);

    ~Sprite();

    VOID SetPosition(CONST D2D1_POINT_2F& position);
    D2D1_POINT_2F GetPosition() CONST;

    VOID SetRotation(FLOAT fAngle);
    FLOAT GetRotation() CONST;

    VOID SetRotationCenter(CONST D2D1_POINT_2F& center);
    D2D1_POINT_2F GetRotationCenter() CONST;

    VOID SetScale(CONST D2D1_SIZE_F& scale);
    D2D1_SIZE_F GetScale() CONST;

    VOID SetScaleCenter(CONST D2D1_POINT_2F& center);
    D2D1_POINT_2F GetScaleCenter() CONST;

    D2D1_SIZE_U GetBitmapSize() CONST;

    HRESULT Draw(ID2D1RenderTarget* pRenderTarget);
    
private:
    Sprite();

    ID2D1Bitmap*    _pBitmap;
    D2D1_SIZE_U     _bitmapSize;
    D2D1_POINT_2F   _position;
    D2D1_SIZE_F     _scale;
    D2D1_POINT_2F   _scaleCenter;
    FLOAT           _fRotation;
    D2D1_POINT_2F   _rotationCenter;
};

#endif // __SPRITE_H