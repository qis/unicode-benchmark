#include "common.hpp"
#include <utf8.h>

static void utfcpp_validate(benchmark::State& state) {
  const auto beg = src.begin();
  const auto end = src.end();
  for (auto _ : state) {
    const auto result = utf8::find_invalid(beg, end);
    benchmark::DoNotOptimize(result);
    assert(result == end);
  }
}

static void utfcpp_iterate(benchmark::State& state) {
  const auto beg = src.begin();
  const auto end = src.end();
  for (auto _ : state) {
    std::size_t code_points = 0;
    for (auto it = beg; it != end; utf8::next(it, end)) {
      auto c = utf8::peek_next(it, end);
      benchmark::DoNotOptimize(c);
      code_points++;
    }
    assert(code_points == 29);
  }
}

static void utfcpp_convert(benchmark::State& state) {
  const auto beg = src.begin();
  const auto end = src.end();
  std::u32string str;
  str.reserve(256);
  for (auto _ : state) {
    str.clear();
    const auto result = utf8::utf8to32(beg, end, std::back_inserter(str));
    benchmark::DoNotOptimize(result);
    assert(str.size() == 29);
    assert(str == dst);
  }
}

BENCHMARK(utfcpp_validate);
BENCHMARK(utfcpp_iterate);
BENCHMARK(utfcpp_convert);
