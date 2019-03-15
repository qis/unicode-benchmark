#pragma once
#include <benchmark/benchmark.h>
#include <string>
#include <cassert>

#ifdef _MSC_VER
using u8string = std::string;
#else
using u8string = std::u8string;
#endif

using std::u32string;

extern const u8string src;
extern const u32string dst;
