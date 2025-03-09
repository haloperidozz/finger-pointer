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

#ifndef __AUDIO_H
#define __AUDIO_H

#include <Windows.h>
#include <mfplay.h>

class Audio;

class AudioCallback : public IMFPMediaPlayerCallback
{
public:
    AudioCallback(Audio* pAudio);

    STDMETHODIMP QueryInterface(REFIID riid, VOID** ppV);

    STDMETHODIMP_(ULONG) AddRef();

    STDMETHODIMP_(ULONG) Release();

    STDMETHODIMP_(VOID)
    OnMediaPlayerEvent(MFP_EVENT_HEADER* pEventHeader);

private:
    LONG            _lRefCount;
    Audio*          _pAudio; 
};

class Audio
{
    friend class AudioCallback;

public:
    static HRESULT CreateAudioFromSourceObject(
        IUnknown*   pSourceUnk,
        Audio**     ppAudio);

    static HRESULT CreateAudioFromResource(
        HINSTANCE   hInstance,
        LPCTSTR     lpszName,
        LPCTSTR     lpszType,
        Audio**     ppAudio);
        
    ~Audio();

    VOID Play();

    VOID Stop();

    VOID SetLoop(BOOL bLoop);
    BOOL GetLoop() CONST;

    BOOL IsPlaying() CONST;

private:
    Audio();

    HRESULT InitializePlayer();

    IMFPMediaPlayer*    _pPlayer;
    BOOL                _bLoop;
    BOOL                _bPlaying;
};

#endif // __AUDIO_H