#include "benchmark/benchmark.h"

#include <memory>

#include "item.h"
#include "item_func.h"

namespace
{

#define TEST_SET(NAME, ITEM)                                                   \
  void BM_##NAME##_val_bool_null(benchmark::State &state)                      \
  {                                                                            \
    for (auto _ : state)                                                       \
    {                                                                          \
      bool is_null;                                                            \
      ITEM->val_bool_null(&is_null);                                           \
    }                                                                          \
  }                                                                            \
  BENCHMARK(BM_##NAME##_val_bool_null);                                        \
                                                                               \
  void BM_##NAME##_to_bool_null(benchmark::State &state)                       \
  {                                                                            \
    for (auto _ : state)                                                       \
      ITEM->to_bool_null();                                                    \
  }                                                                            \
  BENCHMARK(BM_##NAME##_to_bool_null);                                         \
                                                                               \
  void BM_##NAME##_val_int_null(benchmark::State &state)                       \
  {                                                                            \
    for (auto _ : state)                                                       \
    {                                                                          \
      bool is_null;                                                            \
      ITEM->val_int_null(&is_null);                                            \
    }                                                                          \
  }                                                                            \
  BENCHMARK(BM_##NAME##_val_int_null);                                         \
                                                                               \
  void BM_##NAME##_to_longlong_null(benchmark::State &state)                   \
  {                                                                            \
    for (auto _ : state)                                                       \
      ITEM->to_longlong_null();                                                \
  }                                                                            \
  BENCHMARK(BM_##NAME##_to_longlong_null);                                     \
                                                                               \
  void BM_##NAME##_val_int32_null(benchmark::State &state)                     \
  {                                                                            \
    for (auto _ : state)                                                       \
    {                                                                          \
      bool is_null;                                                            \
      ITEM->val_int32_null(&is_null);                                          \
    }                                                                          \
  }                                                                            \
  BENCHMARK(BM_##NAME##_val_int32_null);                                       \
                                                                               \
  void BM_##NAME##_to_int32_null(benchmark::State &state)                      \
  {                                                                            \
    for (auto _ : state)                                                       \
      ITEM->to_int32_null();                                                   \
  }                                                                            \
  BENCHMARK(BM_##NAME##_to_int32_null);                                        \
                                                                               \
  void BM_##NAME##_val_real_null(benchmark::State &state)                      \
  {                                                                            \
    for (auto _ : state)                                                       \
    {                                                                          \
      bool is_null;                                                            \
      ITEM->val_real_null(&is_null);                                           \
    }                                                                          \
  }                                                                            \
  BENCHMARK(BM_##NAME##_val_real_null);                                        \
                                                                               \
  void BM_##NAME##_to_double_null(benchmark::State &state)                     \
  {                                                                            \
    for (auto _ : state)                                                       \
      ITEM->to_double_null();                                                  \
  }                                                                            \
  BENCHMARK(BM_##NAME##_to_double_null);

std::unique_ptr<Item> g_int(new Item_int(0x77));
std::unique_ptr<Item_func_uminus> g_uminus(new Item_func_uminus(g_int.get()));
std::unique_ptr<Item_func_isnull> g_isnull(new Item_func_isnull(g_int.get()));

TEST_SET(Item_int, g_int);
TEST_SET(Item_func_uminus, g_uminus);
TEST_SET(Item_func_isnull, g_isnull);

} // namespace

BENCHMARK_MAIN();
