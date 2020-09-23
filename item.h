#ifndef ITEM_H
#define ITEM_H

#include <cassert>

#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "variant.hpp"

#include "mysql_com.h"

#include "sql_type_bool.h"
#include "sql_type_double.h"
#include "sql_type_int.h"

#include "opt.h"

#include "vm.h"

using namespace std;



class Stat
{
public:
  longlong sum_ll;
  double sum_d;
  double time_spent;
  const char *method;
  bool use_sum_d;
  Stat()
   :sum_ll(0), sum_d(0), time_spent(0), method(""), use_sum_d(false)
  { }
};


class StatAll
{
public:
  Stat val_xxx;
  Stat val_xxx_null;
  Stat get_xxx;
  Stat to_xxx_null;
  StatAll(const Stat &a, const Stat &b, const Stat &c, const Stat &d)
   :val_xxx(a),
    val_xxx_null(b),
    get_xxx(c),
    to_xxx_null(d)
  { }
};


class MethodStat
{
public:
  double val_xxx;
  double val_xxx_null;
  double get_xxx;
  double to_xxx_null;
  MethodStat(const StatAll &st)
   :val_xxx(st.val_xxx.time_spent),
    val_xxx_null(st.val_xxx_null.time_spent),
    get_xxx(st.get_xxx.time_spent),
    to_xxx_null(st.to_xxx_null.time_spent)
  { }
  MethodStat(double a, double b, double c, double d)
   :val_xxx(a), val_xxx_null(b), get_xxx(c), to_xxx_null(d)
  { }
  MethodStat(): MethodStat(0, 0, 0, 0) { }
  MethodStat operator+(const MethodStat &st) const
  {
    return MethodStat(val_xxx      + st.val_xxx,
                      val_xxx_null + st.val_xxx_null,
                      get_xxx      + st.get_xxx,
                      to_xxx_null  + st.to_xxx_null);
  }
  MethodStat &operator+=(const MethodStat &st)
  {
    val_xxx+=      st.val_xxx;
    val_xxx_null+= st.val_xxx_null;
    get_xxx+=      st.get_xxx;
    to_xxx_null+=  st.to_xxx_null;
    return *this;
  }
  bool is_empty() const
  {
    return
      val_xxx      == 0 &&
      val_xxx_null == 0 &&
      get_xxx      == 0 &&
      to_xxx_null  == 0;
  }
  void print(const char *method) const;
  void print_if(const char *method) const
  {
    if (!is_empty())
      print(method);
  }
};


class MethodStatByType
{
public:
  MethodStat st_bool;
  MethodStat st_int32;
  MethodStat st_longlong;
  MethodStat st_double;
  MethodStatByType operator+=(const MethodStatByType &st)
  {
    st_bool+= st.st_bool;
    st_int32+= st.st_int32;
    st_longlong+= st.st_longlong;
    st_double+= st.st_double;
    return *this;
  }
  MethodStat total() const
  {
    MethodStat st;
    st+= st_bool;
    st+= st_int32;
    st+= st_longlong;
    st+= st_double;
    return st;
  }
  void print() const;
};


class Item
{
  char filler[0x7E];
public:
#ifdef HAVE_NULL_VALUE
  bool null_value;
  virtual bool val_bool()= 0;
  virtual double val_real()= 0;
  virtual longlong val_int()= 0;
  virtual int32 val_int32()= 0;
  Stat test_b_old(const Options &opt);
  Stat test_d_old(const Options &opt);
  Stat test_int32_old(const Options &opt);
  Stat test_ll_old(const Options &opt);
  Stat test_native_old(const Options &opt);
#endif
  virtual ~Item() { }

  virtual enum_field_types field_type() const= 0;

  virtual void print(string *to)= 0;

  virtual bool gen(VM *to) { return true; }

  virtual bool val_bool_null(bool *null_value_arg)= 0;
  virtual longlong val_int_null(bool *null_value_arg)= 0;
  virtual int32 val_int32_null(bool *null_value_arg)= 0;
  virtual double val_real_null(bool *null_value_arg)= 0;

  virtual bool get_bool(bool *to)= 0;
  virtual bool get_longlong(longlong *to)= 0;
  virtual bool get_int32(int32 *to)= 0;
  virtual bool get_double(double *to)= 0;

  virtual Bool_null to_bool_null()= 0;
  virtual Longlong_null to_longlong_null()= 0;
  virtual Int32_null to_int32_null()= 0;
  virtual Double_null to_double_null()= 0;

  Stat test_native_prm(const Options &opt);
  Stat test_native_get(const Options &opt);
  Stat test_native_new(const Options &opt);

