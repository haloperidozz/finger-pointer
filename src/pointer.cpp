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

#include "pointer.h"

#include "safemem.h"

#include "resource.h"

Pointer::Pointer()
    : _tweener(0.25f, -45.0f, 0.0f, Easing::EaseOutCirc),
      _fScale(0.9f),
      _bPressed(FALSE),
      _pEffect(NULL),
      _pEffectMove(NULL),
      _pSprite(NULL)
{
}

Pointer::~Pointer()
{
    SafeDelete(&_pSprite);
    SafeDelete(&_pEffect);
    SafeDelete(&_pEffectMove);
}

HRESULT Pointer::InitializeResources(
    ID2D1HwndRenderTarget* pRenderTarget,
    HINSTANCE              hInstance)
{
    HRESULT hResult;

    hResult = Sprite::CreateSpriteFromResource(
        pRenderTarget,
        MAKEINTRESOURCE(IDR_POINTER_PNG),
        RT_RCDATA,
        &_pSprite);

    if (FAILED(hResult)) {
        goto failed;
    }

    hResult = Audio::CreateAudioFromResource(
        hInstance,
        MAKEINTRESOURCE(IDR_EFFECT_WAV),
        TEXT("WAVE"),
        &_pEffect);

    if (FAILED(hResult)) {
        goto failed;
    }

    hResult = Audio::CreateAudioFromResource(
        hInstance,
        MAKEINTRESOURCE(IDR_EFFECT_MOVE_WAV),
        TEXT("WAVE"),
        &_pEffectMove);

    if (FAILED(hResult)) {
        goto failed;
    }

    _pEffectMove->SetLoop(TRUE);
    
    UpdateScale();

failed:
    if (FAILED(hResult)) {
        SafeDelete(&_pSprite);
        SafeDelete(&_pEffect);
        SafeDelete(&_pEffectMove);
    }

    return hResult;
}

VOID Pointer::Update(FLOAT fDelta)
{
    if (_bPressed == TRUE) {
        if (_lastPosition.x != _position.x ||
            _lastPosition.y != _position.y
        ) {
            _pEffectMove->Play();
        } else {
            _pEffectMove->Stop();
        }

        _lastPosition = _position;
    }

    if (_tweener.Update(fDelta)) {
        _pSprite->SetRotation(_tweener.GetValue());
    }
}

VOID Pointer::Draw(ID2D1RenderTarget *pRenderTarget)
{
    _pSprite->Draw(pRenderTarget);
}

D2D1_POINT_2F Pointer::GetPosition()
{
    return _position;
}

VOID Pointer::SetPosition(CONST D2D1_POINT_2F &position)
{
    _position = position;
    _pSprite->SetPosition(position);
}

FLOAT Pointer::GetScale() CONST
{
    return _fScale;
}

VOID Pointer::SetScale(FLOAT fScale)
{
    _fScale = fmaxf(0.0f, fminf(fScale, 1.0f));
    UpdateScale();
}

D2D1_SIZE_F Pointer::GetSize() CONST
{
    D2D1_SIZE_U bitmapSize;
    D2D1_SIZE_F size;

    bitmapSize = _pSprite->GetBitmapSize();

    size.width  = (FLOAT) bitmapSize.width  * _fScale;
    size.height = (FLOAT) bitmapSize.height * _fScale;

    return size;
}

VOID Pointer::SetPressed(BOOL bPressed)
{
    _bPressed = bPressed;

    _tweener.Invert(bPressed);

    if (_bPressed == TRUE) {
        _pEffect->Play();
    } else {
        _pEffect->Stop();
        _pEffectMove->Stop();
    }
}

VOID Pointer::UpdateScale()
{
    D2D1_SIZE_U   bitmapSize;
    D2D1_POINT_2F center;

    bitmapSize = _pSprite->GetBitmapSize();

    center.x = ((FLOAT) bitmapSize.width  * _fScale) / 2.0f;
    center.y =  (FLOAT) bitmapSize.height * _fScale;

    _pSprite->SetRotationCenter(center);
    _pSprite->SetScale(D2D1::SizeF(_fScale, _fScale));
}
