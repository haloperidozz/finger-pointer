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

#include <Windows.h>

#define IDI_ICON                100

#define IDR_POINTER_PNG         200
#define IDR_EFFECT_WAV          201
#define IDR_EFFECT_MOVE_WAV     202

#define IDS_TITLE               300
#define IDS_GITHUB_URL          301
#define IDS_TELEGRAM_URL        302
#define IDS_TIKTOK_URL          303
#define IDS_NOTIFY_TITLE        304
#define IDS_NOTIFY_INFO         305

#define IDM_MENU_MAIN           400

#define IDM_ITEM_SHOW           500
#define IDM_ITEM_SOURCE_CODE    501
#define IDM_ITEM_TIKTOK         502
#define IDM_ITEM_TELEGRAM       503
#define IDM_ITEM_EXIT           504

////////////////////////////////////////////////////////////////////////////

#ifndef RC_INVOKED

#ifndef __RESOURCE_H
#define __RESOURCE_H

#include <Shlwapi.h>

static LPVOID LoadResourceToMemory(
    HINSTANCE   hInstance,
    LPCTSTR     lpszName,
    LPCTSTR     lpszType,
    LPDWORD     lpdwSize)
{
    HRSRC   hResource = NULL;
    HGLOBAL hResourceData = NULL;
    DWORD   dwResourceSize;

    if (lpszName == NULL || lpszType == NULL) {
        return NULL;
    }

    hResource = FindResource(hInstance, lpszName, lpszType);

    if (hResource == NULL) {
        return NULL;
    }

    dwResourceSize = SizeofResource(hInstance, hResource);

    if (dwResourceSize <= 0) {
        return NULL;
    }

    hResourceData = LoadResource(hInstance, hResource);

    if (hResourceData == NULL) {
        return NULL;
    }

    *lpdwSize = dwResourceSize;

    return LockResource(hResourceData);
}

static IStream* CreateIStreamFromResource(
    HINSTANCE   hInstance,
    LPCTSTR     lpszName,
    LPCTSTR     lpszType)
{
    BYTE* pbResourceData = NULL;
    DWORD dwResourceSize = 0;

    if (lpszName == NULL || lpszType == NULL) {
        return NULL;
    }

    pbResourceData = (BYTE*) LoadResourceToMemory(
        hInstance,
        lpszName,
        lpszType,
        &dwResourceSize);
    
    if (pbResourceData == NULL) {
        return NULL;
    }

    return SHCreateMemStream(pbResourceData, dwResourceSize);
}

#endif // __RESOURCE_H

#endif // RC_INVOKED

////////////////////////////////////////////////////////////////////////////
