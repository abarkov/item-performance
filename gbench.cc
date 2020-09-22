#include "benchmark/benchmark.h"

#include <memory>

#include "item.h"
#include "item_func.h"
#include "variant.hpp"

namespace
{

#define TEST_SET(NAME, ITEM)                                                   \
  void BM_##NAME##_val_bool(benchmark::State &state)                           \
  {                                                                            \
    for (auto _ : state)                                                       \
    {                                                                          \
      ITEM->val_bool();                                                        \
      bool is_null= ITEM->null_value;                                          \
      benchmark::DoNotOptimize(is_null);                                       \
    }                                                                          \
  }                                                                            \
  BENCHMARK(BM_##NAME##_val_bool);                                             \
                                                                               \
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
      bool is_null= false;                                                     \
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
  void BM_##NAME##_val_int(benchmark::State &state)                            \
  {                                                                            \
    for (auto _ : state)                                                       \
    {                                                                          \
      ITEM->val_int();                                                         \
      bool is_null= ITEM->null_value;                                          \
      benchmark::DoNotOptimize(is_null);                                       \
    }                                                                          \
  }                                                                            \
  BENCHMARK(BM_##NAME##_val_int);                                              \
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
      bool is_null= false;                                                     \
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
  void BM_##NAME##_val_int32(benchmark::State &state)                          \
  {                                                                            \
    for (auto _ : state)                                                       \
    {                                                                          \
      ITEM->val_int32();                                                       \
      bool is_null= ITEM->null_value;                                          \
      benchmark::DoNotOptimize(is_null);                                       \
    }                                                                          \
  }                                                                            \
  BENCHMARK(BM_##NAME##_val_int32);                                            \
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
      bool is_null= false;                                                     \
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
  void BM_##NAME##_val_real(benchmark::State &state)                           \
  {                                                                            \
    for (auto _ : state)                                                       \
    {                                                                          \
      ITEM->val_real();                                                        \
      bool is_null= ITEM->null_value;                                          \
      benchmark::DoNotOptimize(is_null);                                       \
    }                                                                          \
  }                                                                            \
  BENCHMARK(BM_##NAME##_val_real);                                             \
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
      bool is_null= false;                                                     \
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
std::unique_ptr<Item> g_coalesce1(new Item_func_coalesce(g_int1.get()));
std::unique_ptr<Item> g_real1(new Item_real(1.0));
std::unique_ptr<Item> g_null1(new Item_null);
std::unique_ptr<Item> g_null2(new Item_null);
std::unique_ptr<Item> g_coalesce3(new Item_func_coalesce(g_null1.get(),
                                                         g_null2.get(),
                                                         g_real1.get()));
std::unique_ptr<Item> g_bool(new Item_bool(false));
std::unique_ptr<Item> g_coalesce_bool(new Item_func_coalesce(g_bool.get()));

TEST_SET(Item_int, g_int1);
TEST_SET(Item_func_uminus, g_uminus);
TEST_SET(Item_func_isnull, g_isnull);
TEST_SET(Item_func_add, g_add);
TEST_SET(Item_func_coalesce1, g_coalesce1);
TEST_SET(Item_func_coalesce3, g_coalesce3);
TEST_SET(Item_func_coalesce_bool, g_coalesce_bool);

std::pair<Item *, std::vector<std::unique_ptr<Item>>> g_random_tree=
    generate_tree(40);
TEST_SET(Item_random_tree, g_random_tree.first);

#define VARIANT_TEST_SET(NAME, ITEM)                                           \
  void BM_variant_##NAME##_to_bool_null(benchmark::State &state)               \
  {                                                                            \
    for (auto _ : state)                                                       \
      to_bool_null(ITEM);                                                      \
  }                                                                            \
  BENCHMARK(BM_variant_##NAME##_to_bool_null);                                 \
                                                                               \
  void BM_variant_##NAME##_to_longlong_null(benchmark::State &state)           \
  {                                                                            \
    for (auto _ : state)                                                       \
      to_longlong_null(ITEM);                                                  \
  }                                                                            \
  BENCHMARK(BM_variant_##NAME##_to_longlong_null);                             \
                                                                               \
  void BM_variant_##NAME##_to_int32_null(benchmark::State &state)              \
  {                                                                            \
    for (auto _ : state)                                                       \
      to_int32_null(ITEM);                                                     \
  }                                                                            \
  BENCHMARK(BM_variant_##NAME##_to_int32_null);                                \
                                                                               \
  void BM_variant_##NAME##_to_double_null(benchmark::State &state)             \
  {                                                                            \
    for (auto _ : state)                                                       \
      to_double_null(ITEM);                                                    \
  }                                                                            \
  BENCHMARK(BM_variant_##NAME##_to_double_null);

namespace variant
{
::variant::Item g_int1{0x77};
::variant::Item g_int2{0x77};
::variant::Item g_uminus= ::variant::Item_func_uminus(&g_int1);
::variant::Item g_isnull= ::variant::Item_func_isnull(&g_int1);
::variant::Item g_add= ::variant::Item_func_add(&g_uminus, &g_isnull);

VARIANT_TEST_SET(Item_int, g_int1);
VARIANT_TEST_SET(Item_func_uminus, g_uminus);
VARIANT_TEST_SET(Item_func_isnull, g_isnull);
VARIANT_TEST_SET(Item_func_add, g_add);
} // namespace variant

} // namespace

BENCHMARK_MAIN();
