#pragma once
// THIS IS WINDOWS-ONLY
#define _CRT_SECURE_NO_WARNINGS
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <iostream>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "System_Common.h"

LILLIS::string const ERROR_LOG = L"Error: User pressed 'k'\n";
LILLIS::string const ERROR_WINDOW = L"Do not press 'k'!!\n";