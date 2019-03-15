#include "common.hpp"
#include <utf8h/utf8.h>

static void utf8h_validate(benchmark::State& state) {
  const auto data = src.data();
  const auto size = src.size();
  for (auto _ : state) {
    const auto result = utf8valid(data);
    benchmark::DoNotOptimize(result);
    assert(result == nullptr);
  }
}

static void utf8h_iterate(benchmark::State& state) {
  const auto beg = src.data();
  const auto end = beg + src.size();
  for (auto _ : state) {
    std::size_t code_points = 0;
    for (auto it = beg; it && it < end; ) {
      utf8_int32_t cp = 0;
      it = reinterpret_cast<const u8string::value_type*>(utf8codepoint(it, &cp));  // NOTE: Messes up constness.
      benchmark::DoNotOptimize(cp);
      code_points++;
    }
    assert(code_points == 29);
  }
}

static void utf8h_convert(benchmark::State& state) {
  const auto beg = src.data();
  const auto end = beg + src.size();
  std::u32string str;
  str.reserve(256);
  for (auto _ : state) {
    str.clear();
    for (auto it = beg; it && it < end; ) {
      utf8_int32_t cp = 0;
      it = reinterpret_cast<const u8string::value_type*>(utf8codepoint(it, &cp));  // NOTE: Messes up constness.
      str.push_back(static_cast<char32_t>(cp));
      benchmark::DoNotOptimize(cp);
    }
    assert(str.size() == 29);
    assert(str == dst);
  }
}

BENCHMARK(utf8h_validate);
BENCHMARK(utf8h_iterate);
BENCHMARK(utf8h_convert);
