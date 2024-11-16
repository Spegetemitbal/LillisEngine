#pragma once

#ifdef _WIN32
#include "System_Win32.h"
#elif __EMSCRIPTEN__
#include "System_Emscripten.h"
#endif