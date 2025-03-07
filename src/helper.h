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

#ifndef __HELPER_H
#define __HELPER_H

#include <windows.h>

#define COM_SafeRelease(pInterface)                         \
    do {                                                    \
        if ((pInterface) != NULL) {                         \
            (pInterface)->lpVtbl->Release(pInterface);      \
            (pInterface) = NULL;                            \
        }                                                   \
    } while (FALSE)

#define SafeAlloc(dwSize)                                   \
    HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwSize)

#define SafeAllocSizeof(type) SafeAlloc(sizeof(type))

#define SafeFree(lpMemory)                                  \
    do {                                                    \
        if ((lpMemory) != NULL) {                           \
            HeapFree(GetProcessHeap(), 0, (lpMemory));      \
            (lpMemory) = NULL;                              \
        }                                                   \
    } while (FALSE)

static LPVOID LoadResourceToMemory(
    HINSTANCE hInstance,
    LPCTSTR lpszName,
    LPCTSTR lpszType,
    LPDWORD lpdwSize)
{
    HRSRC   hResource = NULL;
    HGLOBAL hResourceData = NULL;
    DWORD   dwResourceSize;

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

#endif /* __HELPER_H */