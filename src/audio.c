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

/***********************************************************************
 * HACK: Temporary attempt to fix
 * "undefined reference to 'IID_IMFPMediaPlayerCallback'" on MinGW
 ***********************************************************************/

#if __MINGW32__
#include <initguid.h>
#endif /* __MINGW32__ */

#include <mfapi.h>
#include <mfplay.h>

#if __MINGW32__
#undef INITGUID
#endif /* __MINGW32__ */

#include <windowsx.h>
#include <shlwapi.h>

#include "helper.h"

#if __MINGW32__

/***********************************************************************
 * MFCreateMFByteStreamOnStream
 *
 * HACK: For reasons unknown to me, the mfidl.h header in MinGW lacks
 * a definition for the MFCreateMFByteStreamOnStream function.
 ***********************************************************************/

typedef HRESULT (*MFCreateMFByteStreamOnStream_t)(IStream*, IMFByteStream**);

static HRESULT
MFCreateMFByteStreamOnStream(IStream *pStream, IMFByteStream **ppByteStream)
{
    HMODULE hModule = LoadLibraryA("mfplat.dll");
    FARPROC fpProc;

    if (hModule == NULL) {
        return E_FAIL;
    }
    
    fpProc = GetProcAddress(hModule, "MFCreateMFByteStreamOnStream");

    if (fpProc == NULL) {
        return E_FAIL;
    }

    return ((MFCreateMFByteStreamOnStream_t) fpProc)(pStream, ppByteStream);
}

#endif /* __MINGW32__ */

struct _AUDIO {
    IMFPMediaPlayer    *pPlayer;
    BOOL                bLoop;
    BOOL                bPlaying;
};

/***********************************************************************
 * Audio Callback
 ***********************************************************************/

struct _AUDIOCALLBACK {
    IMFPMediaPlayerCallback iface;
    LONG                    lRefCount;
    PAUDIO                  pAudio; 
};

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
        SafeFree(pCallback);
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

    pCallback = SafeAllocSizeof(struct _AUDIOCALLBACK);

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

static IStream* CreateIStreamFromResource(
    HINSTANCE hInstance,
    LPCTSTR lpszName,
    LPCTSTR lpszType)
{
    BYTE* pbResourceData = NULL;
    DWORD dwResourceSize = 0;

    pbResourceData = LoadResourceToMemory(
        hInstance, lpszName, lpszType, &dwResourceSize);
    
    if (pbResourceData == NULL) {
        return NULL;
    }

    return SHCreateMemStream(pbResourceData, dwResourceSize);
}

PAUDIO Audio_LoadFromResource(HINSTANCE hInstance,
                              LPCTSTR lpszName,
                              LPCTSTR lpszType)
{
    PAUDIO pAudio = NULL;
    IStream *pIStream = NULL;
    IMFSourceResolver *pResolver = NULL;
    IMFPMediaItem *pMediaItem = NULL;
    IMFByteStream *pByteStream = NULL;
    IUnknown *pSourceUnk = NULL;
    MF_OBJECT_TYPE objectType = MF_OBJECT_INVALID;
    HRESULT hResult = S_OK;

    pIStream = CreateIStreamFromResource(hInstance, lpszName, lpszType);
    
    if (pIStream == NULL) {
        return NULL;
    }

    pAudio = SafeAllocSizeof(AUDIO);

    if (pAudio == NULL) {
        return NULL;
    }

    pAudio->bLoop = FALSE;

    hResult = MFCreateMFByteStreamOnStream(pIStream, &pByteStream);

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
    COM_SafeRelease(pResolver);
    COM_SafeRelease(pSourceUnk);
    COM_SafeRelease(pMediaItem);
    COM_SafeRelease(pByteStream);
    COM_SafeRelease(pIStream);

    if (FAILED(hResult)) {
        SafeFree(pAudio);
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

    COM_SafeRelease(pAudio->pPlayer);

    SafeFree(pAudio);
}
