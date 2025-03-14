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

#ifndef __APPLICATION_H
#define __APPLICATION_H

#include <Windows.h>
#include <d2d1.h>

#include "sprite.h"
#include "audio.h"
#include "tweener.h"
#include "timer.h"
#include "pointer.h"
#include "trayicon.h"

class Application {
public:
    Application();

    HRESULT Initialize(HINSTANCE hInstance);

    VOID RunMessageLoop();

private:
    static LRESULT CALLBACK WndProc(
        HWND hWnd,
        UINT uMsg,
        WPARAM wParam,
        LPARAM lParam);

    VOID ToggleWindowVisibility();

    ///////////////////////////////////////////////////////////////

    VOID OnRender();

    VOID OnUpdate(FLOAT fDelta);

    ///////////////////////////////////////////////////////////////

    LRESULT OnCreate(WPARAM wParam, LPARAM lParam);

    LRESULT OnTrayIcon(WPARAM wParam, LPARAM lParam);

    LRESULT OnMouseWheel(WPARAM wParam, LPARAM lParam);

    LRESULT OnMouseMove(WPARAM wParam, LPARAM lParam);

    LRESULT OnLeftButtonDown(WPARAM wParam, LPARAM lParam);

    LRESULT OnLeftButtonUp(WPARAM wParam, LPARAM lParam);

    LRESULT OnHotkey(WPARAM wParam, LPARAM lParam);

    LRESULT OnCommand(WPARAM wParam, LPARAM lParam);

    LRESULT OnDestroy(WPARAM wParam, LPARAM lParam);

    ///////////////////////////////////////////////////////////////
    
    HWND                    _hWnd;
    HINSTANCE               _hInstance;
    ID2D1HwndRenderTarget*  _pRenderTarget;
    ID2D1Factory*           _pFactory;    
    Timer                   _timer;
    Pointer                 _pointer;
    TrayIcon                _trayIcon;
    BOOL                    _bShow;
};

#endif // __APPLICATION_H