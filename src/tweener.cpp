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

#include "tweener.h"

#include <math.h>

Tweener::Tweener(
    FLOAT       fDuration,
    FLOAT       fStart,
    FLOAT       fTarget,
    PFNEASING   pfnEasing)
    : _fDuration(fDuration),
      _fStart(fStart),
      _fTarget(fTarget),
      _fProgress(0.0f),
      _fValue(fStart),
      _bInverted(FALSE),
      _pfnEasing(pfnEasing)
{
}

BOOL Tweener::Update(FLOAT fDeltaTime)
{
    FLOAT fEasedRatio;

    if (_fDuration <= 0.0f) {
        _fValue = _fTarget;
        _fProgress = _fDuration;
        return FALSE;
    }

    _fProgress += (_bInverted ? -fDeltaTime : fDeltaTime);
    _fProgress = fmaxf(0.0f, fminf(_fProgress, _fDuration));

    fEasedRatio = _pfnEasing(_fProgress / _fDuration);

    _fValue = _fStart + (_fTarget - _fStart) * fEasedRatio;

    return _fProgress < _fDuration;
}

FLOAT Tweener::GetValue() const
{
    return _fValue;
}

VOID Tweener::Invert(BOOL bInvert)
{
    _bInverted = bInvert;
}

void Tweener::SetEasing(PFNEASING pfnEasing)
{
    _pfnEasing = pfnEasing;
}

void Tweener::Reset()
{
    _fProgress = 0.0f;
    _fValue = _fStart;
}
