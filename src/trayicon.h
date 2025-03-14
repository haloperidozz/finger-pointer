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

#ifndef __TRAYICON_H
#define __TRAYICON_H

#include <Windows.h>
#include <shellapi.h>

class TrayIcon
{
public:
    TrayIcon();
    ~TrayIcon();

    BOOL Add(HWND hWnd, UINT uMessage, UINT uId);
    BOOL Delete();

    BOOL UpdateIcon(HICON hIcon);
    BOOL UpdateTooltip(LPCTSTR szTooltip);
    
    BOOL ShowNotification(LPCTSTR szInfo, LPCTSTR szTitle);

private:
    HWND    _hWnd;
    UINT    _uMessage;
    UINT    _uId;
};

#endif // __TRAYICON_H