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

#include "trayicon.h"

struct _TRAYICON {
    NOTIFYICONDATA   nid;
    HMENU            hMenu;
    HWND             hOwnerWindow;
    TRAYICONCALLBACK fnCallback;
    LPVOID           lpUserData;
};

TRAYICON TrayIcon_Create(HWND hOwnerWindow, UINT uCallbackMessage,
                         TRAYICONCALLBACK fnCallback,
                         LPVOID lpUserData)
{
    TRAYICON trayicon = NULL;

    if (hOwnerWindow == NULL) {
        return NULL;
    }
    
    trayicon = HeapAlloc(
        GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(struct _TRAYICON));

    if (trayicon == NULL) {
        return NULL;
    }
    
    trayicon->hOwnerWindow = hOwnerWindow;

    ZeroMemory(&(trayicon->nid), sizeof(NOTIFYICONDATA));

    trayicon->nid.cbSize           = sizeof(NOTIFYICONDATA);
    trayicon->nid.hWnd             = hOwnerWindow;
    trayicon->nid.uID              = 1;
    trayicon->nid.uFlags           = NIF_MESSAGE | NIF_ICON | NIF_TIP;
    trayicon->nid.uCallbackMessage = uCallbackMessage;

    GetWindowText(hOwnerWindow, trayicon->nid.szTip, 128);

    trayicon->nid.hIcon = (HICON) GetClassLongPtr(hOwnerWindow, GCLP_HICON);
    
    if (Shell_NotifyIcon(NIM_ADD, &(trayicon->nid)) == FALSE) {
        HeapFree(GetProcessHeap(), 0, trayicon);
        return NULL;
    }
    
    trayicon->lpUserData = lpUserData;
    trayicon->fnCallback = fnCallback;
    trayicon->hMenu = NULL;
    
    return trayicon;
}

VOID TrayIcon_AttachMenu(TRAYICON trayicon, HMENU hMenu)
{
    trayicon->hMenu = hMenu;
}

BOOL TrayIcon_HandleMessage(TRAYICON trayicon, UINT uMsg, LPARAM lParam)
{
    POINT pt;

    if (trayicon == NULL) {
        return FALSE;
    }

    if (uMsg != trayicon->nid.uCallbackMessage) {
        return FALSE;
    }

    switch (lParam) {
        case WM_RBUTTONUP: {
            if (trayicon->hMenu != NULL) {
                GetCursorPos(&pt);

                TrackPopupMenuEx(
                    trayicon->hMenu, TPM_RIGHTBUTTON, pt.x, pt.y,
                    trayicon->hOwnerWindow, NULL);
            }
            break;
        }
        
        case WM_LBUTTONUP: {
            if (trayicon->fnCallback != NULL) {
                trayicon->fnCallback(trayicon->lpUserData);
            }
            break;
        }
    }
    
    return TRUE;
}

VOID TrayIcon_Destroy(TRAYICON trayicon)
{
    if (trayicon == NULL) {
        return;
    }
    
    Shell_NotifyIcon(NIM_DELETE, &(trayicon->nid));

    HeapFree(GetProcessHeap(), 0, trayicon);
}