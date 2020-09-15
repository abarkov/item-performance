#ifndef ITEM_FUNC_H
#define ITEM_FUNC_H

#include "item.h"


class Item_func: public Item
{
protected:
  Item *args[3];
  uint arg_count;
  Item_func(Item *a) :arg_count(1)
  {
    args[0]= a;
  }
  Item_func(Item *a, Item *b) :arg_count(2)
  {
    args[0]= a;
    args[1]= b;
  }
  Item_func(Item *a, Item *b, Item *c) :arg_count(3)
  {
    args[0]= a;
    args[1]= b;
    args[2]= c;
  }
};


class Hybrid_field_type
{
protected:
  enum_field_types m_field_type;
  Hybrid_field_type(enum_field_types t)
   :m_field_type(t)
  { }
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
};


class Item_hybrid_func: public Item_func,
                        public Hybrid_field_type
{
public:
  Item_hybrid_func(Item *a)
   :Item_func(a),
    Hybrid_field_type(a->field_type())
  { }
  Item_hybrid_func(Item *a, Item *b)
   :Item_func(a, b),
    Hybrid_field_type(a->field_type(), b->field_type())
  { }
  Item_hybrid_func(Item *a, Item *b, Item *c)
   :Item_func(a, b, c),
    Hybrid_field_type(a->field_type(), b->field_type())
  { }
  enum_field_types field_type() const override { return m_field_type; }
};


class Item_func_add: public Item_hybrid_func
{
public:
  Item_func_add(Item *a, Item *b) :Item_hybrid_func(a,b) { }
  void print(string *to) override;
#ifdef HAVE_NULL_VALUE
  bool val_bool() override;
  double val_real() override;
  longlong val_int() override;
  int32 val_int32() override;
#endif

  bool val_bool_null(bool *null_value_arg) override;
  longlong val_int_null(bool *null_value_arg) override;
  int32 val_int32_null(bool *null_value_arg) override;
  double val_real_null(bool *null_value_arg) override;

  bool get_bool(bool *to) override;
  bool get_longlong(longlong *to) override;
  bool get_int32(int32 *to) override;
  bool get_double(double *to) override;

  Bool_null to_bool_null() override;
  Longlong_null to_longlong_null() override;
  Int32_null to_int32_null() override;
  Double_null to_double_null() override;
};


class Item_cond_or: public Item_func
{
public:
  Item_cond_or(Item *a, Item *b): Item_func(a, b) {  }
  Item_cond_or(Item *a, Item *b, Item *c): Item_func(a, b, c) { }
  enum_field_types field_type() const override
  {
    return MYSQL_TYPE_BOOL;
  }
  void print(string *to) override;
#ifdef HAVE_NULL_VALUE
  bool val_bool() override;
  double val_real() override
  {
    return Item_cond_or::val_bool();
  }
  longlong val_int() override
  {
    return Item_cond_or::val_bool();
  }
  int32 val_int32() override
  {
    return Item_cond_or::val_bool();
  }
#endif

  bool val_bool_null(bool *null_value_arg) override;
  longlong val_int_null(bool *null_value_arg) override;
  int32 val_int32_null(bool *null_value_arg) override;
  double val_real_null(bool *null_value_arg) override;

  bool get_bool(bool *to) override;
  bool get_longlong(longlong *to) override;
  bool get_int32(int32 *to) override;
  bool get_double(double *to) override;

  Bool_null to_bool_null() override;
  Longlong_null to_longlong_null() override;
  Int32_null to_int32_null() override;
  Double_null to_double_null() override;
};


class Item_func_uminus: public Item_func
{
public:
  Item_func_uminus(Item *a): Item_func(a) { }
  enum_field_types field_type() const override
  {
    return MYSQL_TYPE_BOOL;
  }
  void print(string *to) override;
#ifdef HAVE_NULL_VALUE
  bool val_bool() override;
  double val_real() override;
  longlong val_int() override;
  int32 val_int32() override;
#endif

  bool val_bool_null(bool *null_value_arg) override;
  longlong val_int_null(bool *null_value_arg) override;
  int32 val_int32_null(bool *null_value_arg) override;
  double val_real_null(bool *null_value_arg) override;

  bool get_bool(bool *to) override;
  bool get_longlong(longlong *to) override;
  bool get_int32(int32 *to) override;
  bool get_double(double *to) override;

  Bool_null to_bool_null() override;
  Longlong_null to_longlong_null() override;
  Int32_null to_int32_null() override;
  Double_null to_double_null() override;
};


