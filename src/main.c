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

#include <windows.h>
#include <windowsx.h>
#include <math.h>
#include <dwmapi.h>
#include <tchar.h>

#include "sprite.h"
#include "trayicon.h"
#include "tweener.h"
#include "timer.h"

#include "resource.h"

#define FINGERPOINTER_CLASSNAME TEXT("FingerPointerClass")

#define UM_TRAYICON             (WM_USER + 1)

typedef struct _FINGERPOINTER {
    HWND hWnd;
    HINSTANCE hInstance;

    ID2D1Factory          *pFactory;
    ID2D1HwndRenderTarget *pRenderTarget;

    SPRITE                 spritePointer;
    TRAYICON               trayicon;
    TWEENER                tweener;
    TIMER                  timer;

    BOOL                   bInitialized;
    BOOL                   bShow;
} FINGERPOINTER, *PFINGERPOINTER;

/***********************************************************************
 * Misc
 ***********************************************************************/

/* https://easings.net/#easeOutCirc */
static FLOAT EaseOutCircEasing(FLOAT x)
{
    return sqrtf(1 - powf(x - 1, 2));
}

/***********************************************************************
 * Common
 ***********************************************************************/

static VOID FingerPointer_ToggleWindow(PFINGERPOINTER pFingerPointer)
{
    if (pFingerPointer == NULL || pFingerPointer->bInitialized == FALSE) {
        return;
    }

    if (pFingerPointer->bShow == FALSE) {
        ShowWindow(pFingerPointer->hWnd, SW_SHOW);
        UpdateWindow(pFingerPointer->hWnd);

        SetCursor(NULL);

        pFingerPointer->bShow = TRUE;
    } else {
        ShowWindow(pFingerPointer->hWnd, SW_HIDE);
        pFingerPointer->bShow = FALSE;

        ID2D1HwndRenderTarget_Flush(
            pFingerPointer->pRenderTarget, NULL, NULL);
    }
}

static VOID FingerPointer_Render(PFINGERPOINTER pFingerPointer)
{
    ID2D1RenderTarget *pRenderTarget = NULL;

    if (pFingerPointer == NULL || pFingerPointer->bInitialized == FALSE) {
        return;
    }

    ID2D1HwndRenderTarget_BeginDraw(pFingerPointer->pRenderTarget);

    ID2D1HwndRenderTarget_Clear(pFingerPointer->pRenderTarget, 0);
    ID2D1HwndRenderTarget_QueryInterface(pFingerPointer->pRenderTarget,
        &IID_ID2D1RenderTarget, (LPVOID*) &pRenderTarget);
    
    Sprite_Draw(pFingerPointer->spritePointer, pRenderTarget);
    
    ID2D1HwndRenderTarget_EndDraw(pFingerPointer->pRenderTarget, NULL, NULL);
}

static VOID FingerPointer_Update(PFINGERPOINTER pFingerPointer, FLOAT fDelta)
{
    FLOAT fAngle;

    if (pFingerPointer == NULL || pFingerPointer->bInitialized == FALSE) {
        return;
    }

    if (Tweener_Update(pFingerPointer->tweener, fDelta)) {
        
        fAngle = Tweener_GetValue(pFingerPointer->tweener);
        Sprite_SetRotation(pFingerPointer->spritePointer, fAngle);
    }
}

/***********************************************************************
 * Events
 ***********************************************************************/

static LRESULT
FingerPointer_OnLeftMouseButton(PFINGERPOINTER pFingerPointer, BOOL bPress)
{
    Tweener_Invert(pFingerPointer->tweener, bPress);

    if (bPress == FALSE) {
        PlaySound(NULL, NULL, 0);
    } else {
        PlaySound(MAKEINTRESOURCE(IDR_EFFECT_WAV),
            pFingerPointer->hInstance, SND_RESOURCE | SND_ASYNC);
    }

    return 0;
}

static LRESULT FingerPointer_OnKey(PFINGERPOINTER pFingerPointer,
                                   WPARAM wParam, LPARAM lParam, BOOL bDown)
{
    if (bDown == FALSE && wParam == VK_ESCAPE) {
        FingerPointer_ToggleWindow(pFingerPointer);
    }

    return 0;
}

static LRESULT FingerPointer_OnMouseMove(PFINGERPOINTER pFingerPointer,
                                         INT x, INT y)
{
    D2D1_POINT_2F position, center;

    center = Sprite_GetCenterPoint(pFingerPointer->spritePointer);

    position.x = ((FLOAT) x) - center.x;
    position.y = ((FLOAT) y) - center.y;

    Sprite_SetPosition(pFingerPointer->spritePointer, position);

    return 0;
}