  Stat test_b_prm(const Options &opt);
  Stat test_b_get(const Options &opt);
  Stat test_b_new(const Options &opt);

  Stat test_d_prm(const Options &opt);
  Stat test_d_get(const Options &opt);
  Stat test_d_new(const Options &opt);

  Stat test_int32_prm(const Options &opt);
  Stat test_int32_get(const Options &opt);
  Stat test_int32_new(const Options &opt);

  Stat test_ll_prm(const Options &opt);
  Stat test_ll_get(const Options &opt);
  Stat test_ll_new(const Options &opt);

  Stat test_ll_vm(VM *vm, const Options &opt);
  Stat test_d_vm(VM *vm, const Options &opt);
};

class Item_null: public Item
{
public:
  Item_null()
  {
#ifdef HAVE_NULL_VALUE
    null_value= true;
#endif
  }
  enum_field_types field_type() const override { return MYSQL_TYPE_NULL; }
  void print(string *to) override;

#ifdef HAVE_NULL_VALUE
  bool val_bool() override
  {
    return false;
  }
  longlong val_int() override
  {
    return 0;
  }
  int32 val_int32() override
  {
    return 0;
  }
  double val_real() override
  {
    return 0e0;
  }
#endif
  bool val_bool_null(bool *null_value_arg) override
  {
    *null_value_arg= true;
    return false;
  }
  longlong val_int_null(bool *null_value_arg) override
  {
    *null_value_arg= true;
    return 0;
  }
  int32 val_int32_null(bool *null_value_arg) override
  {
    *null_value_arg= true;
    return 0;
  }
  double val_real_null(bool *null_value_arg) override
  {
    *null_value_arg= true;
    return 0e0;
  }

  bool get_bool(bool *to) override { return true; }
  bool get_longlong(longlong *to) override { return true; }
  bool get_int32(int32 *to) override { return true; }
  bool get_double(double *to) override { return true; }

  Bool_null to_bool_null() override
  {
    return Bool_null();
  }
  Longlong_null to_longlong_null() override
  {
    return Longlong_null();
  }
  Int32_null to_int32_null() override
  {
    return Int32_null();
  }
  Double_null to_double_null() override
  {
    return Double_null();
  }
};


class Item_bool: public Item
{
  Bool_null bnval;
  bool val;
public:
  Item_bool(bool val_arg)
   :bnval(val_arg), val(val_arg)
  {
#ifdef HAVE_NULL_VALUE
    null_value= false;
#endif
  }
  enum_field_types field_type() const override { return MYSQL_TYPE_BOOL; }
  void print(string *to) override;

#ifdef HAVE_NULL_VALUE
  bool val_bool() override
  {
    return val;
  }
  longlong val_int() override
  {
    return val;
  }
  int32 val_int32() override
  {
    return val;
  }
  double val_real() override
  {
    return val;
  }
#endif
  bool val_bool_null(bool *null_value_arg) override
  {
    *null_value_arg= false;
    return val;
  }
  longlong val_int_null(bool *null_value_arg) override
  {
    *null_value_arg= false;
    return val;
  }
  int32 val_int32_null(bool *null_value_arg) override
  {
    *null_value_arg= false;
    return val;
  }
  double val_real_null(bool *null_value_arg) override
  {
    *null_value_arg= false;
    return val;
  }

  bool get_bool(bool *to) override
  { 
    *to= val;
    return false;
  }
  bool get_longlong(longlong *to) override
  { 
    *to= val;
    return false;
  }
  bool get_int32(int32 *to) override
  { 
    *to= val;
    return false;
  }
  bool get_double(double *to) override
  { 
    *to= val;
    return false;
  }

  Bool_null to_bool_null() override
  {
    return bnval;//Bool_null(val);
  }
  Longlong_null to_longlong_null() override
  {
    return Longlong_null(val);
  }
  Int32_null to_int32_null() override
  {
    return Int32_null(val);
  }
  Double_null to_double_null() override
  {
    return Double_null(val);
  }
};



class Item_int: public Item
{
  longlong val;
public:
  Item_int(longlong val_arg)
   :val(val_arg)
  {
#ifdef HAVE_NULL_VALUE
    null_value= false;
#endif
  }
  enum_field_types field_type() const override { return MYSQL_TYPE_LONGLONG; }
  void print(string *to) override;

#ifdef HAVE_NULL_VALUE
  bool val_bool() override
  {
    return (bool) val;
  }
  longlong val_int() override
  {
    return val;
  }
  int32 val_int32() override
  {
    return (int32) val;
  }
  double val_real() override
  {
    return (double) val;
  }
#endif
  bool val_bool_null(bool *null_value_arg) override
  {
    *null_value_arg= false;
    return (bool) val;
  }
  longlong val_int_null(bool *null_value_arg) override
  {
    *null_value_arg= false;
    return val;
  }
  int32 val_int32_null(bool *null_value_arg) override
  {
    *null_value_arg= false;
    return val;
  }
  double val_real_null(bool *null_value_arg) override
  {
    *null_value_arg= false;
    return (double) val;
  }

