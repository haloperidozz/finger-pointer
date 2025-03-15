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

#define MARKER_SIZE 2.5f

Pointer::Pointer()
    : _tweener(0.25f, -45.0f, 0.0f, Easing::EaseOutCirc),
      _markerColor(D2D1::ColorF::Red),
      _fScale(0.9f),
      _bPressed(FALSE),
      _bShowMarker(TRUE),
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

    hResult = pRenderTarget->CreateSolidColorBrush(
        _markerColor,
        &_markerBrush);
    
    if (FAILED(hResult)) {
        goto failed;
    }

    _pEffectMove->SetLoop(TRUE);
    
    SetScale(_fScale);

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
    BOOL bHasMoved = FALSE;

    if (_bPressed == TRUE) {
        bHasMoved = _lastPosition.x != _position.x ||
                    _lastPosition.y != _position.y;
        
        if (bHasMoved == TRUE) {
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
    if (_bShowMarker == TRUE) {
        pRenderTarget->FillEllipse(
            D2D1::Ellipse(_markerPosition, MARKER_SIZE, MARKER_SIZE),
            _markerBrush);
    }
    
    _pSprite->Draw(pRenderTarget);
}

D2D1_POINT_2F Pointer::GetPosition()
{
    return _position;
}

VOID Pointer::SetPosition(CONST D2D1_POINT_2F &position)
{
    D2D1_POINT_2F markerPosition;

    _position = position;
    _pSprite->SetPosition(position);

    // The values 45.0f and 35.0f were chosen empirically  
    // TODO: They need to be somehow linked to the sprite size  
    markerPosition.x = position.x - 45.0f * _fScale;
    markerPosition.y = position.y + 35.0f * _fScale;

    _markerPosition = markerPosition;
}

FLOAT Pointer::GetScale() CONST
{
    return _fScale;
}

VOID Pointer::SetScale(FLOAT fScale)
{
    D2D1_SIZE_U   bitmapSize;
    D2D1_POINT_2F center;

    _fScale = fmaxf(0.0f, fminf(fScale, 1.0f));

    bitmapSize = _pSprite->GetBitmapSize();

    center.x = ((FLOAT) bitmapSize.width  * _fScale) / 2.0f;
    center.y =  (FLOAT) bitmapSize.height * _fScale;

    _pSprite->SetRotationCenter(center);
    _pSprite->SetScale(D2D1::SizeF(_fScale, _fScale));
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

VOID Pointer::OnPress()
{
    _bPressed = TRUE;
    _tweener.Invert(TRUE);
    _pEffect->Play();
}

VOID Pointer::OnRelease()
{
    _bPressed = FALSE;
    _tweener.Invert(FALSE);
    _pEffect->Stop();
    _pEffectMove->Stop();
}

VOID Pointer::ToggleMarker()
{
    _bShowMarker = !_bShowMarker;
}