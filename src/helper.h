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
#include <math.h>
#include <d2d1.h>

/***********************************************************************
 * Easing
 ***********************************************************************/

static FLOAT Easing_Linear(FLOAT x)
{
    return x;
}

/* Easing_EaseOutCirc: https://easings.net/#easeOutCirc */
static FLOAT Easing_EaseOutCirc(FLOAT x)
{
    return sqrtf(1.0f - powf(x - 1.0f, 2.0f));
}

/***********************************************************************
 * Matrix
 ***********************************************************************/

#define MakeRotateMatrix(fAngle, center, pResult)                   \
    D2D1MakeRotateMatrix((fAngle), (center), (pResult))

static inline VOID
MakeTranslateMatrix(D2D1_POINT_2F point, D2D1_MATRIX_3X2_F *pResult)
{
    pResult->m11 = 1.0f;
    pResult->m12 = 0.0f;
    pResult->m21 = 0.0f;
    pResult->m22 = 1.0f;
    pResult->dx  = point.x;
    pResult->dy  = point.y;
}

static inline VOID
MakeScaleMatrix(D2D1_SIZE_F size, D2D1_POINT_2F center,
                D2D1_MATRIX_3X2_F *pResult)
{
    pResult->m11 = size.width;
    pResult->m12 = 0.0f;
    pResult->m21 = 0.0f;
    pResult->m22 = size.height;
    pResult->dx  = center.x - size.width  * center.x;
    pResult->dy  = center.y - size.height * center.y;
}

static inline VOID
MultiplyMatrices(D2D1_MATRIX_3X2_F *m1, D2D1_MATRIX_3X2_F *m2,
                 D2D1_MATRIX_3X2_F *pResult)
{
    pResult->m11 = m1->m11 * m2->m11 + m1->m12 * m2->m21;
    pResult->m12 = m1->m11 * m2->m12 + m1->m12 * m2->m22;
    pResult->m21 = m1->m21 * m2->m11 + m1->m22 * m2->m21;
    pResult->m22 = m1->m21 * m2->m12 + m1->m22 * m2->m22;
    pResult->dx  = m1->m11 * m2->dx  + m1->m12 * m2->dy + m1->dx;
    pResult->dy  = m1->m21 * m2->dx  + m1->m22 * m2->dy + m1->dy;
}

/***********************************************************************
 * Misc
 ***********************************************************************/

static LPVOID LoadResourceToMemory(HINSTANCE hInstance,
                                   LPCTSTR lpszName,
                                   LPCTSTR lpszType,
                                   LPDWORD lpdwSize)
{
    HRSRC hResource = NULL;
    DWORD dwImageSize;
    HGLOBAL hResourceData = NULL;

    hResource = FindResource(hInstance, lpszName, lpszType);

    if (hResource == NULL) {
        return NULL;
    }

    dwImageSize = SizeofResource(hInstance, hResource);

    if (dwImageSize <= 0) {
        return NULL;
    }

    hResourceData = LoadResource(hInstance, hResource);

    if (hResourceData == NULL) {
        return NULL;
    }

    *lpdwSize = dwImageSize;

    return LockResource(hResourceData);
}

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

#endif /* __HELPER_H */