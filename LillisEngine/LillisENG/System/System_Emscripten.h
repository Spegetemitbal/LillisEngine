#pragma once

#include "System_Common.h"
#include "SDL.h"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

GPR460::string const ERROR_LOG = "Error: User pressed 'k'\n";
GPR460::string const ERROR_WINDOW = "Do not press 'k'!!\n";