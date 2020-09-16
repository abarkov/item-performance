#ifndef ITEM_H
#define ITEM_H


#define likely(x)      __builtin_expect(!!(x), 1) 
#define unlikely(x)    __builtin_expect(!!(x), 0) 

#include <iostream>
#include <string>

#if defined (__clang__)
#define COMPILER "clang"
#elif defined(__GNUC__)
#define COMPILER "gcc"
#endif

#include "sql_type_bool.h"
#include "sql_type_double.h"
#include "sql_type_int.h"

using namespace std;

enum enum_field_types
{
  MYSQL_TYPE_NULL=       0,
  MYSQL_TYPE_BOOL=       1,
  MYSQL_TYPE_LONGLONG=   2,
  MYSQL_TYPE_DOUBLE=     3
};


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
  void print(class Item *item);
};


class MethodStat
{
public:
  double val_xxx;
  double val_xxx_null;
  double get_xxx;
  double to_xxx_null;
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
  Stat test_b_old(ulonglong count);
  Stat test_d_old(ulonglong count);
  Stat test_int32_old(ulonglong count);
  Stat test_ll_old(ulonglong count);
#endif
  virtual ~Item() { }

  virtual enum_field_types field_type() const= 0;

  virtual void print(string *to)= 0;

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

  Stat test_b_prm(ulonglong count);
  Stat test_b_get(ulonglong count);
  Stat test_b_new(ulonglong count);

  Stat test_d_prm(ulonglong count);
  Stat test_d_get(ulonglong count);
  Stat test_d_new(ulonglong count);

  Stat test_int32_prm(ulonglong count);
  Stat test_int32_get(ulonglong count);
  Stat test_int32_new(ulonglong count);

  Stat test_ll_prm(ulonglong count);
  Stat test_ll_get(ulonglong count);
  Stat test_ll_new(ulonglong count);
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

#endif
