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

#if __MINGW32__

/***********************************************************************
 * HACK: Temporary attempt to fix
 * "undefined reference to 'IID_IMFPMediaPlayerCallback'" on MinGW
 ***********************************************************************/

#include <initguid.h>

#endif /* __MINGW32__ */

#include <windows.h>

typedef struct _AUDIO AUDIO, *PAUDIO;

PAUDIO Audio_LoadFromResource(HINSTANCE hInstance,
                              LPCTSTR lpszName,
                              LPCTSTR lpszType);

VOID Audio_PlayAsync(PAUDIO pAudio);

VOID Audio_Stop(PAUDIO pAudio);

BOOL Audio_IsPlaying(CONST PAUDIO pAudio);

VOID Audio_SetLoop(PAUDIO pAudio, BOOL bLoop);

VOID Audio_Destroy(PAUDIO pAudio);

#endif /* __AUDIO_H */