#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <sys/time.h>
#include "timer.h"

#include "item.h"
#include "item_func.h"

class Stat
{
public:
  longlong sum_ll;
  double sum_d;
  double time_spent;
  Stat(): sum_ll(0), sum_d(0), time_spent(0) { }

  void print_b(Item *item, const char *method)
  {
    string s;
    item->print(&s);
    printf("%s %-16s %f %s %lld\n", COMPILER, method, time_spent, s.c_str(), sum_ll);
  }

  void print_ll(Item *item, const char *method)
  {
    string s;
    item->print(&s);
    printf("%s %-16s %f %s %lld\n", COMPILER, method, time_spent, s.c_str(), sum_ll);
  }

  void print_int32(Item *item, const char *method)
  {
    string s;
    item->print(&s);
    printf("%s %-16s %f %s %lld\n", COMPILER, method, time_spent, s.c_str(), sum_ll);
  }

  void print_d(Item *item, const char *method)
  {
    string s;
    item->print(&s);
    printf("%s %-16s %f %s %.2f\n", COMPILER, method, time_spent, s.c_str(), sum_d);
  }

};


double Item::test_b_old(ulonglong count)
{
#ifdef HAVE_NULL_VALUE
  Stat st;
  Timer t0;
  for (ulonglong i= 0 ; i < count; i++)
  {
    bool b= val_bool();
    if (!null_value)
      st.sum_ll+= b;
  }
  st.time_spent= Timer().diff(t0);
  st.print_b(this, "val_bool");
  return st.time_spent;
#endif
  return 0;
}


double Item::test_d_old(ulonglong count)
{
#ifdef HAVE_NULL_VALUE
  Stat st;
  Timer t0;
  for (ulonglong i= 0 ; i < count; i++)
  {
    double dbl= val_real();
    if (!null_value)
      st.sum_d+= dbl;
  }
  st.time_spent= Timer().diff(t0);
  st.print_d(this, "val_real");
  return st.time_spent;
#endif
  return 0;
}


double Item::test_ll_old(ulonglong count)
{
#ifdef HAVE_NULL_VALUE
  Stat st;
  Timer t0;
  for (ulonglong i= 0 ; i < count; i++)
  {
    longlong tmp= val_int();
    if (!null_value)
      st.sum_ll+= tmp;
  }
  st.time_spent= Timer().diff(t0);
  st.print_ll(this, "val_int");
  return st.time_spent;
#endif
  return 0;
}



double Item::test_int32_old(ulonglong count)
{
#ifdef HAVE_NULL_VALUE
  Stat st;
  Timer t0;
  for (ulonglong i= 0 ; i < count; i++)
  {
    int32 tmp= val_int32();
    if (!null_value)
      st.sum_ll+= tmp;
  }
  st.time_spent= Timer().diff(t0);
  st.print_int32(this, "val_int32");
  return st.time_spent;
#endif
  return 0;
}



double Item::test_b_prm(ulonglong count)
{
  Stat st;
  Timer t0;
  for (ulonglong i= 0 ; i < count; i++)
  {
    bool tmp_null_value;
    bool b= val_bool_null(&tmp_null_value);
    if (!tmp_null_value)
      st.sum_ll+= b;
  }
  st.time_spent= Timer().diff(t0);
  st.print_b(this, "val_bool_null");
  return st.time_spent;
}


double Item::test_b_get(ulonglong count)
{
  Stat st;
  Timer t0;
  for (ulonglong i= 0 ; i < count; i++)
  {
    bool tmp;
    if (!get_bool(&tmp))
      st.sum_ll+= tmp;
  }
  st.time_spent= Timer().diff(t0);
  st.print_b(this, "get_bool");
  return st.time_spent;
}



