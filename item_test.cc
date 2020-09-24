#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <sys/time.h>
#include "timer.h"

#include "item.h"
#include "item_func.h"
#include "vm.h"

#ifdef HAVE_NULL_VALUE
Stat Item::test_b_old(const Options &opt)
{
  if (opt.vm())
  {
    VM vm;
    if (!gen(&vm))
      return test_b_vm(&vm, opt);
  }
  Stat st;
  Timer t0;
  for (ulonglong i= 0, count= opt.count() ; i < count; i++)
  {
    bool b= val_bool();
    if (!null_value)
      st.sum_ll+= b;
  }
  st.time_spent= Timer().diff(t0);
  st.method= "val_bool";
  return st;
}
#endif


#ifdef HAVE_NULL_VALUE
Stat Item::test_d_old(const Options &opt)
{
  if (opt.vm())
  {
    VM vm;
    if (!gen(&vm))
      return test_d_vm(&vm, opt);
  }
  Stat st;
  Timer t0;
  for (ulonglong i= 0, count= opt.count() ; i < count; i++)
  {
    double dbl= val_real();
    if (!null_value)
      st.sum_d+= dbl;
  }
  st.time_spent= Timer().diff(t0);
  st.method= "val_real";
  st.use_sum_d= true;
  return st;
}
#endif


#ifdef HAVE_NULL_VALUE

Stat Item::test_ll_old(const Options &opt)
{
  if (opt.vm())
  {
    VM vm;
    if (!gen(&vm))
      return test_ll_vm(&vm, opt);
  }
  Stat st;
  Timer t0;
  for (ulonglong i= 0, count= opt.count() ; i < count; i++)
  {
    longlong tmp= val_int();
    if (!null_value)
      st.sum_ll+= tmp;
  }
  st.time_spent= Timer().diff(t0);
  st.method= "val_int";
  return st;
}
#endif


#ifdef HAVE_NULL_VALUE
Stat Item::test_int32_old(const Options &opt)
{
  Stat st;
  Timer t0;
  for (ulonglong i= 0, count= opt.count() ; i < count; i++)
  {
    int32 tmp= val_int32();
    if (!null_value)
      st.sum_ll+= tmp;
  }
  st.time_spent= Timer().diff(t0);
  st.method= "val_int32";
  return st;
}
#endif


#ifdef HAVE_NULL_VALUE
Stat Item::test_native_old(const Options &opt)
{
  switch (field_type())
  {
  case MYSQL_TYPE_NULL:
  case MYSQL_TYPE_BOOL:      return test_b_old(opt);
  case MYSQL_TYPE_LONGLONG:  return opt.int32api() ?
                                    test_int32_old(opt) :
                                    test_ll_old(opt);
  case MYSQL_TYPE_DOUBLE:    return test_d_old(opt);
  }
  return Stat();
}
#endif



Stat Item::test_b_prm(const Options &opt)
{
  Stat st;
  Timer t0;
  for (ulonglong i= 0, count= opt.count() ; i < count; i++)
  {
    bool tmp_null_value;
    bool b= val_bool_null(&tmp_null_value);
    if (!tmp_null_value)
      st.sum_ll+= b;
  }
  st.time_spent= Timer().diff(t0);
  st.method= "val_bool_null";
  return st;
}


Stat Item::test_b_get(const Options &opt)
{
  Stat st;
  Timer t0;
  for (ulonglong i= 0, count= opt.count() ; i < count; i++)
  {
    bool tmp;
    if (!get_bool(&tmp))
      st.sum_ll+= tmp;
  }
  st.time_spent= Timer().diff(t0);
  st.method= "get_bool";
  return st;
}



Stat Item::test_d_prm(const Options &opt)
{
  Stat st;
  Timer t0;
  for (ulonglong i= 0, count= opt.count() ; i < count; i++)
  {
    bool tmp_null_value;
    double dbl= val_real_null(&tmp_null_value);
    if (!tmp_null_value)
      st.sum_d+= dbl;
  }
  st.time_spent= Timer().diff(t0);
  st.method= "val_real_null";
  st.use_sum_d= true;
  return st;
}


Stat Item::test_d_get(const Options &opt)
{
  Stat st;
  Timer t0;
  for (ulonglong i= 0, count= opt.count() ; i < count; i++)
  {
    double tmp;
    if (!get_double(&tmp))
      st.sum_d+= tmp;
  }
  st.time_spent= Timer().diff(t0);
  st.method= "get_double";
  st.use_sum_d= true;
  return st;
}


Stat Item::test_ll_prm(const Options &opt)
{
  Stat st;
  Timer t0;
  for (ulonglong i= 0, count= opt.count() ; i < count; i++)
  {
    bool tmp_null_value;
    longlong tmp= val_int_null(&tmp_null_value);
    if (!tmp_null_value)
      st.sum_ll+= tmp;
  }
  st.time_spent= Timer().diff(t0);
  st.method= "val_int_null";
  return st;
}


Stat Item::test_int32_prm(const Options &opt)
{
  Stat st;
  Timer t0;
  for (ulonglong i= 0, count= opt.count() ; i < count; i++)
  {
    bool tmp_null_value;
    int32 tmp= val_int32_null(&tmp_null_value);
    if (!tmp_null_value)
      st.sum_ll+= tmp;
  }
  st.time_spent= Timer().diff(t0);
  st.method= "val_int32_null";
  return st;
}