  bool get_bool(bool *to) override
  { 
    *to= val;
    return false;
  }
  bool get_longlong(longlong *to) override
  { 
    *to= val;
    return false;
  }
  bool get_int32(int32 *to) override
  { 
    *to= (int32) val;
    return false;
  }
  bool get_double(double *to) override
  { 
    *to= val;
    return false;
  }

  Bool_null to_bool_null() override
  {
    return Bool_null((bool) val);
  }
  Longlong_null to_longlong_null() override
  {
    return Longlong_null(val);
  }
  Int32_null to_int32_null() override
  {
    return Int32_null(val);
  }
  Double_null to_double_null() override
  {
    return Double_null((double) val);
  }
};


class Item_real: public Item
{
  double val;
public:
  Item_real(double val_arg)
   :val(val_arg)
  {
#ifdef HAVE_NULL_VALUE
    null_value= false;
#endif
  }
  enum_field_types field_type() const override { return MYSQL_TYPE_DOUBLE; }
  void print(string *to) override;

#ifdef HAVE_NULL_VALUE
  bool val_bool() override
  {
    return (bool) val;
  }
  double val_real() override
  {
    return val;
  }
  longlong val_int() override
  {
    return (longlong) val;
  }
  int32 val_int32() override
  {
    return (int32) val;
  }
#endif
  bool val_bool_null(bool *null_value_arg) override
  {
    *null_value_arg= false;
    return (bool) val;
  }
  double val_real_null(bool *null_value_arg) override
  {
    *null_value_arg= false;
    return val;
  }
  longlong val_int_null(bool *null_value_arg) override
  {
    *null_value_arg= false;
    return (longlong) val;
  }
  int32 val_int32_null(bool *null_value_arg) override
  {
    *null_value_arg= false;
    return (int32) val;
  }

  bool get_bool(bool *to) override
  { 
    *to= val;
    return false;
  }
  bool get_longlong(longlong *to) override
  { 
    *to= val;
    return false;
  }
  bool get_int32(int32 *to) override
  { 
    *to= (int32) val;
    return false;
  }
  bool get_double(double *to) override
  { 
    *to= val;
    return false;
  }

  Bool_null to_bool_null() override
  {
    return Bool_null((bool) val);
  }
  Double_null to_double_null() override
  {
    return Double_null(val);
  }
  Longlong_null to_longlong_null() override
  {
    return Longlong_null((longlong) val);
  }
  Int32_null to_int32_null() override
  {
    return Int32_null((int32) val);
  }
};


Item *generate_node(size_t depth_left,
                    std::vector<std::unique_ptr<Item>> &storage);

static inline std::pair<Item *, std::vector<std::unique_ptr<Item>>>
generate_tree(size_t max_depth)
{
  assert(max_depth > 0);

  std::vector<std::unique_ptr<Item>> storage;

  Item *root= generate_node(max_depth, storage);

  // std::string as_string;
  // root->print(&as_string);
  // std::cout << "                " << as_string << '\n';

  assert(!storage.empty());
  return {root, std::move(storage)};
}

namespace variant
{

struct Item_int;
struct Item_func_uminus;
struct Item_func_isnull;
struct Item_func_add;

using Item= mpark::variant<Item_int, Item_func_uminus, Item_func_isnull, Item_func_add>;

struct Item_int
{
  longlong val;

  Item_int(longlong val) : val(val) {}

  enum_field_types field_type() const { return MYSQL_TYPE_LONGLONG; }

  Bool_null to_bool_null() { return Bool_null(static_cast<bool>(val)); }
  Longlong_null to_longlong_null() { return Longlong_null(val); }
  Int32_null to_int32_null() { return Int32_null(static_cast<int32>(val)); }
  Double_null to_double_null() { return Double_null(val); }
};

struct Item_func
{
  Item *args[3];
  uint arg_count;
  Item_func(Item *a) : args{a, nullptr, nullptr}, arg_count(1) {}
  Item_func(Item *a, Item *b) : args{a, b, nullptr}, arg_count(2) {}
  Item_func(Item *a, Item *b, Item *c) : args{a, b, c}, arg_count(3) {}

