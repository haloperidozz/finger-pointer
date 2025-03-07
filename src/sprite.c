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

#include <wincodec.h>
#include <windowsx.h>

#include "d2d1_vtbl.h"
#include "mat3x2f.h"
#include "helper.h"

struct _SPRITE {
    ID2D1Bitmap    *pBitmap;
    D2D1_SIZE_U     bitmapSize;
    D2D1_POINT_2F   position;
    D2D1_SIZE_F     scale;
    D2D1_POINT_2F   scaleCenter;
    FLOAT           fRotation;
    D2D1_POINT_2F   rotationCenter;
};

PSPRITE Sprite_CreateFromResource(ID2D1HwndRenderTarget *pRenderTarget,
                                  LPCTSTR lpszName,
                                  LPCTSTR lpszType)
{
    IWICImagingFactory *pFactory = NULL;
    IWICStream *pStream = NULL;
    IWICBitmapDecoder *pDecoder = NULL;
    IWICBitmapFrameDecode *pFrame = NULL;
    IWICBitmapSource *pBitmapSource = NULL;
    IWICFormatConverter *pConverter = NULL;
    IStream *pIStream = NULL;
    ID2D1Bitmap *pBitmap = NULL;
    HWND hWnd = NULL;
    LPVOID lpResourceData = NULL;
    DWORD dwResourceSize;
    HRESULT hResult = S_OK;
    PSPRITE pSprite = NULL;

    if (pRenderTarget == NULL) {
        return NULL;
    }

    hWnd = ID2D1HwndRenderTarget_GetHwnd(pRenderTarget);

    lpResourceData = LoadResourceToMemory(
        GetWindowInstance(hWnd), lpszName, lpszType, &dwResourceSize);

    if (lpResourceData == NULL) {
        return NULL;
    }

    hResult = CoCreateInstance(
        &CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER,
        &IID_IWICImagingFactory, (LPVOID*) &pFactory);
    
    if (FAILED(hResult)) {
        goto cleanup;
    }

    hResult = pFactory->lpVtbl->CreateStream(pFactory, &pStream);

    if (FAILED(hResult)) {
        goto cleanup;
    }

    hResult = pStream->lpVtbl->InitializeFromMemory(
        pStream, lpResourceData, dwResourceSize);
    
    if (FAILED(hResult)) {
        goto cleanup;
    }

    hResult = pStream->lpVtbl->QueryInterface(
        pStream, &IID_IStream, (LPVOID*) &pIStream);
    
    if (FAILED(hResult)) {
        goto cleanup;
    }

    hResult = pFactory->lpVtbl->CreateDecoderFromStream(
        pFactory, pIStream, NULL, WICDecodeMetadataCacheOnLoad, &pDecoder);

    if (FAILED(hResult)) {
        goto cleanup;
    }

    hResult = pDecoder->lpVtbl->GetFrame(pDecoder, 0, &pFrame);

    if (FAILED(hResult)) {
        goto cleanup;
    }

    hResult = pFactory->lpVtbl->CreateFormatConverter(pFactory, &pConverter);

    if (FAILED(hResult)) {
        goto cleanup;
    }

    hResult = pFrame->lpVtbl->QueryInterface(
        pFrame, &IID_IWICBitmapSource, (LPVOID*) &pBitmapSource);

    if (FAILED(hResult)) {
        goto cleanup;
    }

    hResult = pConverter->lpVtbl->Initialize(
        pConverter, pBitmapSource, &GUID_WICPixelFormat32bppPBGRA,
        WICBitmapDitherTypeNone, NULL, 0.0, WICBitmapPaletteTypeMedianCut);
    
    if (FAILED(hResult)) {
        goto cleanup;
    }

    hResult = pConverter->lpVtbl->QueryInterface(
        pConverter, &IID_IWICBitmapSource, (LPVOID*) &pBitmapSource);

    if (FAILED(hResult)) {
        goto cleanup;
    }

    hResult = ID2D1HwndRenderTarget_CreateBitmapFromWicBitmap(
        pRenderTarget, pBitmapSource, NULL, &pBitmap);
    
    if (FAILED(hResult)) {
        goto cleanup;
    }

    pSprite = SafeAllocSizeof(SPRITE);

    if (pSprite == NULL) {
        ID2D1Bitmap_Release(pBitmap);
        goto cleanup;
    }

    pSprite->pBitmap = pBitmap;
    
    pBitmapSource->lpVtbl->GetSize(
        pBitmapSource,
        &(pSprite->bitmapSize.width), &(pSprite->bitmapSize.height));
    
    pSprite->scale = (D2D1_SIZE_F) { 1.0f, 1.0f };

cleanup:
    COM_SafeRelease(pConverter);
    COM_SafeRelease(pFrame);
    COM_SafeRelease(pDecoder);
    COM_SafeRelease(pStream);
    COM_SafeRelease(pFactory);

    return pSprite;
}

