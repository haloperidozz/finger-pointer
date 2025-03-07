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

#include <windows.h>
#include <windowsx.h>
#include <math.h>
#include <dwmapi.h>
#include <tchar.h>

#include "sprite.h"
#include "tweener.h"
#include "timer.h"
#include "audio.h"

#include "d2d1_vtbl.h"
#include "openurl.h"

#include "resource.h"

#define FINGERPOINTER_CLASSNAME TEXT("FingerPointerClass")

#define UM_TRAYICON             (WM_USER + 1)

static struct _APPLICATION {
    HWND                    hWnd;
    HINSTANCE               hInstance;

    ID2D1Factory           *pFactory;
    ID2D1HwndRenderTarget  *pRenderTarget;

    PSPRITE                 pSprite;
    PTWEENER                pTweener;
    PTIMER                  pTimer;
    PAUDIO                  pEffect;
    PAUDIO                  pEffectMove;

    NOTIFYICONDATA          nid;

    FLOAT                   fScale;

    D2D1_POINT_2F           position;
    D2D1_POINT_2F           lastPosition;
    
    BOOL                    bPressed;
    BOOL                    bInitialized;
    BOOL                    bShow;
    BOOL                    bUpdateScale;
} g_application;

static VOID Application_ToggleWindowVisibility(VOID)
{
    if (g_application.bInitialized == FALSE) {
        return;
    }

    g_application.bShow = !g_application.bShow;

    if (g_application.bShow == FALSE) {
        ShowWindow(g_application.hWnd, SW_HIDE);
        ID2D1HwndRenderTarget_Flush(g_application.pRenderTarget, NULL, NULL);
    }

    if (g_application.bShow == TRUE) {
        ShowWindow(g_application.hWnd, SW_SHOW);
        UpdateWindow(g_application.hWnd);

        SetCursor(NULL);
    }
}

static LRESULT Application_OnWindowCreate(HWND hWnd)
{
    D2D1_RENDER_TARGET_PROPERTIES renderTargetProps;
    D2D1_HWND_RENDER_TARGET_PROPERTIES hwndRenderTargetProps;
    D2D1_SIZE_U spriteSize;
    D2D1_POINT_2F spritePosition;
    RECT rc;
    HRESULT hRes = S_OK;

    g_application.hWnd = hWnd;
    g_application.hInstance = GetWindowInstance(hWnd);

    hRes = D2D1CreateFactory(
        D2D1_FACTORY_TYPE_SINGLE_THREADED,
        &IID_ID2D1Factory, NULL, (LPVOID*) &(g_application.pFactory));
    
    if (FAILED(hRes)) {
        return -1;
    }

    GetClientRect(g_application.hWnd, &rc);

    renderTargetProps.dpiX     = 0.0f;
    renderTargetProps.dpiY     = 0.0f;

    renderTargetProps.minLevel = D2D1_FEATURE_LEVEL_DEFAULT;
    renderTargetProps.type     = D2D1_RENDER_TARGET_TYPE_DEFAULT;
    renderTargetProps.usage    = D2D1_RENDER_TARGET_USAGE_NONE;

    renderTargetProps.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
    renderTargetProps.pixelFormat.format    = DXGI_FORMAT_B8G8R8A8_UNORM;

    hwndRenderTargetProps.hwnd           = hWnd;
    hwndRenderTargetProps.presentOptions = D2D1_PRESENT_OPTIONS_NONE;

    hwndRenderTargetProps.pixelSize.width  = rc.right - rc.left;
    hwndRenderTargetProps.pixelSize.height = rc.bottom - rc.top;

    hRes = ID2D1Factory_CreateHwndRenderTarget(
        g_application.pFactory,
        &renderTargetProps, &hwndRenderTargetProps,
        &(g_application.pRenderTarget));

    if (FAILED(hRes)) {
        goto destroy;
    }

    g_application.pSprite = Sprite_CreateFromResource(
        g_application.pRenderTarget,
        MAKEINTRESOURCE(IDR_POINTER_PNG), RT_RCDATA);

    if (g_application.pSprite == NULL) {
        goto destroy;
    }

    g_application.pTweener = Tweener_Create(0.25f, -45.0f, 0);

    if (g_application.pTweener == NULL) {
        goto destroy;
    }

    Tweener_SetEasing(g_application.pTweener, Easing_EaseOutCirc);

    g_application.pTimer = Timer_Create();

    if (g_application.pTimer == NULL) {
        goto destroy;
    }

    g_application.pEffect = Audio_LoadFromResource(
        g_application.hInstance,
        MAKEINTRESOURCE(IDR_EFFECT_WAV), TEXT("WAVE"));

    if (g_application.pEffect == NULL) {
        goto destroy;
    }

    g_application.pEffectMove = Audio_LoadFromResource(
        g_application.hInstance,
        MAKEINTRESOURCE(IDR_EFFECT_MOVE_WAV), TEXT("WAVE"));

    if (g_application.pEffectMove == NULL) {
        goto destroy;
    }

    Audio_SetLoop(g_application.pEffectMove, TRUE);

    /* ALT + H */
    RegisterHotKey(g_application.hWnd, 1, MOD_ALT | MOD_NOREPEAT, 0x48);

    ZeroMemory(&(g_application.nid), sizeof(NOTIFYICONDATA));

    g_application.nid.cbSize           = sizeof(NOTIFYICONDATA);
    g_application.nid.hWnd             = g_application.hWnd;
    g_application.nid.uID              = 1;
    g_application.nid.uFlags           = NIF_MESSAGE | NIF_ICON | NIF_TIP;
    g_application.nid.uCallbackMessage = UM_TRAYICON;

    GetWindowText(hWnd, g_application.nid.szTip, 128);

    g_application.nid.hIcon = (HICON) GetClassLongPtr(hWnd, GCLP_HICON);
    
    if (Shell_NotifyIcon(NIM_ADD, &(g_application.nid)) == FALSE) {
        goto destroy;
    }

    g_application.bShow = FALSE;
    g_application.bInitialized = TRUE;
    g_application.bUpdateScale = TRUE;

    g_application.fScale = 1.0f;

    spriteSize = Sprite_GetBitmapSize(g_application.pSprite);

    spritePosition.x = ((rc.right - rc.left) - spriteSize.width)  / 2.0f;
    spritePosition.y = ((rc.bottom - rc.top) - spriteSize.height) / 2.0f;

    g_application.position = spritePosition;

    return 0;

destroy:
    DestroyWindow(g_application.hWnd);
    return -1;
}

