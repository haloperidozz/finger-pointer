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

#ifndef __EASING_H
#define __EASING_H

#include <Windows.h>
#include <math.h>

typedef FLOAT (*PFNEASING)(FLOAT x);

namespace Easing {

static inline FLOAT Linear(FLOAT x)
{
    return x;
}

// https://easings.net/#easeOutCirc
static inline FLOAT EaseOutCirc(FLOAT x)
{
    return sqrtf(1.0f - powf(x - 1.0f, 2.0f));
}

} // namespace Easing

#endif // __EASING_H