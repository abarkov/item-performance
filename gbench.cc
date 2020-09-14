#include "benchmark/benchmark.h"

#include "item.h"

static void BM_Item_int_to_int32_null(benchmark::State &state)
{
  Item_int item(1);
  for (auto _ : state)
  {
    auto tmp = item.to_int32_null();
    benchmark::DoNotOptimize(tmp);
  }
}
BENCHMARK(BM_Item_int_to_int32_null);

static void BM_Item_int_to_longlong_null(benchmark::State &state)
{
  Item_int item(1);
  for (auto _ : state)
  {
    auto tmp = item.to_longlong_null();
    benchmark::DoNotOptimize(tmp);
  }
}
BENCHMARK(BM_Item_int_to_longlong_null);

BENCHMARK_MAIN();
