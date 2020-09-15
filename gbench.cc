#include "benchmark/benchmark.h"

#include <memory>

#include "item.h"
#include "item_func.h"

namespace
{

#define TEST_SET(NAME, ITEM)                                                   \
  void BM_##NAME##_get_bool(benchmark::State &state)                           \
  {                                                                            \
    for (auto _ : state)                                                       \
    {                                                                          \
      bool value;                                                              \
      ITEM->get_bool(&value);                                                  \
    }                                                                          \
  }                                                                            \
  BENCHMARK(BM_##NAME##_get_bool);                                             \
                                                                               \
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
  void BM_##NAME##_get_longlong(benchmark::State &state)                       \
  {                                                                            \
    for (auto _ : state)                                                       \
    {                                                                          \
      longlong value;                                                          \
      ITEM->get_longlong(&value);                                              \
    }                                                                          \
  }                                                                            \
  BENCHMARK(BM_##NAME##_get_longlong);                                         \
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
  void BM_##NAME##_get_int32(benchmark::State &state)                          \
  {                                                                            \
    for (auto _ : state)                                                       \
    {                                                                          \
      int32 value;                                                             \
      ITEM->get_int32(&value);                                                 \
    }                                                                          \
  }                                                                            \
  BENCHMARK(BM_##NAME##_get_int32);                                            \
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
  void BM_##NAME##_get_double(benchmark::State &state)                         \
  {                                                                            \
    for (auto _ : state)                                                       \
    {                                                                          \
      double value;                                                            \
      ITEM->get_double(&value);                                                \
    }                                                                          \
  }                                                                            \
  BENCHMARK(BM_##NAME##_get_double);                                           \
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

std::unique_ptr<Item> g_int1(new Item_int(0x77));
std::unique_ptr<Item> g_int2(new Item_int(0x77));
std::unique_ptr<Item> g_uminus(new Item_func_uminus(g_int1.get()));
std::unique_ptr<Item> g_isnull(new Item_func_isnull(g_int1.get()));
std::unique_ptr<Item> g_add(new Item_func_add(g_uminus.get(), g_isnull.get()));

TEST_SET(Item_int, g_int1);
TEST_SET(Item_func_uminus, g_uminus);
TEST_SET(Item_func_isnull, g_isnull);
TEST_SET(Item_func_add, g_add);

} // namespace

BENCHMARK_MAIN();
