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

#ifndef __OPENURL_H
#define __OPENURL_H

#include <windows.h>

#define OU_DEFAULT      0x0
#define OU_RESOURCE     0x1

static VOID OpenUrl(LPCTSTR lpszUrl, DWORD dwFlags)
{
    TCHAR szBuffer[256];
    UINT uResourceId;

    if (dwFlags & OU_RESOURCE) {
        uResourceId = (UINT) (WORD) (ULONG_PTR) lpszUrl;
        LoadString(GetModuleHandle(NULL), uResourceId, szBuffer, 256);
        lpszUrl = szBuffer;
    }

    ShellExecute(NULL, TEXT("open"), lpszUrl, NULL, NULL, SW_SHOWNORMAL);
}

#endif /* __OPENURL_H */