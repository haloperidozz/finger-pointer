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
    : _hWnd(NULL)
{
}

TrayIcon::~TrayIcon()
{
    Delete();
}

BOOL TrayIcon::Add(HWND hWnd, UINT uMessage, UINT uId)
{
    NOTIFYICONDATA nid;

    if (_hWnd != NULL || hWnd == NULL) {
        return FALSE;
    }

    _hWnd = hWnd;
    _uMessage = uMessage;
    _uId = uId;

    nid.cbSize           = sizeof(NOTIFYICONDATA);
    nid.hWnd             = _hWnd;
    nid.uID              = _uId;
    nid.uFlags           = NIF_MESSAGE | NIF_ICON | NIF_TIP;
    nid.uCallbackMessage = _uMessage;

    GetWindowText(_hWnd, nid.szTip, ARRAYSIZE(nid.szTip));

    nid.hIcon = (HICON) GetClassLongPtr(_hWnd, GCLP_HICON);

    return Shell_NotifyIcon(NIM_ADD, &nid);
}

BOOL TrayIcon::Delete()
{
    NOTIFYICONDATA nid;

    if (_hWnd == NULL) {
        return FALSE;
    }

    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd   = _hWnd;
    nid.uID    = _uId;

    if (Shell_NotifyIcon(NIM_DELETE, &nid) == TRUE) {
        _hWnd = NULL;
        return TRUE;
    }

    return FALSE;
}

BOOL TrayIcon::UpdateIcon(HICON hIcon)
{
    NOTIFYICONDATA nid;

    if (_hWnd == NULL) {
        return FALSE;
    }

    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd   = _hWnd;
    nid.uID    = _uId;
    nid.uFlags = NIF_ICON;
    nid.hIcon  = hIcon;

    return Shell_NotifyIcon(NIM_MODIFY, &nid);
}

BOOL TrayIcon::UpdateTooltip(LPCTSTR szTooltip)
{
    NOTIFYICONDATA nid;

    if (_hWnd == NULL) {
        return FALSE;
    }

    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd   = _hWnd;
    nid.uID    = _uId;
    nid.uFlags = NIF_TIP;

    lstrcpyn(nid.szTip, szTooltip, ARRAYSIZE(nid.szTip));

    return Shell_NotifyIcon(NIM_MODIFY, &nid);
}

BOOL TrayIcon::ShowNotification(LPCTSTR szInfo, LPCTSTR szTitle)
{
    NOTIFYICONDATA nid;

    if (_hWnd == NULL) {
        return FALSE;
    }

    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd   = _hWnd;
    nid.uID    = _uId;
    nid.uFlags = NIF_INFO;

    lstrcpyn(nid.szInfo, szInfo, ARRAYSIZE(nid.szInfo));
    lstrcpyn(nid.szInfoTitle, szTitle, ARRAYSIZE(nid.szInfoTitle));

    return Shell_NotifyIcon(NIM_MODIFY, &nid);
}
