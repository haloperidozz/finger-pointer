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
#include <tchar.h>
#include <mfapi.h>

#include "application.h"

INT APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                       PTSTR lpCmdLine, int nCmdShow)
{
    HANDLE hMutex = NULL;

    HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

    hMutex = CreateMutex(NULL, TRUE, TEXT("FingerPointer_ONCE"));

    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        return -1;
    }

    if (FAILED(CoInitialize(NULL))) {
        return -1;
    }

    if (FAILED(MFStartup(MF_VERSION, MFSTARTUP_LITE))) {
        return -1;
    }

    if (Application_CreateWindow(hInstance) == NULL) {
        return -1;
    }

    Application_RunMessageLoop();

    MFShutdown();
    CoUninitialize();
    ReleaseMutex(hMutex);
    return 0;
}