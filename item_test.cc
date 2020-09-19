#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <sys/time.h>
#include "timer.h"

#include "item.h"
#include "item_func.h"


class VM
{
public:
  enum Cmd {
    NOP= 0,
    MOV_LL_TO_LL0,
    MOV_LL_TO_LL1,
    ADD_LL0_LL1
  };
  class Instr
  {
  public:
    Cmd m_cmd;
    Longlong_null m_param_ll;
    Instr()
     :m_cmd(NOP), m_param_ll(0, false)
    { }
    Instr(Cmd cmd)
     :m_cmd(cmd), m_param_ll(Longlong_null())
    { }
    Instr(Cmd cmd, const Longlong_null &param)
     :m_cmd(cmd), m_param_ll(param)
    { }
  };

  Instr m_instr[100];
  size_t m_instr_count;
  Longlong_null m_ll0;
  Longlong_null m_ll1;

  VM() :m_instr_count(0) { }
  bool add(const Instr &instr)
  {
    m_instr[m_instr_count]= instr;
    m_instr_count++;
    return false;
  }
  void exec_instr(const Instr &i)
  {
    switch (i.m_cmd) {
    case MOV_LL_TO_LL0:
      m_ll0= i.m_param_ll;
      break;
    case MOV_LL_TO_LL1:
      m_ll1= i.m_param_ll;
      break;
    case ADD_LL0_LL1:
      m_ll0= Longlong_null(m_ll0.value + m_ll1.value,
                           m_ll0.is_null | m_ll1.is_null);
      break;
    }
  }
  void exec()
  {
    for (size_t i= 0; i < m_instr_count; i++)
      exec_instr(m_instr[i]);
  }
};



#ifdef HAVE_NULL_VALUE
Stat Item::test_b_old(ulonglong count)
{
  Stat st;
  Timer t0;
  for (ulonglong i= 0 ; i < count; i++)
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
Stat Item::test_d_old(ulonglong count)
{
  Stat st;
  Timer t0;
  for (ulonglong i= 0 ; i < count; i++)
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

Stat Item::test_ll_old(ulonglong count)
{
  Item_func_add *add;
  Item_int *ia, *ib;
  if ((add= dynamic_cast<Item_func_add*>(this)) &&
      add->argument_count() == 2 &&
      (ia= dynamic_cast<Item_int*>(add->arguments()[0])) &&
      (ib= dynamic_cast<Item_int*>(add->arguments()[1])))
  {
    VM vm;
    vm.add(VM::Instr(VM::MOV_LL_TO_LL0, ia->to_longlong_null()));
    vm.add(VM::Instr(VM::MOV_LL_TO_LL1, ib->to_longlong_null()));
    vm.add(VM::Instr(VM::ADD_LL0_LL1));
    Stat st;
    Timer t0;
    for (ulonglong i= 0 ; i < count; i++)
    {
      vm.exec();
      if (!vm.m_ll0.is_null)
        st.sum_ll+= vm.m_ll0.value;
    }
    st.time_spent= Timer().diff(t0);
    st.method= "vm";
    return st;
  }
  Stat st;
  Timer t0;
  for (ulonglong i= 0 ; i < count; i++)
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
Stat Item::test_int32_old(ulonglong count)
{
  Stat st;
  Timer t0;
  for (ulonglong i= 0 ; i < count; i++)
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
Stat Item::test_native_old(ulonglong count)
{
  switch (field_type())
  {
  case MYSQL_TYPE_NULL:
  case MYSQL_TYPE_BOOL:      return test_b_old(count);
  case MYSQL_TYPE_LONGLONG:  return test_ll_old(count);
  case MYSQL_TYPE_DOUBLE:    return test_d_old(count);
  }
  return Stat();
}
#endif



Stat Item::test_b_prm(ulonglong count)
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
  st.method= "val_bool_null";
  return st;
}


Stat Item::test_b_get(ulonglong count)
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
  st.method= "get_bool";
  return st;
}



Stat Item::test_d_prm(ulonglong count)
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
  st.method= "val_real_null";
  st.use_sum_d= true;
  return st;
}


Stat Item::test_d_get(ulonglong count)
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
  st.method= "get_double";
  st.use_sum_d= true;
  return st;
}


Stat Item::test_ll_prm(ulonglong count)
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
  st.method= "val_int_null";
  return st;
}


Stat Item::test_int32_prm(ulonglong count)
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
  st.method= "val_int32_null";
  return st;
}


Stat Item::test_ll_get(ulonglong count)
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
  st.method= "get_longlong";
  return st;
}


Stat Item::test_int32_get(ulonglong count)
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
  st.method= "get_int32";
  return st;
}


Stat Item::test_b_new(ulonglong count)
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
  st.method= "to_bool_null";
  return st;
}


Stat Item::test_d_new(ulonglong count)
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
  st.method= "to_double_null";
  st.use_sum_d= true;
  return st;
}


Stat Item::test_ll_new(ulonglong count)
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
  st.method= "to_longlong_null";
  return st;
}


Stat Item::test_int32_new(ulonglong count)
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
  st.method= "to_int32_null";
  return st;
}


/***************************************************/

Stat Item::test_native_prm(ulonglong count)
{
  switch (field_type())
  {
  case MYSQL_TYPE_NULL:
  case MYSQL_TYPE_BOOL:      return test_b_prm(count);
  case MYSQL_TYPE_LONGLONG:  return test_ll_prm(count);
  case MYSQL_TYPE_DOUBLE:    return test_d_prm(count);
  }
  return Stat();
}


Stat Item::test_native_get(ulonglong count)
{
  switch (field_type())
  {
  case MYSQL_TYPE_NULL:
  case MYSQL_TYPE_BOOL:      return test_b_get(count);
  case MYSQL_TYPE_LONGLONG:  return test_ll_get(count);
  case MYSQL_TYPE_DOUBLE:    return test_d_get(count);
  }
  return Stat();
}


Stat Item::test_native_new(ulonglong count)
{
  switch (field_type())
  {
  case MYSQL_TYPE_NULL:
  case MYSQL_TYPE_BOOL:      return test_b_new(count);
  case MYSQL_TYPE_LONGLONG:  return test_ll_new(count);
  case MYSQL_TYPE_DOUBLE:    return test_d_new(count);
  }
  return Stat();
}