double Item::test_d_prm(ulonglong count)
{
  Stat st;
  Timer t0;
  for (ulonglong i= 0 ; i < count; i++)
  {
    bool tmp_null_value;
    double dbl= val_real_null(&tmp_null_value);
    if (!tmp_null_value)
      st.sum_d+= dbl;
  }
  st.time_spent= Timer().diff(t0);
  st.print_d(this, "val_real_null");
  return st.time_spent;
}


double Item::test_d_get(ulonglong count)
{
  Stat st;
  Timer t0;
  for (ulonglong i= 0 ; i < count; i++)
  {
    double tmp;
    if (!get_double(&tmp))
      st.sum_d+= tmp;
  }
  st.time_spent= Timer().diff(t0);
  st.print_d(this, "get_double");
  return st.time_spent;
}


double Item::test_ll_prm(ulonglong count)
{
  Stat st;
  Timer t0;
  for (ulonglong i= 0 ; i < count; i++)
  {
    bool tmp_null_value;
    longlong tmp= val_int_null(&tmp_null_value);
    if (!tmp_null_value)
      st.sum_ll+= tmp;
  }
  st.time_spent= Timer().diff(t0);
  st.print_ll(this, "val_int_null");
  return st.time_spent;
}


double Item::test_int32_prm(ulonglong count)
{
  Stat st;
  Timer t0;
  for (ulonglong i= 0 ; i < count; i++)
  {
    bool tmp_null_value;
    int32 tmp= val_int32_null(&tmp_null_value);
    if (!tmp_null_value)
      st.sum_ll+= tmp;
  }
  st.time_spent= Timer().diff(t0);
  st.print_int32(this, "val_int32_null");
  return st.time_spent;
}


double Item::test_ll_get(ulonglong count)
{
  Stat st;
  Timer t0;
  for (ulonglong i= 0 ; i < count; i++)
  {
    longlong tmp;
    if (!get_longlong(&tmp))
      st.sum_ll+= tmp;
  }
  st.time_spent= Timer().diff(t0);
  st.print_ll(this, "get_longlong");
  return st.time_spent;
}


double Item::test_int32_get(ulonglong count)
{
  Stat st;
  Timer t0;
  for (ulonglong i= 0 ; i < count; i++)
  {
    int32 tmp;
    if (!get_int32(&tmp))
      st.sum_ll+= tmp;
  }
  st.time_spent= Timer().diff(t0);
  st.print_int32(this, "get_int32");
  return st.time_spent;
}


double Item::test_b_new(ulonglong count)
{
  Stat st;
  Timer t0;
  for (ulonglong i= 0; i < count; i++)
  {
    Bool_null res= to_bool_null();
    if (!res.is_null)
      st.sum_ll+= res.value;
  }
  st.time_spent= Timer().diff(t0);
  st.print_b(this, "to_bool_null");
  return st.time_spent;
}


double Item::test_d_new(ulonglong count)
{
  Stat st;
  Timer t0;
  for (ulonglong i= 0; i < count; i++)
  {
    Double_null res= to_double_null();
    if (!res.is_null)
      st.sum_d+= res.value;
  }
  st.time_spent= Timer().diff(t0);
  st.print_d(this, "to_double_null");
  return st.time_spent;
}


double Item::test_ll_new(ulonglong count)
{
  Stat st;
  Timer t0;
  for (ulonglong i= 0; i < count; i++)
  {
    Longlong_null res= to_longlong_null();
    if (!res.is_null)
      st.sum_ll+= res.value;
  }
  st.time_spent= Timer().diff(t0);
  st.print_ll(this, "to_longlong_null");
  return st.time_spent;
}


double Item::test_int32_new(ulonglong count)
{
  Stat st;
  Timer t0;
  for (ulonglong i= 0; i < count; i++)
  {
    Int32_null res= to_int32_null();
    if (!res.is_null)
      st.sum_ll+= res.value;
  }
  st.time_spent= Timer().diff(t0);
  st.print_int32(this, "to_int32_null");
  return st.time_spent;
}