Stat Item::test_ll_get(const Options &opt)
{
  Stat st;
  Timer t0;
  for (ulonglong i= 0, count= opt.count() ; i < count; i++)
  {
    longlong tmp;
    if (!get_longlong(&tmp))
      st.sum_ll+= tmp;
  }
  st.time_spent= Timer().diff(t0);
  st.method= "get_longlong";
  return st;
}


Stat Item::test_int32_get(const Options &opt)
{
  Stat st;
  Timer t0;
  for (ulonglong i= 0, count= opt.count() ; i < count; i++)
  {
    int32 tmp;
    if (!get_int32(&tmp))
      st.sum_ll+= tmp;
  }
  st.time_spent= Timer().diff(t0);
  st.method= "get_int32";
  return st;
}


Stat Item::test_b_new(const Options &opt)
{
  Stat st;
  Timer t0;
  for (ulonglong i= 0, count= opt.count() ; i < count; i++)
  {
    Bool_null res= to_bool_null();
    if (!res.is_null)
      st.sum_ll+= res.value;
  }
  st.time_spent= Timer().diff(t0);
  st.method= "to_bool_null";
  return st;
}


Stat Item::test_d_new(const Options &opt)
{
  Stat st;
  Timer t0;
  for (ulonglong i= 0, count= opt.count() ; i < count; i++)
  {
    Double_null res= to_double_null();
    if (!res.is_null)
      st.sum_d+= res.value;
  }
  st.time_spent= Timer().diff(t0);
  st.method= "to_double_null";
  st.use_sum_d= true;
  return st;
}


Stat Item::test_ll_new(const Options &opt)
{
  Stat st;
  Timer t0;
  for (ulonglong i= 0, count= opt.count() ; i < count; i++)
  {
    Longlong_null res= to_longlong_null();
    if (!res.is_null)
      st.sum_ll+= res.value;
  }
  st.time_spent= Timer().diff(t0);
  st.method= "to_longlong_null";
  return st;
}


Stat Item::test_int32_new(const Options &opt)
{
  Stat st;
  Timer t0;
  for (ulonglong i= 0, count= opt.count() ; i < count; i++)
  {
    Int32_null res= to_int32_null();
    if (!res.is_null)
      st.sum_ll+= res.value;
  }
  st.time_spent= Timer().diff(t0);
  st.method= "to_int32_null";
  return st;
}


/********************************************************/

Stat Item::test_ll_vm(VM *vm, const Options &opt)
{
  Stat st;
  Timer t0;
  for (ulonglong i= 0, count= opt.count() ; i < count; i++)
  {
    vm->exec();
    if (!vm->m_ll0.is_null)
      st.sum_ll+= vm->m_ll0.value;
  }
  st.time_spent= Timer().diff(t0);
  st.method= "vm_longlong";
  return st;
}


Stat Item::test_b_vm(VM *vm, const Options &opt)
{
  Stat st;
  Timer t0;
  for (ulonglong i= 0, count= opt.count() ; i < count; i++)
  {
    vm->exec();
    if (!vm->m_b0.is_null)
      st.sum_ll+= vm->m_b0.value;
  }
  st.time_spent= Timer().diff(t0);
  st.method= "vm_bool";
  return st;
}


Stat Item::test_d_vm(VM *vm, const Options &opt)
{
  Stat st;
  Timer t0;
  for (ulonglong i= 0, count= opt.count() ; i < count; i++)
  {
    vm->exec();
    if (!vm->m_d0.is_null)
      st.sum_d+= vm->m_d0.value;
  }
  st.time_spent= Timer().diff(t0);
  st.method= "vm_double";
  st.use_sum_d= true;
  return st;
}


/***************************************************/

Stat Item::test_native_prm(const Options &opt)
{
  switch (field_type())
  {
  case MYSQL_TYPE_NULL:
  case MYSQL_TYPE_BOOL:      return test_b_prm(opt);
  case MYSQL_TYPE_LONGLONG:  return opt.int32api() ?
                                    test_int32_prm(opt) :
                                    test_ll_prm(opt);
  case MYSQL_TYPE_DOUBLE:    return test_d_prm(opt);
  }
  return Stat();
}


Stat Item::test_native_get(const Options &opt)
{
  switch (field_type())
  {
  case MYSQL_TYPE_NULL:
  case MYSQL_TYPE_BOOL:      return test_b_get(opt);
  case MYSQL_TYPE_LONGLONG:  return opt.int32api() ?
                                    test_int32_get(opt) :
                                    test_ll_get(opt);
  case MYSQL_TYPE_DOUBLE:    return test_d_get(opt);
  }
  return Stat();
}


Stat Item::test_native_new(const Options &opt)
{
  switch (field_type())
  {
  case MYSQL_TYPE_NULL:
  case MYSQL_TYPE_BOOL:      return test_b_new(opt);
  case MYSQL_TYPE_LONGLONG:  return opt.int32api() ?
                                    test_int32_new(opt) :
                                    test_ll_new(opt);
  case MYSQL_TYPE_DOUBLE:    return test_d_new(opt);
  }
  return Stat();
}
