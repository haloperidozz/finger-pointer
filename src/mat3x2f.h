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

#ifndef __MAT3X2F_H
#define __MAT3X2F_H

#include <windows.h>
#include <d2d1.h>

static inline D2D1_MATRIX_3X2_F Matrix3x2F_Rotation(FLOAT fAngle,
                                                    D2D1_POINT_2F center)
{
    D2D1_MATRIX_3X2_F result;
    D2D1MakeRotateMatrix(fAngle, center, &result);
    return result;
}

static inline D2D1_MATRIX_3X2_F Matrix3x2F_Translation(FLOAT dx, FLOAT dy)
{
    return (D2D1_MATRIX_3X2_F) { 1.0f, 0.0f, 0.0f, 1.0f, dx, dy };
}

static inline
D2D1_MATRIX_3X2_F Matrix3x2F_TranslationByPoint(D2D1_POINT_2F point)
{
    return Matrix3x2F_Translation(point.x, point.y);
}

static inline D2D1_MATRIX_3X2_F Matrix3x2F_Multiply(D2D1_MATRIX_3X2_F *m1,
                                                    D2D1_MATRIX_3X2_F *m2)
{
    D2D1_MATRIX_3X2_F result;

    result.m11 = m1->m11 * m2->m11 + m1->m12 * m2->m21;
    result.m12 = m1->m11 * m2->m12 + m1->m12 * m2->m22;
    result.m21 = m1->m21 * m2->m11 + m1->m22 * m2->m21;
    result.m22 = m1->m21 * m2->m12 + m1->m22 * m2->m22;
    result.dx  = m1->m11 * m2->dx  + m1->m12 * m2->dy + m1->dx;
    result.dy  = m1->m21 * m2->dx  + m1->m22 * m2->dy + m1->dy;

    return result;
}

#endif /* __MAT3X2F_H */