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

#include "application.h"

#include <windowsx.h>
#include <dwmapi.h>

#include "safemem.h"

#include "resource.h"

#define FINGERPOINTER_CLASSNAME     TEXT("FingerPointerClass")

#define UM_TRAYICON                 (WM_USER + 1)

#define HK_TOGGLE_VISIBILITY        1
#define HK_TOGGLE_MARKER            2

////////////////////////////////////////////////////////////////////////////
// OpenUrl
////////////////////////////////////////////////////////////////////////////

static VOID OpenUrl(LPCTSTR lpszUrl)
{
    if (lpszUrl == NULL || lpszUrl[0] == TEXT('\0')) {
        return;
    }

    ShellExecute(NULL, TEXT("open"), lpszUrl, NULL, NULL, SW_SHOWNORMAL);
}

static VOID OpenUrl(HINSTANCE hInstance, UINT uResourceId)
{
    TCHAR szBuffer[MAX_PATH];
    LoadString(hInstance, uResourceId, szBuffer, MAX_PATH);
    OpenUrl(szBuffer);
}

////////////////////////////////////////////////////////////////////////////
// Application
////////////////////////////////////////////////////////////////////////////

Application::Application()
    : _hWnd(NULL),
      _hInstance(NULL),
      _pRenderTarget(NULL),
      _pFactory(NULL),
      _bShow(FALSE)
{
}

HRESULT Application::Initialize(HINSTANCE hInstance)
{
    WNDCLASSEX  wcex = {0};
    MARGINS     margins = {-1};
    HICON       hIcon = NULL;

    hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));

    wcex.cbSize        = sizeof(WNDCLASSEX);
    wcex.style         = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc   = WndProc;
    wcex.cbClsExtra    = 0;
    wcex.cbWndExtra    = sizeof(LONG_PTR);
    wcex.hInstance     = hInstance;
    wcex.hbrBackground = NULL;
    wcex.lpszMenuName  = NULL;
    wcex.hIcon         = hIcon;
    wcex.lpszClassName = FINGERPOINTER_CLASSNAME;

    if (RegisterClassEx(&wcex) == 0) {
        return S_FALSE;
    }

    _hWnd = CreateWindowEx(
        WS_EX_APPWINDOW | WS_EX_TOPMOST,
        FINGERPOINTER_CLASSNAME,
        NULL,
        WS_POPUP,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        GetSystemMetrics(SM_CXSCREEN),
        GetSystemMetrics(SM_CYSCREEN),
        NULL,
        NULL,
        hInstance,
        this);
    
    if (_hWnd != NULL) {
        DwmExtendFrameIntoClientArea(_hWnd, &margins);
    }

    return (_hWnd != NULL) ? S_OK : S_FALSE;
}

VOID Application::RunMessageLoop()
{
    FLOAT   fDelta = 0.0f;
    MSG     msg = {0};

    while (msg.message != WM_QUIT) {
        if (_bShow == TRUE) {
            if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
                DispatchMessage(&msg);
            } else {
                _timer.Tick();
    
                OnUpdate(_timer.GetDeltaTime());
                OnRender();
            }
        } else {
            if (GetMessage(&msg, NULL, 0, 0) > 0) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
    }
}

VOID Application::ToggleWindowVisibility()
{
    _bShow = !_bShow;

    ShowWindow(_hWnd, (_bShow == TRUE) ? SW_SHOW : SW_HIDE);
    UpdateWindow(_hWnd);
}

////////////////////////////////////////////////////////////////////////////
// Render
////////////////////////////////////////////////////////////////////////////

VOID Application::OnRender()
{
    _pRenderTarget->BeginDraw();
    _pRenderTarget->Clear();
    _pointer.Draw(_pRenderTarget);
    _pRenderTarget->EndDraw();
}

VOID Application::OnUpdate(FLOAT fDelta)
{
    _pointer.Update(fDelta);
}

////////////////////////////////////////////////////////////////////////////
// Messages
////////////////////////////////////////////////////////////////////////////

