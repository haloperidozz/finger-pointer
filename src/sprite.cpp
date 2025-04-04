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

#include "sprite.h"

#include <windowsx.h>
#include <d2d1helper.h>

#include "safemem.h"
#include "resource.h"

Sprite::Sprite()
    : _pBitmap(NULL),
      _bitmapSize(D2D1::SizeU()),
      _position(D2D1::Point2F()),
      _scale(D2D1::SizeF(1.0f, 1.0f)),
      _scaleCenter(D2D1::Point2F()),
      _fRotation(0.0f),
      _rotationCenter(D2D1::Point2F())
{
}

Sprite::~Sprite()
{
    SafeRelease(&_pBitmap);
}

////////////////////////////////////////////////////////////////////////////

VOID Sprite::SetPosition(CONST D2D1_POINT_2F& position)
{
    _position = position;
}

D2D1_POINT_2F Sprite::GetPosition() CONST
{
    return _position;
}

VOID Sprite::SetRotation(FLOAT fAngle)
{
    _fRotation = fAngle;
}

FLOAT Sprite::GetRotation() CONST
{
    return _fRotation;
}

VOID Sprite::SetRotationCenter(CONST D2D1_POINT_2F& center)
{
    _rotationCenter = center;
}

D2D1_POINT_2F Sprite::GetRotationCenter() CONST
{
    return _rotationCenter;
}

VOID Sprite::SetScale(CONST D2D1_SIZE_F& scale)
{
    _scale = scale;
}

D2D1_SIZE_F Sprite::GetScale() CONST
{
    return _scale;
}

VOID Sprite::SetScaleCenter(CONST D2D1_POINT_2F& center)
{
    _scaleCenter = center;
}

D2D1_POINT_2F Sprite::GetScaleCenter() CONST
{
    return _scaleCenter;
}

D2D1_SIZE_U Sprite::GetBitmapSize() CONST
{
    return _bitmapSize;
}

////////////////////////////////////////////////////////////////////////////

HRESULT Sprite::Draw(ID2D1RenderTarget* pRenderTarget)
{
    D2D1::Matrix3x2F rotate, translate, scale;
    D2D1::Matrix3x2F oldTransform, transform;

    if (pRenderTarget == NULL) {
        return E_INVALIDARG;
    }

    if (_pBitmap == NULL) {
        return E_FAIL;
    }

    pRenderTarget->GetTransform(&oldTransform);

    translate = D2D1::Matrix3x2F::Translation(_position.x, _position.y);
    rotate = D2D1::Matrix3x2F::Rotation(_fRotation, _rotationCenter);
    scale = D2D1::Matrix3x2F::Scale(_scale, _scaleCenter);

    transform = scale * rotate * translate;

    pRenderTarget->SetTransform(&transform);

    pRenderTarget->DrawBitmap(
        _pBitmap,
        NULL,
        1.0f,
        D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
        NULL);
    
    pRenderTarget->SetTransform(&oldTransform);

    return S_OK;
}

////////////////////////////////////////////////////////////////////////////

HRESULT Sprite::CreateSpriteFromResource(
    ID2D1HwndRenderTarget*  pHwndRenderTarget,
    LPCTSTR                 lpszName,
    LPCTSTR                 lpszType,
    Sprite**                ppSprite)
{
    IStream*                pIStream = NULL;
    IWICImagingFactory*     pFactory = NULL;
    IWICBitmapDecoder*      pDecoder = NULL;
    IWICBitmapFrameDecode*  pFrame = NULL;
    IWICFormatConverter*    pConverter = NULL;
    HRESULT                 hResult = S_OK;

    if (pHwndRenderTarget == NULL || ppSprite == NULL) {
        return E_INVALIDARG;
    }

    pIStream = CreateIStreamFromResource(
        GetWindowInstance(pHwndRenderTarget->GetHwnd()),
        lpszName,
        lpszType);
    
    if (pIStream == NULL) {
        return E_INVALIDARG;
    }

    hResult = CoCreateInstance(
        CLSID_WICImagingFactory,
        NULL,
        CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(&pFactory));
    
    if (FAILED(hResult)) {
        goto cleanup;
    }

    hResult = pFactory->CreateDecoderFromStream(
        pIStream,
        NULL,
        WICDecodeMetadataCacheOnLoad,
        &pDecoder);

    if (FAILED(hResult)) {
        goto cleanup;
    }

    hResult = pDecoder->GetFrame(0, &pFrame);

    if (FAILED(hResult)) {
        goto cleanup;
    }

    hResult = pFactory->CreateFormatConverter(&pConverter);

    if (FAILED(hResult)) {
        goto cleanup;
    }

    hResult = pConverter->Initialize(
        pFrame,
        GUID_WICPixelFormat32bppPBGRA,
        WICBitmapDitherTypeNone,
        NULL,
        0.0,
        WICBitmapPaletteTypeMedianCut);
    
    if (FAILED(hResult)) {
        goto cleanup;
    }

    hResult = CreateSpriteFromIWICBitmapSource(
        pHwndRenderTarget,
        pConverter,
        ppSprite);

cleanup:
    SafeRelease(&pConverter);
    SafeRelease(&pFrame);
    SafeRelease(&pDecoder);
    SafeRelease(&pFactory);
    SafeRelease(&pIStream);

    return hResult;
}

HRESULT Sprite::CreateSpriteFromIWICBitmapSource(
    ID2D1RenderTarget*  pRenderTarget,
    IWICBitmapSource*   pBitmapSource,
    Sprite**            ppSprite)
{
    Sprite* pSprite;
    HRESULT hResult = S_OK;

    if (ppSprite == NULL) {
        return E_INVALIDARG;
    }

    if (pRenderTarget == NULL || pBitmapSource == NULL) {
        return E_INVALIDARG;
    }
    
    pSprite = new Sprite();

    if (pSprite == NULL) {
        return E_OUTOFMEMORY;
    }

    hResult = pRenderTarget->CreateBitmapFromWicBitmap(
        pBitmapSource,
        &(pSprite->_pBitmap));
    
    if (FAILED(hResult)) {
        goto cleanup;
    }

    hResult = pBitmapSource->GetSize(
        &(pSprite->_bitmapSize.width),
        &(pSprite->_bitmapSize.height));

cleanup:
    if (SUCCEEDED(hResult)) {
        *ppSprite = pSprite;
    } else if (pSprite != NULL){
        delete pSprite;
    }

    return hResult;
}