  uint argument_count() const { return arg_count; }
  Item **arguments() { return args; }
};

struct Hybrid_field_type
{
  enum_field_types m_field_type;

  Hybrid_field_type(enum_field_types t) : m_field_type(t) {}
  Hybrid_field_type(enum_field_types ta, enum_field_types tb)
  {
    if (ta == tb)
      m_field_type= ta;
    else if (ta == MYSQL_TYPE_NULL || tb == MYSQL_TYPE_NULL)
      m_field_type= std::max(ta, tb);
    else if (ta == MYSQL_TYPE_DOUBLE || tb == MYSQL_TYPE_DOUBLE)
      m_field_type= MYSQL_TYPE_DOUBLE;
    else
      m_field_type= MYSQL_TYPE_LONGLONG;
  }
  Hybrid_field_type(enum_field_types ta, enum_field_types tb,
                    enum_field_types tc)
      : Hybrid_field_type(Hybrid_field_type(ta, tb).m_field_type, tc)
  {
  }
};

struct Item_hybrid_func : Item_func, Hybrid_field_type
{
  inline Item_hybrid_func(Item *a);
  inline Item_hybrid_func(Item *a, Item *b);
  inline Item_hybrid_func(Item *a, Item *b, Item *c);
};

struct Item_func_add : Item_hybrid_func
{
  Item_func_add(Item *a, Item *b) : Item_hybrid_func(a, b) {}

  enum_field_types field_type() const { return m_field_type; }

  Bool_null to_bool_null();
  Longlong_null to_longlong_null();
  Int32_null to_int32_null();
  Double_null to_double_null();
};

struct Item_func_uminus : Item_hybrid_func
{
  Item_func_uminus(Item *a) : Item_hybrid_func(a) {}

  enum_field_types field_type() const { return m_field_type; }

  Bool_null to_bool_null();
  Longlong_null to_longlong_null();
  Int32_null to_int32_null();
  Double_null to_double_null();
};

struct Item_func_isnull : Item_func
{
  enum_field_types m_arg0_field_type;

  inline Item_func_isnull(Item *a);

  enum_field_types field_type() const { return MYSQL_TYPE_BOOL; }

  Bool_null to_bool_null();

  Longlong_null to_longlong_null()
  {
    Bool_null val= Item_func_isnull::to_bool_null();
    return Longlong_null(val.value, val.is_null);
  }

  Int32_null to_int32_null()
  {
    Bool_null val= Item_func_isnull::to_bool_null();
    return Int32_null(val.value, val.is_null);
  }

  Double_null to_double_null()
  {
    Bool_null val= Item_func_isnull::to_bool_null();
    return Double_null(val.value, val.is_null);
  }
};

#define SWITCH_CALL_RETURN(ITEM, METHOD)                                       \
  switch (ITEM.index())                                                        \
  {                                                                            \
  case 0:                                                                      \
    return mpark::get<Item_int>(ITEM).METHOD();                                \
  case 1:                                                                      \
    return mpark::get<Item_func_uminus>(ITEM).METHOD();                        \
  case 2:                                                                      \
    return mpark::get<Item_func_isnull>(ITEM).METHOD();                        \
  case 3:                                                                      \
    return mpark::get<Item_func_add>(ITEM).METHOD();                           \
  default:                                                                     \
    __builtin_unreachable();                                                   \
  }

static inline enum_field_types field_type(Item &item)
{
  SWITCH_CALL_RETURN(item, field_type);
};

static inline Bool_null to_bool_null(Item &item)
{
  SWITCH_CALL_RETURN(item, to_bool_null);
};

static inline Longlong_null to_longlong_null(Item &item)
{
  SWITCH_CALL_RETURN(item, to_longlong_null);
};

static inline Int32_null to_int32_null(Item &item)
{
  SWITCH_CALL_RETURN(item, to_int32_null);
};

static inline Double_null to_double_null(Item &item)
{
  SWITCH_CALL_RETURN(item, to_double_null);
};

Item_hybrid_func::Item_hybrid_func(Item *a)
    : Item_func(a), Hybrid_field_type(field_type(*a))
{
}

Item_hybrid_func::Item_hybrid_func(Item *a, Item *b)
    : Item_func(a, b), Hybrid_field_type(field_type(*a), field_type(*b))
{
}

Item_hybrid_func::Item_hybrid_func(Item *a, Item *b, Item *c)
    : Item_func(a, b, c),
      Hybrid_field_type(field_type(*a), field_type(*b), field_type(*c))
{
}

Item_func_isnull::Item_func_isnull(Item *a)
    : Item_func(a), m_arg0_field_type(variant::field_type(*a))
{
}

} // namespace variant

#endif
