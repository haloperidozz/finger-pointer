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

#include "mat3x2f.h"

struct _SPRITE {
    ID2D1Bitmap   *pBitmap;
    D2D1_POINT_2F  position;
    UINT           uWidth;
    UINT           uHeight;
    FLOAT          fAngle;
};

static LPVOID LoadResourceToMemory(HINSTANCE hInstance, LPCTSTR lpszName,
                                   LPCTSTR lpszType, LPDWORD lpdwSize)
{
    HRSRC hResource = NULL;
    DWORD dwImageSize;
    HGLOBAL hResData = NULL;

    hResource = FindResource(hInstance, lpszName, lpszType);

    if (hResource == NULL) {
        return NULL;
    }

    dwImageSize = SizeofResource(hInstance, hResource);

    if (dwImageSize <= 0) {
        return NULL;
    }

    hResData = LoadResource(hInstance, hResource);

    if (hResData == NULL) {
        return NULL;
    }

    *lpdwSize = dwImageSize;

    return LockResource(hResData);
}

SPRITE Sprite_LoadFromResource(ID2D1HwndRenderTarget *pRenderTarget,
                               LPCTSTR lpszName, LPCTSTR lpszType)
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
    HRESULT hRes = S_OK;
    SPRITE sprite = NULL;

    if (pRenderTarget == NULL) {
        return NULL;
    }

    hWnd = ID2D1HwndRenderTarget_GetHwnd(pRenderTarget);

    lpResourceData = LoadResourceToMemory(
        GetWindowInstance(hWnd), lpszName, lpszType, &dwResourceSize);

    if (lpResourceData == NULL) {
        return NULL;
    }

    hRes = CoCreateInstance(
        &CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER,
        &IID_IWICImagingFactory, (LPVOID*) &pFactory);
    
    if (FAILED(hRes)) {
        goto cleanup;
    }

    hRes = pFactory->lpVtbl->CreateStream(pFactory, &pStream);

    if (FAILED(hRes)) {
        goto cleanup;
    }

    hRes = pStream->lpVtbl->InitializeFromMemory(pStream,
        lpResourceData, dwResourceSize);
    
    if (FAILED(hRes)) {
        goto cleanup;
    }

    hRes = pStream->lpVtbl->QueryInterface(pStream, &IID_IStream,
                                           (LPVOID*) &pIStream);
    
    if (FAILED(hRes)) {
        goto cleanup;
    }

    hRes = pFactory->lpVtbl->CreateDecoderFromStream(pFactory,
        pIStream, NULL, WICDecodeMetadataCacheOnLoad, &pDecoder);

    if (FAILED(hRes)) {
        goto cleanup;
    }

    hRes = pDecoder->lpVtbl->GetFrame(pDecoder, 0, &pFrame);

    if (FAILED(hRes)) {
        goto cleanup;
    }

    hRes = pFactory->lpVtbl->CreateFormatConverter(pFactory, &pConverter);

    if (FAILED(hRes)) {
        goto cleanup;
    }

    hRes = pFrame->lpVtbl->QueryInterface(pFrame, &IID_IWICBitmapSource,
                                          (LPVOID*) &pBitmapSource);

    if (FAILED(hRes)) {
        goto cleanup;
    }

    hRes = pConverter->lpVtbl->Initialize(pConverter,
        pBitmapSource, &GUID_WICPixelFormat32bppPBGRA,
        WICBitmapDitherTypeNone, NULL, 0.0, WICBitmapPaletteTypeMedianCut);
    
    if (FAILED(hRes)) {
        goto cleanup;
    }

    hRes = pConverter->lpVtbl->QueryInterface(pConverter,
        &IID_IWICBitmapSource, (LPVOID*) &pBitmapSource);

    if (FAILED(hRes)) {
        goto cleanup;
    }

    hRes = ID2D1HwndRenderTarget_CreateBitmapFromWicBitmap(pRenderTarget,
        pBitmapSource, NULL, &pBitmap);
    
    if (FAILED(hRes)) {
        goto cleanup;
    }

    sprite = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
                       sizeof(struct _SPRITE));

    if (sprite == NULL) {
        ID2D1Bitmap_Release(pBitmap);
        goto cleanup;
    }

    sprite->pBitmap = pBitmap;

    pFrame->lpVtbl->GetSize(pFrame, &(sprite->uWidth), &(sprite->uHeight));

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

    return sprite;
}

D2D1_POINT_2F Sprite_GetCenterPoint(SPRITE sprite)
{
    if (sprite == NULL) {
        return (D2D1_POINT_2F) { 0, 0 };
    }

    return (D2D1_POINT_2F) {
        ((FLOAT) sprite->uWidth)  / 2.0f,
        ((FLOAT) sprite->uHeight) / 2.0f
    };
}

VOID Sprite_SetPosition(SPRITE sprite, D2D1_POINT_2F position)
{
    if (sprite == NULL) {
        return;
    }

    sprite->position = position;
}

D2D1_POINT_2F Sprite_GetPosition(SPRITE sprite)
{
    if (sprite == NULL) {
        return (D2D1_POINT_2F) { 0, 0 };
    }

    return sprite->position;
}

VOID Sprite_SetRotation(SPRITE sprite, FLOAT fAngle)
{
    if (sprite == NULL) {
        return;
    }

    sprite->fAngle = fAngle;
}

FLOAT Sprite_GetRotation(SPRITE sprite)
{
    if (sprite == NULL) {
        return 0.0f;
    }

    return sprite->fAngle;
}

VOID Sprite_Draw(SPRITE sprite, ID2D1RenderTarget *pRenderTarget)
{
    D2D1_POINT_2F center;
    D2D1_MATRIX_3X2_F rotation, translation, transform;

    if (sprite == NULL || sprite->pBitmap == NULL) {
        return;
    }

    center = Sprite_GetCenterPoint(sprite);

    rotation = Matrix3x2F_Rotation(sprite->fAngle, center);
    translation = Matrix3x2F_TranslationByPoint(sprite->position);
    transform = Matrix3x2F_Multiply(&translation, &rotation);

    ID2D1RenderTarget_SetTransform(pRenderTarget, &transform);

    ID2D1RenderTarget_DrawBitmap(pRenderTarget,
        sprite->pBitmap, NULL, 1.0f,
        D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);
}

VOID Sprite_Destroy(SPRITE sprite)
{
    if (sprite == NULL) {
        return;
    }

    ID2D1Bitmap_Release(sprite->pBitmap);

    HeapFree(GetProcessHeap(), 0, sprite);
}