class Item_func_isnull: public Item_func
{
  enum_field_types m_arg0_field_type;
public:
  Item_func_isnull(Item *a)
   :Item_func(a),
    m_arg0_field_type(a->field_type())
  { }
  enum_field_types field_type() const override
  {
    return MYSQL_TYPE_BOOL;
  }
  void print(string *to) override;
#ifdef HAVE_NULL_VALUE
  bool val_bool() override;
  double val_real() override
  {
    return Item_func_isnull::val_bool();
  }
  longlong val_int() override
  {
    return Item_func_isnull::val_bool();
  }
  int32 val_int32() override
  {
    return Item_func_isnull::val_bool();
  }
#endif

  bool val_bool_null(bool *null_value_arg) override;
  longlong val_int_null(bool *null_value_arg) override
  {
    return Item_func_isnull::val_bool_null(null_value_arg);
  }
  int32 val_int32_null(bool *null_value_arg) override
  {
    return Item_func_isnull::val_bool_null(null_value_arg);
  }
  double val_real_null(bool *null_value_arg) override
  {
    return Item_func_isnull::val_bool_null(null_value_arg);
  }

  bool get_bool(bool *to) override;
  bool get_longlong(longlong *to) override
  {
    bool tmp;
    bool is_null= Item_func_isnull::get_bool(&tmp);
    *to= tmp;
    return is_null;
  }
  bool get_int32(int32 *to) override
  {
    bool tmp;
    bool is_null= Item_func_isnull::get_bool(&tmp);
    *to= tmp;
    return is_null;
  }
  bool get_double(double *to) override
  {
    bool tmp;
    bool is_null= Item_func_isnull::get_bool(&tmp);
    *to= tmp;
    return is_null;
  }

  Bool_null to_bool_null() override;
  Longlong_null to_longlong_null() override
  {
    Bool_null val= Item_func_isnull::to_bool_null();
    return Longlong_null(val.value, val.is_null);
  }
  Int32_null to_int32_null() override
  {
    Bool_null val= Item_func_isnull::to_bool_null();
    return Int32_null(val.value, val.is_null);
  }
  Double_null to_double_null() override
  {
    Bool_null val= Item_func_isnull::to_bool_null();
    return Double_null(val.value, val.is_null);
  }
};


class Item_func_coalesce: public Item_hybrid_func
{
public:
  Item_func_coalesce(Item *a) :Item_hybrid_func(a) { }
  Item_func_coalesce(Item *a, Item *b) :Item_hybrid_func(a,b) { }
  Item_func_coalesce(Item *a, Item *b, Item *c) :Item_hybrid_func(a,b,c) { }
  void print(string *to) override;
#ifdef HAVE_NULL_VALUE
  bool val_bool() override;
  double val_real() override;
  longlong val_int() override;
  int32 val_int32() override;
#endif

  bool val_bool_null(bool *null_value_arg) override;
  longlong val_int_null(bool *null_value_arg) override;
  int32 val_int32_null(bool *null_value_arg) override;
  double val_real_null(bool *null_value_arg) override;

  bool get_bool(bool *to) override;
  bool get_longlong(longlong *to) override;
  bool get_int32(int32 *to) override;
  bool get_double(double *to) override;

  Bool_null to_bool_null() override;
  Longlong_null to_longlong_null() override;
  Int32_null to_int32_null() override;
  Double_null to_double_null() override;
};


class Item_func_last_value: public Item_hybrid_func
{
  Item *last_value;
  void fix_fields()
  {
    last_value= args[arg_count-1];
    m_field_type= last_value->field_type();
  }
public:
  Item_func_last_value(Item *a)
   :Item_hybrid_func(a) { fix_fields(); }
  Item_func_last_value(Item *a, Item *b)
   :Item_hybrid_func(a,b) { fix_fields(); }
  Item_func_last_value(Item *a, Item *b, Item *c)
   :Item_hybrid_func(a,b,c) { fix_fields(); }
  void print(string *to) override;
#ifdef HAVE_NULL_VALUE
  void evaluate_sideeffects_val_int();
  void evaluate_sideeffects_val_int32();
  bool val_bool() override;
  double val_real() override;
  longlong val_int() override;
  int32 val_int32() override;
#endif

  void evaluate_sideeffects_val_int_null();
  void evaluate_sideeffects_val_int32_null();
  bool val_bool_null(bool *null_value_arg) override;
  longlong val_int_null(bool *null_value_arg) override;
  int32 val_int32_null(bool *null_value_arg) override;
  double val_real_null(bool *null_value_arg) override;

  void evaluate_sideeffects_get_longlong();
  void evaluate_sideeffects_get_int32();
  bool get_bool(bool *to) override;
  bool get_longlong(longlong *to) override;
  bool get_int32(int32 *to) override;
  bool get_double(double *to) override;

  void evaluate_sideeffects_to_longlong_null();
  void evaluate_sideeffects_to_int32_null();
  Bool_null to_bool_null() override;
  Longlong_null to_longlong_null() override;
  Int32_null to_int32_null() override;
  Double_null to_double_null() override;
};



#endif
