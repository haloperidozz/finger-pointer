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

#include <Shlwapi.h>

#include "safemem.h"
#include "resource.h"

#if __MINGW32__

////////////////////////////////////////////////////////////////////////////
// MFCreateMFByteStreamOnStream
//
// HACK: For reasons unknown to me, the mfidl.h header in MinGW lacks
// a definition for the MFCreateMFByteStreamOnStream function.
////////////////////////////////////////////////////////////////////////////

typedef HRESULT (*MFCreateMFByteStreamOnStream_t)(IStream*, IMFByteStream**);

static HRESULT
MFCreateMFByteStreamOnStream(IStream* pStream, IMFByteStream** ppByteStream)
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

#endif // __MINGW32__

////////////////////////////////////////////////////////////////////////////
// AudioCallback
////////////////////////////////////////////////////////////////////////////

AudioCallback::AudioCallback(Audio* pAudio)
    : _pAudio(pAudio),
      _lRefCount(1)
{
}

STDMETHODIMP AudioCallback::QueryInterface(REFIID riid, VOID** ppV)
{
    static CONST QITAB qit[] = {
        QITABENT(AudioCallback, IMFPMediaPlayerCallback),
        { 0 },
    };

    return QISearch(this, qit, riid, ppV);
}

STDMETHODIMP_(ULONG) AudioCallback::AddRef()
{
    return InterlockedIncrement(&_lRefCount);
}

STDMETHODIMP_(ULONG) AudioCallback::Release()
{
    ULONG ulCount = InterlockedDecrement(&_lRefCount);

    if (ulCount == 0) {
        delete this;
        return 0;
    }

    return ulCount;
}

STDMETHODIMP_(VOID)
AudioCallback::OnMediaPlayerEvent(MFP_EVENT_HEADER* pEventHeader)
{
    if (pEventHeader == NULL) {
        return;
    }
    
    switch (pEventHeader->eEventType) {
        case MFP_EVENT_TYPE_PLAYBACK_ENDED: {
            if (_pAudio->_bLoop == TRUE) {
                _pAudio->Play();
            }
            break;
        }

        case MFP_EVENT_TYPE_STOP: {
            _pAudio->_bPlaying = FALSE;
            break;
        }

        case MFP_EVENT_TYPE_PLAY: {
            _pAudio->_bPlaying = TRUE;
            break;
        }
    }
}

////////////////////////////////////////////////////////////////////////////
// Audio
////////////////////////////////////////////////////////////////////////////

Audio::Audio()
    : _pPlayer(NULL),
      _bLoop(FALSE),
      _bPlaying(FALSE)
{
}

Audio::~Audio()
{
    if (_pPlayer == NULL) {
        return;
    }

    _pPlayer->Shutdown();
    SafeRelease(&_pPlayer);
}

VOID Audio::Play()
{
    if (_pPlayer == NULL) {
        return;
    }

    _pPlayer->Play();
}

VOID Audio::Stop()
{
    if (_pPlayer == NULL) {
        return;
    }

    _pPlayer->Stop();
}

VOID Audio::SetLoop(BOOL bLoop)
{
    _bLoop = bLoop;
}

BOOL Audio::GetLoop() CONST
{
    return _bLoop;
}

BOOL Audio::IsPlaying() CONST
{
    return _bPlaying;
}

HRESULT Audio::InitializePlayer()
{
    AudioCallback*  pCallback = new AudioCallback(this);
    HRESULT         hResult = S_OK;

    if (pCallback == NULL) {
        return E_OUTOFMEMORY;
    }

    hResult = MFPCreateMediaPlayer(
        NULL,
        FALSE,
        0,
        pCallback,
        NULL,
        &_pPlayer);

    if (FAILED(hResult)) {
        SafeRelease(&pCallback);
    }

    return hResult;
}

////////////////////////////////////////////////////////////////////////////

HRESULT Audio::CreateAudioFromSourceObject(
    IUnknown*   pSourceUnk,
    Audio**     ppAudio)
{
    Audio*          pAudio = NULL;
    IMFPMediaItem*  pMediaItem = NULL;
    HRESULT         hResult = S_OK;

    if (pSourceUnk == NULL || ppAudio == NULL) {
        return E_INVALIDARG;
    }

    pAudio = new Audio();

    if (pAudio == NULL) {
        return E_OUTOFMEMORY;
    }

    hResult = pAudio->InitializePlayer();

    if (FAILED(hResult)) {
        goto cleanup;
    }

    hResult = pAudio->_pPlayer->CreateMediaItemFromObject(
        pSourceUnk,
        TRUE,
        0,
        &pMediaItem);
    
    if (FAILED(hResult)) {
        goto cleanup;
    }
    
    hResult = pAudio->_pPlayer->SetMediaItem(pMediaItem);

cleanup:
    SafeRelease(&pMediaItem);

    if (SUCCEEDED(hResult)) {
        *ppAudio = pAudio;
    } else if (pAudio != NULL) {
        delete pAudio;
    }

    return hResult;
}

HRESULT Audio::CreateAudioFromResource(
    HINSTANCE   hInstance,
    LPCTSTR     lpszName,
    LPCTSTR     lpszType,
    Audio**     ppAudio)
{
    IStream*            pIStream = NULL;
    IMFSourceResolver*  pResolver = NULL;
    IMFByteStream*      pByteStream = NULL;
    IUnknown*           pSourceUnk = NULL;
    MF_OBJECT_TYPE      objectType = MF_OBJECT_INVALID;
    HRESULT             hResult = S_OK;

    pIStream = CreateIStreamFromResource(hInstance, lpszName, lpszType);
    
    if (pIStream == NULL) {
        return E_INVALIDARG;
    }

    hResult = MFCreateMFByteStreamOnStream(pIStream, &pByteStream);

    if (FAILED(hResult)) {
        goto cleanup;
    }

    hResult = MFCreateSourceResolver(&pResolver);

    if (FAILED(hResult)) {
        goto cleanup;
    }

    hResult = pResolver->CreateObjectFromByteStream(
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

    hResult = CreateAudioFromSourceObject(pSourceUnk, ppAudio);

cleanup:
    SafeRelease(&pResolver);
    SafeRelease(&pSourceUnk);
    SafeRelease(&pByteStream);
    SafeRelease(&pIStream);

    return hResult;
}