static LRESULT FingerPointer_OnDestroy(PFINGERPOINTER pFingerPointer)
{
    if (pFingerPointer->timer != NULL) {
        Timer_Destroy(pFingerPointer->timer);
        pFingerPointer->timer = NULL;
    }

    if (pFingerPointer->tweener != NULL) {
        Tweener_Destroy(pFingerPointer->tweener);
        pFingerPointer->tweener = NULL;
    }

    if (pFingerPointer->trayicon != NULL) {
        TrayIcon_Destroy(pFingerPointer->trayicon);
        pFingerPointer->trayicon = NULL;
    }

    if (pFingerPointer->spritePointer != NULL) {
        Sprite_Destroy(pFingerPointer->spritePointer);
        pFingerPointer->spritePointer = NULL;
    }

    if (pFingerPointer->pRenderTarget != NULL) {
        ID2D1HwndRenderTarget_Release(pFingerPointer->pRenderTarget);
        pFingerPointer->pRenderTarget = NULL;
    }

    if (pFingerPointer->pFactory != NULL) {
        ID2D1Factory_Release(pFingerPointer->pFactory);
        pFingerPointer->pFactory = NULL;
    }
    
    PostQuitMessage(0);

    pFingerPointer->bInitialized = FALSE;
    return 0;
}

static LRESULT FingerPointer_OnHotkey(PFINGERPOINTER pFingerPointer,
                                      WPARAM wParam, LPARAM lParam)
{
    FingerPointer_ToggleWindow(pFingerPointer);
    return 0;
}

static LRESULT FingerPointer_OnCommand(PFINGERPOINTER pFingerPointer,
                                       WPARAM wParam, LPARAM lParam)
{
    TCHAR szBuffer[256];

    switch (LOWORD(wParam)) {
        case IDM_ITEM_SHOW:
            FingerPointer_ToggleWindow(pFingerPointer);
            break;
        case IDM_ITEM_SOURCE_CODE:
            LoadString(
                pFingerPointer->hInstance, IDS_GITHUB_URL, szBuffer, 256);
            ShellExecute(
                NULL, TEXT("open"), szBuffer, NULL, NULL, SW_SHOWNORMAL);
            break;
        case IDM_ITEM_EXIT:
            DestroyWindow(pFingerPointer->hWnd);
            break;
    }

    return 0;
}

static LRESULT FingerPointer_OnCreate(PFINGERPOINTER pFingerPointer)
{
    D2D1_RENDER_TARGET_PROPERTIES renderTargetProps;
    D2D1_HWND_RENDER_TARGET_PROPERTIES hwndRenderTargetProps;
    HRESULT hRes = S_OK;
    HMENU hMenu = NULL;
    RECT rc;

    hRes = D2D1CreateFactory(
        D2D1_FACTORY_TYPE_SINGLE_THREADED,
        &IID_ID2D1Factory, NULL, (LPVOID*) &(pFingerPointer->pFactory));
    
    if (FAILED(hRes)) {
        return -1;
    }

    GetClientRect(pFingerPointer->hWnd, &rc);

    renderTargetProps.dpiX     = 0.0f;
    renderTargetProps.dpiY     = 0.0f;

    renderTargetProps.minLevel = D2D1_FEATURE_LEVEL_DEFAULT;
    renderTargetProps.type     = D2D1_RENDER_TARGET_TYPE_DEFAULT;
    renderTargetProps.usage    = D2D1_RENDER_TARGET_USAGE_NONE;

    renderTargetProps.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
    renderTargetProps.pixelFormat.format    = DXGI_FORMAT_B8G8R8A8_UNORM;

    hwndRenderTargetProps.hwnd           = pFingerPointer->hWnd;
    hwndRenderTargetProps.presentOptions = D2D1_PRESENT_OPTIONS_NONE;

    hwndRenderTargetProps.pixelSize.width  = rc.right - rc.left;
    hwndRenderTargetProps.pixelSize.height = rc.bottom - rc.top;

    hRes = ID2D1Factory_CreateHwndRenderTarget(pFingerPointer->pFactory,
        &renderTargetProps, &hwndRenderTargetProps,
        &(pFingerPointer->pRenderTarget));

    if (FAILED(hRes)) {
        FingerPointer_OnDestroy(pFingerPointer);
        return -1;
    }

    pFingerPointer->spritePointer = Sprite_LoadFromResource(
        pFingerPointer->pRenderTarget,
        MAKEINTRESOURCE(IDR_POINTER_PNG), RT_RCDATA);

    if (pFingerPointer->spritePointer == NULL) {
        FingerPointer_OnDestroy(pFingerPointer);
        return -1;
    }

    pFingerPointer->trayicon = TrayIcon_Create(
        pFingerPointer->hWnd, UM_TRAYICON,
        (TRAYICONCALLBACK) FingerPointer_ToggleWindow, pFingerPointer);

    if (pFingerPointer->trayicon == NULL) {
        FingerPointer_OnDestroy(pFingerPointer);
        return -1;
    }

    hMenu = LoadMenu(
        pFingerPointer->hInstance, MAKEINTRESOURCE(IDM_MENU_MAIN));
    
    if (hMenu == NULL) {
        FingerPointer_OnDestroy(pFingerPointer);
        return -1;
    }

    TrayIcon_AttachMenu(pFingerPointer->trayicon, GetSubMenu(hMenu, 0));

    pFingerPointer->tweener = Tweener_Create(0.25f, -45.0f, 0);

    if (pFingerPointer->tweener == NULL) {
        FingerPointer_OnDestroy(pFingerPointer);
        return -1;
    }

    Tweener_SetEasing(pFingerPointer->tweener, EaseOutCircEasing);

    pFingerPointer->timer = Timer_Create();

    if (pFingerPointer->timer == NULL) {
        FingerPointer_OnDestroy(pFingerPointer);
        return -1;
    }

    /* ALT + H */
    RegisterHotKey(pFingerPointer->hWnd, 1, MOD_ALT | MOD_NOREPEAT, 0x48);

    pFingerPointer->bShow = FALSE;
    pFingerPointer->bInitialized = TRUE;

    return 0;
}

