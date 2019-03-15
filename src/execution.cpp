#include <benchmark/benchmark.h>
#include <algorithm>
#include <execution>
#include <random>
#include <vector>

template <typename Policy>
static void benchmark_test(benchmark::State& state, Policy policy) {
  thread_local std::random_device rd;
  thread_local std::uniform_int_distribution<int> dist(0, std::numeric_limits<int>::max());
  std::vector<int> v(100);
  for (auto _ : state) {
    std::generate(policy, v.begin(), v.end(), [] () {
      return dist(rd);
    });
  }
}

BENCHMARK_CAPTURE(benchmark_test, seq, std::execution::seq);
BENCHMARK_CAPTURE(benchmark_test, par, std::execution::par);
BENCHMARK_CAPTURE(benchmark_test, par_unseq, std::execution::par_unseq);