static LRESULT Application_OnTrayIcon(WPARAM wParam, LPARAM lParam)
{
    HINSTANCE hInstance = g_application.hInstance;
    HMENU hMenu = NULL;
    HMENU hMenuPopup = NULL;
    POINT pt;

    hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDM_MENU_MAIN));
    hMenuPopup = GetSubMenu(hMenu, 0);
    
    switch (lParam) {
        case WM_RBUTTONUP: {
            GetCursorPos(&pt);

            TrackPopupMenuEx(
                hMenuPopup, TPM_RIGHTBUTTON, pt.x, pt.y,
                g_application.hWnd, NULL);
            
            break;
        }
        
        case WM_LBUTTONUP: {
            Application_ToggleWindowVisibility();
            break;
        }
    }

    return 0;
}

static LRESULT Application_OnMouseWheel(WPARAM wParam, LPARAM lParam)
{
    FLOAT fNewScale;
    SHORT zDelta = GET_WHEEL_DELTA_WPARAM(wParam);

    fNewScale = g_application.fScale += (zDelta > 0) ? 0.05f : -0.05f;

    g_application.fScale = fmaxf(0.0f, fminf(fNewScale, 1.0f));
    g_application.bUpdateScale = TRUE;
    return 0;
}

static LRESULT Application_OnMouseMove(WPARAM wParam, LPARAM lParam)
{
    D2D1_POINT_2F spritePosition;
    D2D1_SIZE_U spriteBitmapSize;
    INT iCenterX, iCenterY;
    INT iDeltaX, iDeltaY;
    INT iMinX, iMinY, iMaxX, iMaxY;
    POINT ptCenter;
    RECT rc;

    spriteBitmapSize = Sprite_GetBitmapSize(g_application.pSprite);

    GetClientRect(g_application.hWnd, &rc);

    iCenterX = (rc.right - rc.left) / 2;
    iCenterY = (rc.bottom - rc.top) / 2;

    iDeltaX = GET_X_LPARAM(lParam) - iCenterX;
    iDeltaY = GET_Y_LPARAM(lParam) - iCenterY;

    spritePosition.x = g_application.position.x + (FLOAT) iDeltaX;
    spritePosition.y = g_application.position.y + (FLOAT) iDeltaY;

    iMinX = -((FLOAT) spriteBitmapSize.width  * g_application.fScale);
    iMinY = -((FLOAT) spriteBitmapSize.height * g_application.fScale);
    iMaxX = (rc.right - rc.left) + (-iMinX);
    iMaxY = (rc.bottom - rc.top) + (-iMinY);

    spritePosition.x = fmaxf(iMinX, fminf(spritePosition.x, iMaxX));
    spritePosition.y = fmaxf(iMinY, fminf(spritePosition.y, iMaxY));

    g_application.position = spritePosition;

    ptCenter.x = iCenterX;
    ptCenter.y = iCenterY;

    ClientToScreen(g_application.hWnd, &ptCenter);

    SetCursorPos(ptCenter.x, ptCenter.y);
    return 0;
}

