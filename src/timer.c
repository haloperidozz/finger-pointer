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

#include "timer.h"

struct _TIMER {
    LARGE_INTEGER frequency;
    LARGE_INTEGER lastCount;
    LARGE_INTEGER currentCount;
    FLOAT         fDeltaTime;
};

TIMER Timer_Create(VOID)
{
    TIMER timer;
    
    timer = HeapAlloc(
        GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(struct _TIMER));

    if (timer == NULL) {
        return NULL;
    }

    QueryPerformanceFrequency(&(timer->frequency));    
    Timer_Reset(timer);

    return timer;
}

VOID Timer_Tick(TIMER timer)
{
    LONGLONG llElapsed, llElapsedTime;

    if (timer == NULL) {
        return;
    }

    QueryPerformanceCounter(&(timer->currentCount));

    llElapsed = timer->currentCount.QuadPart - timer->lastCount.QuadPart;
    llElapsedTime = (llElapsed * 1000000) / timer->frequency.QuadPart;
    
    timer->fDeltaTime = (FLOAT) llElapsedTime / 1000000.0f;
    timer->lastCount = timer->currentCount;
}

FLOAT Timer_GetDeltaTime(TIMER timer)
{
    if (timer == NULL) {
        return 0.0f;
    }

    return timer->fDeltaTime;
}

VOID Timer_Reset(TIMER timer)
{
    if (timer == NULL) {
        return;
    }

    QueryPerformanceCounter(&(timer->lastCount));
    
    timer->fDeltaTime = 0.0f;
}

VOID Timer_Destroy(TIMER timer)
{
    if (timer == NULL) {
        return;
    }

    HeapFree(GetProcessHeap(), 0, timer);
}