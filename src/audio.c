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

#include "audio.h"

#include <windowsx.h>
#include <mfapi.h>
#include <mfplay.h>
#include <shlwapi.h>

#include "helper.h"

struct _AUDIO {
    IMFPMediaPlayer    *pPlayer;
    BOOL                bLoop;
    BOOL                bPlaying;
};

struct _AUDIOCALLBACK {
    IMFPMediaPlayerCallback iface;
    LONG                    lRefCount;
    PAUDIO                  pAudio; 
};

/***********************************************************************
 * MFCreateMFByteStreamOnStream
 *
 * HACK: For reasons unknown to me, the mfidl.h header in MinGW lacks
 * a definition for the MFCreateMFByteStreamOnStream function.
 ***********************************************************************/

#if __MINGW32__

typedef HRESULT (*MFCreateMFByteStreamOnStream_t)(IStream*, IMFByteStream**);

static HRESULT
MFCreateMFByteStreamOnStream(IStream *pStream, IMFByteStream **ppByteStream)
{
    HMODULE hMfplat = LoadLibrary(TEXT("mfplat.dll"));
    FARPROC fpAddress = NULL;

    if (hMfplat == NULL) {
        return E_FAIL;
    }

    fpAddress = GetProcAddress(hMfplat, "MFCreateMFByteStreamOnStream");

    if (fpAddress == NULL) {
        return E_FAIL;
    }

    return ((MFCreateMFByteStreamOnStream_t) fpAddress)(pStream, ppByteStream);
}

#endif /* __MINGW32__ */

/***********************************************************************
 * Audio Callback
 ***********************************************************************/

static STDMETHODIMP_(ULONG)
AudioCallback_AddRef(IMFPMediaPlayerCallback *pThis)
{
    struct _AUDIOCALLBACK *pCallback = (struct _AUDIOCALLBACK*) pThis;
    return InterlockedIncrement(&pCallback->lRefCount);
}

static STDMETHODIMP_(ULONG)
AudioCallback_Release(IMFPMediaPlayerCallback *pThis)
{
    ULONG ulCount;
    struct _AUDIOCALLBACK *pCallback = (struct _AUDIOCALLBACK*) pThis;
    
    ulCount = InterlockedDecrement(&pCallback->lRefCount);

    if (ulCount == 0) {
        HeapFree(GetProcessHeap(), 0, pCallback);
    }

    return ulCount;
}

static STDMETHODIMP
AudioCallback_QueryInterface(IMFPMediaPlayerCallback *pThis,
                             REFIID riid, VOID **ppv)
{
    if (IsEqualIID(riid, &IID_IMFPMediaPlayerCallback) ||
        IsEqualIID(riid, &IID_IUnknown)) 
    {
        *ppv = pThis;
        AudioCallback_AddRef(pThis);
        return S_OK;
    }
    
    *ppv = NULL;
    return E_NOINTERFACE;
}

static STDMETHODIMP_(VOID)
AudioCallback_OnMediaPlayerEvent(IMFPMediaPlayerCallback *pThis,
                                 MFP_EVENT_HEADER *pEventHeader)
{
    struct _AUDIOCALLBACK *pCallback = (struct _AUDIOCALLBACK*) pThis;

    if (pEventHeader->eEventType == MFP_EVENT_TYPE_PLAYBACK_ENDED) {
        if (pCallback->pAudio->bLoop == TRUE) {
            Audio_PlayAsync(pCallback->pAudio);
        } else {
            pCallback->pAudio->bPlaying = FALSE;
        }
    }
}

static IMFPMediaPlayerCallbackVtbl g_IMFPMediaPlayerCallbackVtbl = {
    .AddRef = AudioCallback_AddRef,
    .OnMediaPlayerEvent = AudioCallback_OnMediaPlayerEvent,
    .QueryInterface = AudioCallback_QueryInterface,
    .Release = AudioCallback_Release
};

static IMFPMediaPlayerCallback *AudioCallback_Create(PAUDIO pAudio)
{
    struct _AUDIOCALLBACK *pCallback = NULL;

    pCallback = HeapAlloc(
        GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(struct _AUDIOCALLBACK));

    if (pCallback == NULL) {
        return NULL;
    }

    pCallback->iface.lpVtbl = &g_IMFPMediaPlayerCallbackVtbl;
    pCallback->lRefCount = 1;
    pCallback->pAudio = pAudio;

    return (IMFPMediaPlayerCallback*) pCallback;
}

/***********************************************************************
 * Audio
 ***********************************************************************/

