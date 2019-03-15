#include "common.hpp"
#include <utf8proc.h>

static void utf8proc_validate(benchmark::State& state) {
  const auto data = reinterpret_cast<const utf8proc_uint8_t*>(src.data());
  const auto size = static_cast<utf8proc_ssize_t>(src.size());
  utf8proc_int32_t cp = 0;
  for (auto _ : state) {
    for (auto it = data; it < data + size; ) {
      const auto result = utf8proc_iterate(it, data + size - it, &cp);
      assert(result > 0);
      benchmark::DoNotOptimize(result);
      it += result;
    }
  }
}

static void utf8proc_iterate(benchmark::State& state) {
  const auto data = reinterpret_cast<const utf8proc_uint8_t*>(src.data());
  const auto size = static_cast<utf8proc_ssize_t>(src.size());
  utf8proc_int32_t cp = 0;
  for (auto _ : state) {
    std::size_t count = 0;
    for (auto it = data; it < data + size; ) {
      const auto result = utf8proc_iterate(it, data + size - it, &cp);
      assert(result > 0);
      benchmark::DoNotOptimize(result);
      it += result;
      count++;
    }
    assert(count == 29);
  }
}

static void utf8proc_convert(benchmark::State& state) {
  const auto data = reinterpret_cast<const utf8proc_uint8_t*>(src.data());
  const auto size = static_cast<utf8proc_ssize_t>(src.size());
  utf8proc_int32_t cp = 0;
  std::u32string str;
  str.reserve(256);
  for (auto _ : state) {
    str.clear();
    for (auto it = data; it < data + size; ) {
      const auto result = utf8proc_iterate(it, data + size - it, &cp);
      assert(result > 0);
      benchmark::DoNotOptimize(result);
      str.push_back(static_cast<char32_t>(cp));
      it += result;
    }
    assert(str.size() == 29);
    assert(str == dst);
  }
}

BENCHMARK(utf8proc_validate);
BENCHMARK(utf8proc_iterate);
BENCHMARK(utf8proc_convert);