LRESULT Application::OnCreate(WPARAM wParam, LPARAM lParam)
{
    D2D1_RENDER_TARGET_PROPERTIES       renderTargetProps;
    D2D1_HWND_RENDER_TARGET_PROPERTIES  hwndRenderTargetProps;
    D2D1_PIXEL_FORMAT                   pixelFormat;
    RECT                                rc;
    D2D1_SIZE_F                         pointerSize;
    HRESULT                             hResult = S_OK;

    hResult = D2D1CreateFactory(
        D2D1_FACTORY_TYPE_SINGLE_THREADED,
        &_pFactory);
    
    if (FAILED(hResult)) {
        return -1;
    }

    GetClientRect(_hWnd, &rc);

    pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
    pixelFormat.format    = DXGI_FORMAT_B8G8R8A8_UNORM;
    
    renderTargetProps = D2D1::RenderTargetProperties(
        D2D1_RENDER_TARGET_TYPE_DEFAULT,
        pixelFormat);

    hwndRenderTargetProps = D2D1::HwndRenderTargetProperties(
        _hWnd,
        D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top));

    hResult = _pFactory->CreateHwndRenderTarget(
        renderTargetProps,
        hwndRenderTargetProps,
        &_pRenderTarget);

    if (FAILED(hResult)) {
        goto destroy;
    }

    hResult = _pointer.InitializeResources(_pRenderTarget, _hInstance);

    if (FAILED(hResult)) {
        goto destroy;
    }

    pointerSize = _pointer.GetSize();

    _pointer.SetPosition(D2D1::Point2F(
        (((FLOAT)(rc.right - rc.left)) - pointerSize.width)  / 2.0f,
        (((FLOAT)(rc.bottom - rc.top)) - pointerSize.height) / 2.0f
    ));

    RegisterHotKey(
        _hWnd,
        HK_TOGGLE_VISIBILITY,
        MOD_ALT | MOD_NOREPEAT,
        0x48 /* H */);
    
    RegisterHotKey(
        _hWnd,
        HK_TOGGLE_MARKER,
        MOD_ALT | MOD_NOREPEAT,
        0x4D /* M */);

    if (_trayIcon.Add(_hWnd, UM_TRAYICON, 1001) == FALSE) {
        goto destroy;
    }

    return 0;

destroy:
    DestroyWindow(_hWnd);
    return S_FALSE;
}

LRESULT Application::OnTrayIcon(WPARAM wParam, LPARAM lParam)
{
    HMENU hMenu = NULL;
    HMENU hMenuTrackPopup = NULL;
    POINT pt;

    hMenu = LoadMenu(_hInstance, MAKEINTRESOURCE(IDM_MENU_MAIN));
    hMenuTrackPopup = GetSubMenu(hMenu, 0);

    if (hMenu == NULL) {
        return S_FALSE;
    }
    
    switch (lParam) {
        case WM_RBUTTONUP:
            GetCursorPos(&pt);
            TrackPopupMenuEx(
                hMenuTrackPopup,
                TPM_RIGHTBUTTON,
                pt.x,
                pt.y,
                _hWnd,
                NULL);
            DestroyMenu(hMenu);
            break;
        
        case WM_LBUTTONUP:
            ToggleWindowVisibility();
            break;
    }

    return 0;
}

LRESULT Application::OnMouseWheel(WPARAM wParam, LPARAM lParam)
{
    SHORT zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
    FLOAT fScale = _pointer.GetScale();

    _pointer.SetScale(fScale + ((zDelta > 0) ? -0.05f : 0.05f));
    return 0;
}