PAUDIO Audio_LoadFromResource(HINSTANCE hInstance,
                              LPCTSTR lpszName,
                              LPCTSTR lpszType)
{
    PAUDIO pAudio = NULL;
    LPVOID pResourceData = NULL;
    DWORD dwResourceSize = 0;
    IStream *pStream = NULL;
    IMFSourceResolver *pResolver = NULL;
    IMFPMediaItem *pMediaItem = NULL;
    IMFByteStream *pByteStream = NULL;
    IUnknown *pSourceUnk = NULL;
    MF_OBJECT_TYPE objectType = MF_OBJECT_INVALID;
    HRESULT hResult = S_OK;

    pResourceData = LoadResourceToMemory(
        hInstance, lpszName, lpszType, &dwResourceSize);
    
    if (pResourceData == NULL) {
        return NULL;
    }

    pAudio = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(AUDIO));

    if (pAudio == NULL) {
        return NULL;
    }

    pAudio->bLoop = FALSE;

    pStream = SHCreateMemStream((const BYTE*) pResourceData, dwResourceSize);

    if (pStream == NULL) {
        goto cleanup;
    }

    hResult = MFCreateMFByteStreamOnStream(pStream, &pByteStream);

    if (FAILED(hResult)) {
        goto cleanup;
    }

    hResult = MFCreateSourceResolver(&pResolver);

    if (FAILED(hResult)) {
        goto cleanup;
    }

    hResult = pResolver->lpVtbl->CreateObjectFromByteStream(
        pResolver,
        pByteStream,
        NULL,
        MF_RESOLUTION_MEDIASOURCE |
        MF_RESOLUTION_CONTENT_DOES_NOT_HAVE_TO_MATCH_EXTENSION_OR_MIME_TYPE,
        NULL,
        &objectType,
        &pSourceUnk);
    
    if (FAILED(hResult)) {
        goto cleanup;
    }
    
    hResult = MFPCreateMediaPlayer(
        NULL, FALSE, 0, AudioCallback_Create(pAudio),
        NULL, &pAudio->pPlayer);
    
    if (FAILED(hResult)) {
        goto cleanup;
    }
    
    hResult = pAudio->pPlayer->lpVtbl->CreateMediaItemFromObject(
        pAudio->pPlayer,
        pSourceUnk,
        TRUE,
        0,
        &pMediaItem);
    
    if (FAILED(hResult)) {
        goto cleanup;
    }

    hResult = pAudio->pPlayer->lpVtbl->SetMediaItem(
        pAudio->pPlayer, pMediaItem);

    if (FAILED(hResult)) {
        goto cleanup;
    }

cleanup:
    if (pResolver != NULL) {
        pResolver->lpVtbl->Release(pResolver);
    }

    if (pSourceUnk != NULL) {
        pSourceUnk->lpVtbl->Release(pSourceUnk);
    }

    if (pMediaItem != NULL) {
        pMediaItem->lpVtbl->Release(pMediaItem);
    }

    if (pByteStream != NULL) {
        pByteStream->lpVtbl->Release(pByteStream);
    }

    if (pStream != NULL) {
        pStream->lpVtbl->Release(pStream);
    }

    if (FAILED(hResult) && pAudio != NULL) {
        HeapFree(GetProcessHeap(), 0, pAudio);
        return NULL;
    }

    return pAudio;
}

VOID Audio_PlayAsync(PAUDIO pAudio)
{
    if (pAudio != NULL && pAudio->bPlaying == FALSE) {
        pAudio->bPlaying = TRUE;
        pAudio->pPlayer->lpVtbl->Play(pAudio->pPlayer);
    }
}

VOID Audio_Stop(PAUDIO pAudio)
{
    if (pAudio != NULL && pAudio->bPlaying == TRUE) {
        pAudio->bPlaying = FALSE;
        pAudio->pPlayer->lpVtbl->Stop(pAudio->pPlayer);
    }
}

BOOL Audio_IsPlaying(CONST PAUDIO pAudio)
{
    return (pAudio != NULL) ? pAudio->bPlaying : FALSE;
}

VOID Audio_SetLoop(PAUDIO pAudio, BOOL bLoop)
{
    if (pAudio != NULL) {
        pAudio->bLoop = bLoop;
    }
}

VOID Audio_Destroy(PAUDIO pAudio)
{
    if (pAudio == NULL) {
        return;
    }

    pAudio->pPlayer->lpVtbl->Shutdown(pAudio->pPlayer);
    pAudio->pPlayer->lpVtbl->Release(pAudio->pPlayer);

    HeapFree(GetProcessHeap(), 0, pAudio);
}
