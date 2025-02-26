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

#include <windows.h>

typedef struct _TIMER *TIMER;

TIMER Timer_Create(VOID);

VOID Timer_Tick(TIMER timer);

FLOAT Timer_GetDeltaTime(TIMER timer);

VOID Timer_Reset(TIMER timer);

VOID Timer_Destroy(TIMER timer);

#endif /* __TIMER_H */