LRESULT Application::OnMouseMove(WPARAM wParam, LPARAM lParam)
{
    D2D1_POINT_2F   pointerPosition;
    D2D1_SIZE_F     pointerSize;
    D2D1_POINT_2F   position;
    INT             iCenterX, iCenterY;
    INT             iDeltaX, iDeltaY;
    INT             iMinX, iMinY, iMaxX, iMaxY;
    POINT           ptCenter;
    RECT            rc;

    pointerSize = _pointer.GetSize();
    pointerPosition = _pointer.GetPosition();

    GetClientRect(_hWnd, &rc);

    iCenterX = (rc.right - rc.left) / 2;
    iCenterY = (rc.bottom - rc.top) / 2;

    iDeltaX = GET_X_LPARAM(lParam) - iCenterX;
    iDeltaY = GET_Y_LPARAM(lParam) - iCenterY;

    position.x = pointerPosition.x + (FLOAT) iDeltaX;
    position.y = pointerPosition.y + (FLOAT) iDeltaY;

    iMinX = -pointerSize.width;
    iMinY = -pointerSize.height;
    iMaxX = (rc.right - rc.left) + (-iMinX);
    iMaxY = (rc.bottom - rc.top) + (-iMinY);

    position.x = fmaxf(iMinX, fminf(position.x, iMaxX));
    position.y = fmaxf(iMinY, fminf(position.y, iMaxY));

    _pointer.SetPosition(position);

    ptCenter.x = iCenterX;
    ptCenter.y = iCenterY;

    ClientToScreen(_hWnd, &ptCenter);

    SetCursorPos(ptCenter.x, ptCenter.y);
    SetCursor(NULL);

    return 0;
}

LRESULT Application::OnLeftButtonDown(WPARAM wParam, LPARAM lParam)
{
    _pointer.OnPress();
    return 0;
}

LRESULT Application::OnLeftButtonUp(WPARAM wParam, LPARAM lParam)
{
    _pointer.OnRelease();
    return 0;
}

LRESULT Application::OnHotkey(WPARAM wParam, LPARAM lParam)
{
    switch (wParam) {
        case HK_TOGGLE_VISIBILITY:
            ToggleWindowVisibility();
            break;
        case HK_TOGGLE_MARKER:
            _pointer.ToggleMarker();
            break;
    }
    return 0;
}

LRESULT Application::OnCommand(WPARAM wParam, LPARAM lParam)
{
    switch (LOWORD(wParam)) {
        case IDM_ITEM_SHOW:
            ToggleWindowVisibility();
            break;
        case IDM_ITEM_SOURCE_CODE:
            OpenUrl(_hInstance, IDS_GITHUB_URL);
            break;
        case IDM_ITEM_TELEGRAM:
            OpenUrl(_hInstance, IDS_TELEGRAM_URL);
            break;
        case IDM_ITEM_TIKTOK:
            OpenUrl(_hInstance, IDS_TIKTOK_URL);
            break;
        case IDM_ITEM_EXIT:
            DestroyWindow(_hWnd);
            break;
    }

    return 0;
}

LRESULT Application::OnDestroy(WPARAM wParam, LPARAM lParam)
{
    SafeRelease(&_pRenderTarget);
    SafeRelease(&_pFactory);

    PostQuitMessage(0);
    return 0;
}

////////////////////////////////////////////////////////////////////////////
// WinProc
////////////////////////////////////////////////////////////////////////////

LRESULT CALLBACK Application::WndProc(
    HWND hWnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam)
{
    Application*    pThis = NULL;
    LPCREATESTRUCT  pcs = NULL;

    if (uMsg == WM_CREATE) {
        pThis = (Application*) ((LPCREATESTRUCT) lParam)->lpCreateParams;

        pThis->_hWnd = hWnd;
        pThis->_hInstance = GetWindowInstance(hWnd);

        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR) pThis);
    } else {
        pThis = (Application*) GetWindowLongPtr(hWnd, GWLP_USERDATA);
    }

    switch (uMsg) {
        case WM_CREATE:
            return pThis->OnCreate(wParam, lParam);
        case UM_TRAYICON:
            return pThis->OnTrayIcon(wParam, lParam);
        case WM_MOUSEWHEEL:
            return pThis->OnMouseWheel(wParam, lParam);
        case WM_MOUSEMOVE:
            return pThis->OnMouseMove(wParam, lParam);
        case WM_LBUTTONDOWN:
            return pThis->OnLeftButtonDown(wParam, lParam);
        case WM_LBUTTONUP:
            return pThis->OnLeftButtonUp(wParam, lParam);
        case WM_HOTKEY:
            return pThis->OnHotkey(wParam, lParam);
        case WM_COMMAND:
            return pThis->OnCommand(wParam, lParam);
        case WM_DESTROY:
            return pThis->OnDestroy(wParam, lParam);
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