/***********************************************************************
 * WndProc
 ***********************************************************************/

static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam,
                                LPARAM lParam)
{
    LPCREATESTRUCT pcs;
    PFINGERPOINTER pFingerPointer = NULL;
    INT x, y;
    BOOL bResult;

    if (uMsg == WM_CREATE) {
        pcs = (LPCREATESTRUCT) lParam;

        pFingerPointer = (PFINGERPOINTER) pcs->lpCreateParams;
        pFingerPointer->hWnd = hWnd;

        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR) pFingerPointer);

        return FingerPointer_OnCreate(pFingerPointer);
    } else {
        pFingerPointer = (PFINGERPOINTER) GetWindowLongPtr(
            hWnd, GWLP_USERDATA);
    }

    if (pFingerPointer != NULL) {
        bResult = TrayIcon_HandleMessage(pFingerPointer->trayicon,
                                         uMsg, lParam);

        if (bResult == TRUE) {
            return 0;
        }
    }

    switch (uMsg) {
        case WM_MOUSEMOVE: {
            x = GET_X_LPARAM(lParam);
            y = GET_Y_LPARAM(lParam);

            return FingerPointer_OnMouseMove(pFingerPointer, x, y);
        }

        case WM_KEYDOWN:
            return FingerPointer_OnKey(pFingerPointer, wParam, lParam, TRUE);

        case WM_KEYUP:
            return FingerPointer_OnKey(pFingerPointer, wParam, lParam, FALSE);

        case WM_LBUTTONDOWN:
            return FingerPointer_OnLeftMouseButton(pFingerPointer, TRUE);
        
        case WM_LBUTTONUP:
            return FingerPointer_OnLeftMouseButton(pFingerPointer, FALSE);
        
        case WM_COMMAND:
            return FingerPointer_OnCommand(pFingerPointer, wParam, lParam);
        
        case WM_HOTKEY:
            return FingerPointer_OnHotkey(pFingerPointer, wParam, lParam);

        case WM_DESTROY:
            return FingerPointer_OnDestroy(pFingerPointer);
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

/***********************************************************************
 * WinMain
 ***********************************************************************/

 INT APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                        PTSTR lpCmdLine, int nCmdShow)
{
    PFINGERPOINTER pFingerPointer = NULL;
    MSG msg = {0};
    WNDCLASSEX wcex = {0};
    FLOAT fDelta;
    HWND hWnd = NULL;
    HANDLE hMutex = NULL;

    HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

    hMutex = CreateMutex(NULL, TRUE, FINGERPOINTER_CLASSNAME);

    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        return -1;
    }

    if (FAILED(CoInitialize(NULL))) {
        return -1;
    }

    wcex.cbSize        = sizeof(WNDCLASSEX);
    wcex.style         = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc   = WndProc;
    wcex.cbClsExtra    = 0;
    wcex.cbWndExtra    = sizeof(LONG_PTR);
    wcex.hInstance     = hInstance;
    wcex.hbrBackground = NULL;
    wcex.lpszMenuName  = NULL;
    wcex.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));
    wcex.lpszClassName = FINGERPOINTER_CLASSNAME;

    RegisterClassEx(&wcex);

    pFingerPointer = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
                               sizeof(FINGERPOINTER));
    
    if (pFingerPointer == NULL) {
        return -1;
    }

    pFingerPointer->hInstance = hInstance;

    hWnd = CreateWindowEx(
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
        pFingerPointer);
    
    if (hWnd == NULL) {
        CoUninitialize();
        return -1;
    }

    DwmExtendFrameIntoClientArea(hWnd, &(MARGINS) {-1});

    while (msg.message != WM_QUIT) {
        if (pFingerPointer->bShow == TRUE) {
            if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
                DispatchMessage(&msg);
            } else {
                Timer_Tick(pFingerPointer->timer);
    
                fDelta = Timer_GetDeltaTime(pFingerPointer->timer);

                FingerPointer_Update(pFingerPointer, fDelta);
                FingerPointer_Render(pFingerPointer);
            }
        } else {
            if (GetMessage(&msg, NULL, 0, 0) > 0) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
    }

    HeapFree(GetProcessHeap(), 0, pFingerPointer);
    CoUninitialize();
    ReleaseMutex(hMutex);
    return 0;
}