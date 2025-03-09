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

#ifndef __TIMER_H
#define __TIMER_H

#include <Windows.h>

class Timer {
public:
    Timer();

    void Tick();

    FLOAT GetDeltaTime() CONST;

    VOID Reset();

private:
    LARGE_INTEGER   _frequency;
    LARGE_INTEGER   _lastCount;
    LARGE_INTEGER   _currentCount;
    FLOAT           _fDeltaTime;
};

#endif // __TIMER_H