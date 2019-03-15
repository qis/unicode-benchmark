#include "common.hpp"
#include <utfz.h>

static void utfz_validate(benchmark::State& state) {
  for (auto _ : state) {
    for (const auto cp : utfz::cp(reinterpret_cast<const char*>(src.data()), src.size())) {
      assert(cp != utfz::code::invalid);
      assert(cp != utfz::code::replace);
      benchmark::DoNotOptimize(cp);
    }
  }
}

static void utfz_iterate(benchmark::State& state) {
  for (auto _ : state) {
    std::size_t code_points = 0;
    for (const auto cp : utfz::cp(reinterpret_cast<const char*>(src.data()), src.size())) {
      assert(cp != utfz::code::invalid);
      assert(cp != utfz::code::replace);
      benchmark::DoNotOptimize(cp);
      code_points++;
    }
    assert(code_points == 29);
  }
}

static void utfz_convert(benchmark::State& state) {
  const auto beg = src.begin();
  const auto end = src.end();
  std::u32string str;
  str.reserve(256);
  for (auto _ : state) {
    str.clear();
    for (const auto cp : utfz::cp(reinterpret_cast<const char*>(src.data()), src.size())) {
      assert(cp != utfz::code::invalid);
      assert(cp != utfz::code::replace);
      benchmark::DoNotOptimize(cp);
      str.push_back(static_cast<char32_t>(cp));
    }
    assert(str.size() == 29);
    assert(str == dst);
  }
}

BENCHMARK(utfz_validate);
BENCHMARK(utfz_iterate);
BENCHMARK(utfz_convert);