static LRESULT Application_OnLeftButtonDown(WPARAM wParam, LPARAM lParam)
{
    g_application.bPressed = TRUE;

    Tweener_Invert(g_application.pTweener, g_application.bPressed);

    Audio_PlayAsync(g_application.pEffect);

    return 0;
}


static LRESULT Application_OnLeftButtonUp(WPARAM wParam, LPARAM lParam)
{
    g_application.bPressed = FALSE;

    Tweener_Invert(g_application.pTweener, g_application.bPressed);

    Audio_Stop(g_application.pEffect);
    Audio_Stop(g_application.pEffectMove);

    return 0;
}

static LRESULT Application_OnHotkey(WPARAM wParam, LPARAM lParam)
{
    Application_ToggleWindowVisibility();
    return 0;
}

static LRESULT Application_OnCommand(WPARAM wParam, LPARAM lParam)
{
    switch (LOWORD(wParam)) {
        case IDM_ITEM_SHOW:
            Application_ToggleWindowVisibility();
            break;
        case IDM_ITEM_SOURCE_CODE:
            OpenUrl(MAKEINTRESOURCE(IDS_GITHUB_URL), OU_RESOURCE);
            break;
        case IDM_ITEM_TELEGRAM:
            OpenUrl(MAKEINTRESOURCE(IDS_TELEGRAM_URL), OU_RESOURCE);
            break;
        case IDM_ITEM_TIKTOK:
            OpenUrl(MAKEINTRESOURCE(IDS_TIKTOK_URL), OU_RESOURCE);
            break;
        case IDM_ITEM_EXIT:
            DestroyWindow(g_application.hWnd);
            break;
    }

    return 0;
}

static VOID Application_OnRender(VOID)
{
    if (g_application.bInitialized == FALSE) {
        return;
    }

    ID2D1HwndRenderTarget_BeginDraw(g_application.pRenderTarget);
    ID2D1HwndRenderTarget_Clear(g_application.pRenderTarget, 0);

    Sprite_Draw(g_application.pSprite, g_application.pRenderTarget);
    
    ID2D1HwndRenderTarget_EndDraw(g_application.pRenderTarget, NULL, NULL);
}

static VOID Application_OnUpdate(FLOAT fDelta)
{
    D2D1_SIZE_U size;
    D2D1_POINT_2F center;
    D2D1_SIZE_F newScale;
    FLOAT fRotation;

    if (g_application.bInitialized == FALSE) {
        return;
    }

    if (g_application.lastPosition.x != g_application.position.x ||
        g_application.lastPosition.y != g_application.position.y &&
        g_application.bPressed
    ) {
        Audio_PlayAsync(g_application.pEffectMove);
    } else {
        Audio_Stop(g_application.pEffectMove);
    }

    g_application.lastPosition = g_application.position;

    Sprite_SetPosition(g_application.pSprite, g_application.position);

    if (g_application.bUpdateScale == TRUE) {
        size = Sprite_GetBitmapSize(g_application.pSprite);

        newScale.width  = g_application.fScale;
        newScale.height = g_application.fScale;

        center.x = ((FLOAT) size.width  * newScale.width) / 2.0f;
        center.y = ((FLOAT) size.height * newScale.height);

        Sprite_SetRotationCenter(g_application.pSprite, center);
        Sprite_SetScale(g_application.pSprite, newScale);

        g_application.bUpdateScale = FALSE;
    }

    if (Tweener_Update(g_application.pTweener, fDelta)) {
        fRotation = Tweener_GetValue(g_application.pTweener);
        Sprite_SetRotation(g_application.pSprite, fRotation);
    }
}

