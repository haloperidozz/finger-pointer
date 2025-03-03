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
    LARGE_INTEGER   frequency;
    LARGE_INTEGER   lastCount;
    LARGE_INTEGER   currentCount;
    FLOAT           fDeltaTime;
};

PTIMER Timer_Create(VOID)
{
    PTIMER pTimer = NULL;
    
    pTimer = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(TIMER));

    if (pTimer == NULL) {
        return NULL;
    }

    QueryPerformanceFrequency(&(pTimer->frequency));    
    Timer_Reset(pTimer);

    return pTimer;
}

VOID Timer_Tick(PTIMER pTimer)
{
    LONGLONG llElapsed, llFrequency;

    if (pTimer == NULL) {
        return;
    }

    QueryPerformanceCounter(&(pTimer->currentCount));

    llElapsed = pTimer->currentCount.QuadPart - pTimer->lastCount.QuadPart;
    llFrequency = pTimer->frequency.QuadPart;

    pTimer->fDeltaTime = (FLOAT) llElapsed / (FLOAT) llFrequency;
    pTimer->lastCount = pTimer->currentCount;
}

FLOAT Timer_GetDeltaTime(CONST PTIMER pTimer)
{
    return (pTimer != NULL) ? pTimer->fDeltaTime : 0.0f;
}

VOID Timer_Reset(PTIMER pTimer)
{
    if (pTimer == NULL) {
        return;
    }

    QueryPerformanceCounter(&(pTimer->lastCount));
    
    pTimer->fDeltaTime = 0.0f;
}

VOID Timer_Destroy(PTIMER pTimer)
{
    if (pTimer != NULL) {
        HeapFree(GetProcessHeap(), 0, pTimer);
    }
}