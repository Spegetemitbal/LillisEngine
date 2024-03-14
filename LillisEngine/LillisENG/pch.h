//Precompiled universal header.

#ifdef _DEBUG
#ifdef _WIN32
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#endif
#endif
#else
#define DBG_NEW new
#endif

#include <vector>
#include <map>
#include <list>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

//Put logging here too

//Any universal external library also goes here.

//All TypeDefs
//Unsigned ints
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

//Signed ints
typedef signed char i8;
typedef signed short i16;
typedef signed int i32;
typedef signed long long i64;

//Floats
typedef float f32;
typedef double f64;

//Boolean types
typedef int b32;
typedef char b8;