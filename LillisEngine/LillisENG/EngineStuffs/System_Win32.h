#pragma once
// THIS IS WINDOWS-ONLY
#define _CRT_SECURE_NO_WARNINGS
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <iostream>
#include "SDL.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "System_Common.h"

GPR460::string const ERROR_LOG = L"Error: User pressed 'k'\n";
GPR460::string const ERROR_WINDOW = L"Do not press 'k'!!\n";