VOID Sprite_SetPosition(PSPRITE pSprite, D2D1_POINT_2F position)
{
    if (pSprite != NULL) {
        pSprite->position = position;
    }
}

D2D1_POINT_2F Sprite_GetPosition(CONST PSPRITE pSprite)
{
    return (pSprite != NULL) ? pSprite->position : (D2D1_POINT_2F) {0};
}

VOID Sprite_SetRotation(PSPRITE pSprite, FLOAT fAngle)
{
    if (pSprite != NULL) {
        pSprite->fRotation = fAngle;
    }
}

FLOAT Sprite_GetRotation(CONST PSPRITE pSprite)
{
    return (pSprite != NULL) ? pSprite->fRotation : 0.0f;
}

VOID Sprite_SetRotationCenter(PSPRITE pSprite, D2D1_POINT_2F center)
{
    if (pSprite != NULL) {
        pSprite->rotationCenter = center;
    }
}

D2D1_POINT_2F Sprite_GetRotationCenter(CONST PSPRITE pSprite)
{
    return (pSprite != NULL) ? pSprite->rotationCenter : (D2D1_POINT_2F) {0};
}

VOID Sprite_SetScale(PSPRITE pSprite, D2D1_SIZE_F scale)
{
    if (pSprite != NULL) {
        pSprite->scale = scale;
    }
}

D2D1_SIZE_F Sprite_GetScale(CONST PSPRITE pSprite)
{
    return (pSprite != NULL) ? pSprite->scale : (D2D1_SIZE_F) {0};
}

VOID Sprite_SetScaleCenter(PSPRITE pSprite, D2D1_POINT_2F center)
{
    if (pSprite != NULL) {
        pSprite->scaleCenter = center;
    }
}

D2D1_POINT_2F Sprite_GetScaleCenter(CONST PSPRITE pSprite)
{
    return (pSprite != NULL) ? pSprite->scaleCenter : (D2D1_POINT_2F) {0};
}

D2D1_SIZE_U Sprite_GetBitmapSize(CONST PSPRITE pSprite)
{
    return (pSprite != NULL) ? pSprite->bitmapSize : (D2D1_SIZE_U) {0};
}

VOID Sprite_Draw(CONST PSPRITE pSprite, ID2D1HwndRenderTarget *pRenderTarget)
{
    D2D1_MATRIX_3X2_F rotation, translation, scale;
    D2D1_MATRIX_3X2_F oldTransform, transform0, transform1;

    if (pSprite == NULL || pRenderTarget == NULL) {
        return;
    }

    ID2D1HwndRenderTarget_GetTransform(pRenderTarget, &oldTransform);

    MakeTranslateMatrix(pSprite->position, &translation);
    MakeRotateMatrix(pSprite->fRotation, pSprite->rotationCenter, &rotation);
    MakeScaleMatrix(pSprite->scale, pSprite->scaleCenter, &scale);

    /* T*R*S */
    MultiplyMatrices(&translation, &rotation, &transform0);
    MultiplyMatrices(&transform0, &scale, &transform1);

    ID2D1HwndRenderTarget_SetTransform(pRenderTarget, &transform1);

    ID2D1HwndRenderTarget_DrawBitmap(
        pRenderTarget, pSprite->pBitmap, NULL, 1.0f,
        D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);
    
    ID2D1HwndRenderTarget_SetTransform(pRenderTarget, &oldTransform);
}

VOID Sprite_Destroy(PSPRITE pSprite)
{
    if (pSprite == NULL) {
        return;
    }

    ID2D1Bitmap_Release(pSprite->pBitmap);
    
    SafeFree(pSprite);
}