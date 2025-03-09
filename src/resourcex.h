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

#ifndef __RESOURCEX_H
#define __RESOURCEX_H

#include <Windows.h>
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

#endif // __RESOURCEX_H