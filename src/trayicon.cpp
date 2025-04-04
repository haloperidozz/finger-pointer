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

TrayIcon::TrayIcon()
{
    ZeroMemory(&_nid, sizeof(NOTIFYICONDATA));
}

TrayIcon::~TrayIcon()
{
    Delete();
}

BOOL TrayIcon::Add(HWND hWnd, UINT uMessage, UINT uId)
{
    if (_nid.hWnd != NULL || hWnd == NULL) {
        return FALSE;
    }

    _nid.cbSize           = sizeof(NOTIFYICONDATA);
    _nid.hWnd             = hWnd;
    _nid.uCallbackMessage = uMessage;
    _nid.uID              = uId;
    _nid.uFlags           = NIF_MESSAGE | NIF_ICON | NIF_TIP;

    GetWindowText(_nid.hWnd, _nid.szTip, ARRAYSIZE(_nid.szTip));

    _nid.hIcon = (HICON) GetClassLongPtr(_nid.hWnd, GCLP_HICON);

    return Shell_NotifyIcon(NIM_ADD, &_nid);
}

BOOL TrayIcon::Delete()
{
    if (_nid.hWnd == NULL) {
        return FALSE;
    }

    if (Shell_NotifyIcon(NIM_DELETE, &_nid) == FALSE) {
        return FALSE;
    }
    
    ZeroMemory(&_nid, sizeof(NOTIFYICONDATA));
    
    return TRUE;
}

BOOL TrayIcon::UpdateIcon(HICON hIcon)
{
    if (_nid.hWnd == NULL) {
        return FALSE;
    }

    _nid.uFlags = NIF_ICON;
    _nid.hIcon  = hIcon;

    return Shell_NotifyIcon(NIM_MODIFY, &_nid);
}

BOOL TrayIcon::UpdateTooltip(LPCTSTR szTooltip)
{

    if (_nid.hWnd == NULL) {
        return FALSE;
    }

    _nid.uFlags = NIF_TIP;

    lstrcpyn(_nid.szTip, szTooltip, ARRAYSIZE(_nid.szTip));

    return Shell_NotifyIcon(NIM_MODIFY, &_nid);
}

BOOL TrayIcon::ShowNotification(LPCTSTR szInfo, LPCTSTR szTitle)
{
    if (_nid.hWnd == NULL) {
        return FALSE;
    }

    _nid.uFlags      = NIF_INFO;
    _nid.dwInfoFlags = NIIF_NONE;

    lstrcpyn(_nid.szInfo, szInfo, ARRAYSIZE(_nid.szInfo));
    lstrcpyn(_nid.szInfoTitle, szTitle, ARRAYSIZE(_nid.szInfoTitle));

    return Shell_NotifyIcon(NIM_MODIFY, &_nid);
}