#pragma once

#include <cassert>
#include <stdarg.h>
#include <string.h>

int arrayToParameter(char* array, int size, const char* format, ...);
int parameterToArray(char* array, int size, const char* format, ...);