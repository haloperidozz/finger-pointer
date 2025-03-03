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

#include "helper.h"

struct _SPRITE {
    ID2D1Bitmap    *pBitmap;
    D2D1_SIZE_U     bitmapSize;
    D2D1_POINT_2F   position;
    FLOAT           fAngle;
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

    pSprite = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(SPRITE));

    if (pSprite == NULL) {
        ID2D1Bitmap_Release(pBitmap);
        goto cleanup;
    }

    pBitmapSource->lpVtbl->GetSize(
        pBitmapSource,
        &(pSprite->bitmapSize.width), &(pSprite->bitmapSize.height));

    pSprite->pBitmap = pBitmap;

cleanup:
    if (pConverter != NULL) {
        pConverter->lpVtbl->Release(pConverter);
    }

    if (pFrame != NULL) {
        pFrame->lpVtbl->Release(pFrame);
    }

    if (pDecoder != NULL) {
        pDecoder->lpVtbl->Release(pDecoder);
    }

    if (pStream != NULL) {
        pStream->lpVtbl->Release(pStream);
    }

    if (pFactory != NULL) {
        pFactory->lpVtbl->Release(pFactory);
    }

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
        pSprite->fAngle = fAngle;
    }
}

FLOAT Sprite_GetRotation(CONST PSPRITE pSprite)
{
    return (pSprite != NULL) ? pSprite->fAngle : 0.0f;
}

D2D1_SIZE_U Sprite_GetSize(CONST PSPRITE pSprite)
{
    return (pSprite != NULL) ? pSprite->bitmapSize : (D2D1_SIZE_U) {0};
}

D2D1_POINT_2F Sprite_GetCenterPoint(CONST PSPRITE pSprite)
{
    D2D1_SIZE_U size;

    if (pSprite == NULL) {
        return (D2D1_POINT_2F) {0};
    }
    
    size = Sprite_GetSize(pSprite);

    return (D2D1_POINT_2F) {
        .x = (FLOAT) size.width  / 2.0f,
        .y = (FLOAT) size.height / 2.0f
    };
}

VOID Sprite_Draw(CONST PSPRITE pSprite, ID2D1RenderTarget *pRenderTarget)
{
    D2D1_POINT_2F centerPoint;
    D2D1_MATRIX_3X2_F rotation, translation, transform;

    if (pSprite == NULL || pRenderTarget == NULL) {
        return;
    }

    centerPoint = Sprite_GetCenterPoint(pSprite);

    MakeRotateMatrix(pSprite->fAngle, centerPoint, &rotation);
    MakeTranslateMatrixByPoint(pSprite->position, &translation);
    MultiplyMatrices(&translation, &rotation, &transform);

    ID2D1RenderTarget_SetTransform(pRenderTarget, &transform);

    ID2D1RenderTarget_DrawBitmap(
        pRenderTarget, pSprite->pBitmap, NULL, 1.0f,
        D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);
}

VOID Sprite_Destroy(PSPRITE pSprite)
{
    if (pSprite == NULL) {
        return;
    }

    ID2D1Bitmap_Release(pSprite->pBitmap);

    HeapFree(GetProcessHeap(), 0, pSprite);
}