static LRESULT Application_OnWindowDestroy(VOID)
{
    Shell_NotifyIcon(NIM_DELETE, &(g_application.nid));

    if (g_application.pEffectMove != NULL) {
        Audio_Destroy(g_application.pEffectMove);
        g_application.pEffectMove = NULL;
    }

    if (g_application.pEffect != NULL) {
        Audio_Destroy(g_application.pEffect);
        g_application.pEffect = NULL;
    }

    if (g_application.pTimer != NULL) {
        Timer_Destroy(g_application.pTimer);
        g_application.pTimer = NULL;
    }

    if (g_application.pTweener != NULL) {
        Tweener_Destroy(g_application.pTweener);
        g_application.pTweener = NULL;
    }

    if (g_application.pSprite != NULL) {
        Sprite_Destroy(g_application.pSprite);
        g_application.pSprite = NULL;
    }

    if (g_application.pRenderTarget != NULL) {
        ID2D1HwndRenderTarget_Release(g_application.pRenderTarget);
        g_application.pRenderTarget = NULL;
    }

    if (g_application.pFactory != NULL) {
        ID2D1Factory_Release(g_application.pFactory);
        g_application.pFactory = NULL;
    }

    g_application.bInitialized = FALSE;

    PostQuitMessage(0);
}

static LRESULT CALLBACK
Application_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
        case WM_CREATE:
            return Application_OnWindowCreate(hWnd);
        
        case UM_TRAYICON:
            return Application_OnTrayIcon(wParam, lParam);
        
        case WM_MOUSEWHEEL:
            return Application_OnMouseWheel(wParam, lParam);

        case WM_MOUSEMOVE:
            return Application_OnMouseMove(wParam, lParam);        
        
        case WM_LBUTTONDOWN:
            return Application_OnLeftButtonDown(wParam, lParam);
        
        case WM_LBUTTONUP:
            return Application_OnLeftButtonUp(wParam, lParam);
        
        case WM_COMMAND:
            return Application_OnCommand(wParam, lParam);
        
        case WM_HOTKEY:
            return Application_OnHotkey(wParam, lParam);

        case WM_DESTROY:
            return Application_OnWindowDestroy();
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

static ATOM Application_RegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex = {0};

    wcex.cbSize        = sizeof(WNDCLASSEX);
    wcex.style         = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc   = Application_WndProc;
    wcex.cbClsExtra    = 0;
    wcex.cbWndExtra    = sizeof(LONG_PTR);
    wcex.hInstance     = hInstance;
    wcex.hbrBackground = NULL;
    wcex.lpszMenuName  = NULL;
    wcex.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));
    wcex.lpszClassName = FINGERPOINTER_CLASSNAME;

    return RegisterClassEx(&wcex);
}

HWND Application_CreateWindow(HINSTANCE hInstance)
{
    HWND hWnd = NULL;

    if (g_application.bInitialized == TRUE) {
        return NULL;
    }

    if (Application_RegisterClass(hInstance) == 0) {
        return NULL;
    }

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
        NULL);
    
    if (hWnd != NULL) {
        DwmExtendFrameIntoClientArea(hWnd, &(MARGINS) {-1});
    }

    return hWnd;
}

VOID Application_RunMessageLoop(VOID)
{
    FLOAT fDelta = 0.0f;
    MSG msg = {0};

    while (msg.message != WM_QUIT) {
        if (g_application.bShow == TRUE) {
            if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
                DispatchMessage(&msg);
            } else {
                Timer_Tick(g_application.pTimer);
    
                fDelta = Timer_GetDeltaTime(g_application.pTimer);

                Application_OnUpdate(fDelta);
                Application_OnRender();
            }
        } else {
            if (GetMessage(&msg, NULL, 0, 0) > 0) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
    }
}