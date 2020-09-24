#include "item_func.h"
#include <string>
#include <random>


void Item_null::print(string *to)
{
  to->append("NULL");
}


void Item_bool::print(string *to)
{
  to->append(val ? "TRUE" : "FALSE");
}

void Item_int::print(string *to)
{
  to->append(to_string(val));
}

void Item_real::print(string *to)
{
  to->append(to_string(val));
}

void Item_func_uminus::print(string *to)
{
  to->append("(-(");
  args[0]->print(to);
  to->append("))");
}


void Item_func_add::print(string *to)
{
  to->append("(");
  args[0]->print(to);
  to->append("+");
  args[1]->print(to);
  to->append(")");
}

void Item_cond_or::print(string *to)
{
  to->append("(");
  args[0]->print(to);
  for (uint i= 1; i < arg_count; i++)
  {
    to->append(" OR ");
    args[i]->print(to);
  }
  to->append(")");
}


bool Item_func_add::gen(VM *vm)
{
  Item_int *ia, *ib;
  Item_real *da, *db;

  if (arg_count == 2 &&
      (ia= dynamic_cast<Item_int*>(args[0])) &&
      (ib= dynamic_cast<Item_int*>(args[1])))
  {
    vm->push_back(VM::Instr(VM::MOV_LL_TO_LL0, ia->to_longlong_null()));
    vm->push_back(VM::Instr(VM::ADD_LL0_LLI, ib->to_longlong_null()));
    return false;
  }

  if (arg_count == 2 &&
      (da= dynamic_cast<Item_real*>(args[0])) &&
      (db= dynamic_cast<Item_real*>(args[1])))
  {
    vm->push_back(VM::Instr(VM::MOV_D_TO_D0, da->to_double_null()));
    vm->push_back(VM::Instr(VM::ADD_D0_DI, db->to_double_null()));
    return false;
  }

  return true;
}


#ifdef HAVE_NULL_VALUE
bool Item_func_add::val_bool()
{
  double nr= Item_func_add::val_real();
  if (null_value)
    return false;
  return (bool) nr;
}
#endif


#ifdef HAVE_NULL_VALUE
bool Item_func_uminus::val_bool()
{
  double nr= Item_func_uminus::val_real();
  if (null_value)
    return false;
  return (bool) nr;
}
#endif


#ifdef HAVE_NULL_VALUE
double Item_func_uminus::val_real()
{
  double val0= args[0]->val_real();
  if (args[0]->null_value)
  {
    null_value= true;
    return 0.0;
  }
  null_value= false;
  return -val0;
}
#endif


#ifdef HAVE_NULL_VALUE
longlong Item_func_uminus::val_int()
{
  longlong val0= args[0]->val_int();
  if (args[0]->null_value)
  {
    null_value= true;
    return 0;
  }
  null_value= false;
  return -val0;
}
#endif


#ifdef HAVE_NULL_VALUE
int32 Item_func_uminus::val_int32()
{
  int32 val0= args[0]->val_int32();
  if (args[0]->null_value)
  {
    null_value= true;
    return 0;
  }
  null_value= false;
  return -val0;
}
#endif


#ifdef HAVE_NULL_VALUE
double Item_func_add::val_real()
{
  double val0= args[0]->val_real();
  if (args[0]->null_value)
  {
    null_value= true;
    return 0.0;
  }
  double val1= args[1]->val_real();
  if (args[1]->null_value)
  {
    null_value= true;
    return 0.0;
  }
  null_value= false;
  return val0 + val1;
}
#endif


#ifdef HAVE_NULL_VALUE
longlong Item_func_add::val_int()
{
  longlong val0= args[0]->val_int();
  if (args[0]->null_value)
  {
    null_value= true;
    return 0;
  }
  longlong val1= args[1]->val_int();
  if (args[1]->null_value)
  {
    null_value= true;
    return 0;
  }
  null_value= false;
  return val0 + val1;
}
#endif


#ifdef HAVE_NULL_VALUE
int32 Item_func_add::val_int32()
{
  int32 val0= args[0]->val_int32();
  if (args[0]->null_value)
  {
    null_value= true;
    return 0;
  }
  int32 val1= args[1]->val_int32();
  if (args[1]->null_value)
  {
    null_value= true;
    return 0;
  }
  null_value= false;
  return val0 + val1;
}
#endif


#ifdef HAVE_NULL_VALUE
bool Item_cond_or::val_bool()
{
  null_value= false;
  for (uint i= 0 ; i < arg_count; i++)
  {
    if (args[i]->val_bool())
    { 
      null_value= false;
      return true;
    }
    if (args[i]->null_value)
      null_value= true;
  }
  return false;
}
#endif


bool Item_cond_or::get_bool(bool *to)
{
  bool tmp_null_value= false;
  for (uint i= 0 ; i < arg_count; i++)
  {
    if (!args[i]->get_bool(to))
    {
      if (*to)
        return false; 
    }
    else
      tmp_null_value= true;
  }
  *to= false;
  return tmp_null_value;
}


bool Item_cond_or::get_longlong(longlong *to)
{
  bool tmp;
  if (Item_cond_or::get_bool(&tmp))
    return true;
  *to= tmp;
  return false;
}


bool Item_cond_or::get_int32(int32 *to)
{
  bool tmp;
  if (Item_cond_or::get_bool(&tmp))
    return true;
  *to= tmp;
  return false;
}


bool Item_cond_or::get_double(double *to)
{
  bool tmp;
  if (Item_cond_or::get_bool(&tmp))
    return true;
  *to= tmp;
  return false;
}


bool Item_func_uminus::gen(VM *vm)
{
  if (dynamic_cast<Item_int*>(args[0]))
  {
    vm->push_back(VM::Instr(VM::MOV_LL_TO_LL0, args[0]->to_longlong_null()));
    vm->push_back(VM::Instr(VM::NEG_LL0));
    return false;
  }

  if (dynamic_cast<Item_real*>(args[0]))
  {
    vm->push_back(VM::Instr(VM::MOV_D_TO_D0, args[0]->to_double_null()));
    vm->push_back(VM::Instr(VM::NEG_D0));
    return false;
  }

  return true;
}



bool Item_func_uminus::val_bool_null(bool *null_value_arg)
{
  return (bool) Item_func_uminus::val_real_null(null_value_arg);
}


longlong Item_func_uminus::val_int_null(bool *null_value_arg)
{
  longlong val0= args[0]->val_int_null(null_value_arg);
  return -val0;
}


int32 Item_func_uminus::val_int32_null(bool *null_value_arg)
{
  int32 val0= args[0]->val_int32_null(null_value_arg);
  return -val0;
}


bool Item_func_uminus::get_bool(bool *to)
{
  double nr;
  bool is_null= args[0]->get_double(&nr);
  *to= (bool) nr;
  return is_null;
}


bool Item_func_uminus::get_longlong(longlong *to)
{
  bool is_null= args[0]->get_longlong(to);
  *to= -(*to);
  return is_null;
}


bool Item_func_uminus::get_int32(int32 *to)
{
  bool is_null= args[0]->get_int32(to);
  *to= -(*to);
  return is_null;
}


bool Item_func_uminus::get_double(double *to)
{
  bool is_null= args[0]->get_double(to);
  *to= -(*to);
  return is_null;
}


Bool_null Item_func_uminus::to_bool_null()
{
  Double_null nr= Item_func_uminus::to_double_null();
  return Bool_null((bool) nr.value, nr.is_null);
}

double Item_func_uminus::val_real_null(bool *null_value_arg)
{
  double val0= args[0]->val_real_null(null_value_arg);
  return -val0;
}


Longlong_null Item_func_uminus::to_longlong_null()
{
  return args[0]->to_longlong_null().neg();
}


Int32_null Item_func_uminus::to_int32_null()
{
  return args[0]->to_int32_null().neg();
}


Double_null Item_func_uminus::to_double_null()
{
  return args[0]->to_double_null().neg();
}


bool Item_cond_or::val_bool_null(bool *null_value_arg)
{
  // QQ: try to call val_bool_null(null_value_arg)!
  for (uint i= 0 ; i < arg_count; i++)
  {
    bool null_cur;
    if (args[i]->val_bool_null(&null_cur) && !null_cur)
    {
      *null_value_arg= false;
      return true;
    }
    *null_value_arg|= null_cur;
  }
  return false;
}


longlong Item_cond_or::val_int_null(bool *null_value_arg)
{
  return Item_cond_or::val_bool_null(null_value_arg);
}


int32 Item_cond_or::val_int32_null(bool *null_value_arg)
{
  return Item_cond_or::val_bool_null(null_value_arg);
}


double Item_cond_or::val_real_null(bool *null_value_arg)
{
  return Item_cond_or::val_bool_null(null_value_arg);
}


Bool_null Item_cond_or::to_bool_null()
{
  Bool_null res= args[0]->to_bool_null();
  if (res.is_true())
    return res;
  for (uint i= 1; i < arg_count; i++)
  {
    Bool_null tmp= args[i]->to_bool_null();
    if (tmp.is_true())
      return tmp;
    res|= tmp; // This ORs the NULL flag
  }
  return res;
}


Longlong_null Item_cond_or::to_longlong_null()
{
  Bool_null tmp= Item_cond_or::to_bool_null();
  return Longlong_null(tmp.value, tmp.is_null);
}


Int32_null Item_cond_or::to_int32_null()
{
  Bool_null tmp= Item_cond_or::to_bool_null();
  return Int32_null(tmp.value, tmp.is_null);
}


Double_null Item_cond_or::to_double_null()
{
  Bool_null tmp= Item_cond_or::to_bool_null();
  return Double_null(tmp.value, tmp.is_null);
}


bool Item_func_add::val_bool_null(bool *null_value_arg)
{
  return (bool) val_real_null(null_value_arg);
}


bool Item_func_add::get_bool(bool *to)
{
  double nr;
  bool is_null= Item_func_add::get_double(&nr);
  *to= nr;
  return is_null;
}


longlong Item_func_add::val_int_null(bool *null_value_arg)
{
  longlong val0= args[0]->val_int_null(null_value_arg);
  if (*null_value_arg)
    return 0;
  longlong val1= args[1]->val_int_null(null_value_arg);
  return val0 + val1;
}


int32 Item_func_add::val_int32_null(bool *null_value_arg)
{
  int32 val0= args[0]->val_int32_null(null_value_arg);
  if (*null_value_arg)
    return 0;
  int32 val1= args[1]->val_int32_null(null_value_arg);
  return val0 + val1;
}


bool Item_func_add::get_longlong(longlong *to)
{
  longlong val1;
  if (args[0]->get_longlong(to) || args[1]->get_longlong(&val1))
    return true;
  *to+= val1;
  return false;
}


bool Item_func_add::get_int32(int32 *to)
{
  int32 val1;
  bool is_null= args[0]->get_int32(to);
  if (is_null)
    return true;
  is_null= args[1]->get_int32(&val1);
  *to+= val1;
  return is_null;
}


bool Item_func_add::get_double(double *to)
{
  double val1;
  bool is_null= args[0]->get_double(to);
  if (is_null)
    return true;
  is_null= args[1]->get_double(&val1);
  *to+= val1;
  return is_null;
}


double Item_func_add::val_real_null(bool *null_value_arg)
{
  double val0= args[0]->val_real_null(null_value_arg);
  if (*null_value_arg)
    return 0.0;
  double val1= args[1]->val_real_null(null_value_arg);
  return val0 + val1;
}


Bool_null Item_func_add::to_bool_null()
{
  Double_null nr= Item_func_add::to_double_null();
  return Bool_null((bool) nr.value, nr.is_null);
}

Longlong_null Item_func_add::to_longlong_null()
{
#if 0
  // Version with more constructors
  const Longlong_null nr0(args[0]->to_longlong_null());
  if (nr0.is_null)
    return Longlong_null();
  const Longlong_null nr1(args[1]->to_longlong_null());
  if (nr1.is_null)
    return Longlong_null();
  return Longlong_null(nr0.value + nr1.value);
#endif

  // Version with one nr
#if 0
  Longlong_null nr(args[0]->to_longlong_null());
  if (nr.is_null)
    return nr;
  longlong tmp= nr.value;
  nr= args[1]->to_longlong_null();
  if (!nr.is_null)
    nr.value+= tmp;
  return nr;
#endif

  // Version with two nr
#if 0
  Longlong_null nr0= args[0]->to_longlong_null();
  if (nr0.is_null)
    return nr0;
  const Longlong_null nr1= args[1]->to_longlong_null();
  if (nr1.is_null)
    return nr1;
  nr0.value+= nr1.value;
  return nr0;
#endif

  // Version with the operator
#if 1
  const Longlong_null nr0= args[0]->to_longlong_null();
  if (nr0.is_null)
    return nr0;
  const Longlong_null nr1= args[1]->to_longlong_null();
  return Longlong_null(nr0.value + nr1.value, nr1.is_null);
#endif
}


Int32_null Item_func_add::to_int32_null()
{
  const Int32_null nr0= args[0]->to_int32_null();
  if (nr0.is_null)
    return nr0;
  const Int32_null nr1= args[1]->to_int32_null();
  return Int32_null(nr0.value + nr1.value, nr1.is_null);

}


Double_null Item_func_add::to_double_null()
{
  // Version with one nr
#if 0
  Double_null nr(args[0]->to_double_null());
  if (nr.is_null)
    return nr;
  double tmp= nr.value;
  nr= args[1]->to_double_null();
  if (!nr.is_null)
    nr.value+= tmp;
  return nr;
#endif

  // Version with two nr
#if 0
  Double_null nr0= args[0]->to_double_null();
  if (nr0.is_null)
    return nr0;
  const Double_null nr1= args[1]->to_double_null();
  if (nr1.is_null)
    return nr1;
  nr0.value+= nr1.value;
  return nr0;
#endif

#if 1
  const Double_null nr0= args[0]->to_double_null();
  if (nr0.is_null)
    return nr0;
  const Double_null nr1= args[1]->to_double_null();
  return Double_null(nr0.value + nr1.value, nr1.is_null);
#endif
}


void Item_func_isnull::print(string *to)
{
  to->append("((");
  args[0]->print(to);
  to->append(") IS NULL)");
}


#ifdef HAVE_NULL_VALUE
bool Item_func_isnull::val_bool()
{
  null_value= false;
  switch (m_arg0_field_type) {
  case MYSQL_TYPE_NULL:      return true;
  case MYSQL_TYPE_BOOL:      args[0]->val_bool(); break;
  case MYSQL_TYPE_LONGLONG:  args[0]->val_int();  break;
  case MYSQL_TYPE_DOUBLE:    args[0]->val_real(); break;
  }
  return args[0]->null_value;
}
#endif


bool Item_func_isnull::val_bool_null(bool *null_value_arg)
{
  *null_value_arg= false;
  switch (m_arg0_field_type) {
  case MYSQL_TYPE_NULL:      return true;
  case MYSQL_TYPE_BOOL:      args[0]->val_bool_null(null_value_arg); break;
  case MYSQL_TYPE_LONGLONG:  args[0]->val_int_null(null_value_arg);  break;
  case MYSQL_TYPE_DOUBLE:    args[0]->val_real_null(null_value_arg); break;
  }
  return *null_value_arg;
}


bool Item_func_isnull::get_bool(bool *to)
{
  switch (m_arg0_field_type) {
  case MYSQL_TYPE_NULL:
    *to= true;
    return false;
  case MYSQL_TYPE_BOOL:
  {
    bool tmp;
    *to= args[0]->get_bool(&tmp);
    return false;
  }
  case MYSQL_TYPE_LONGLONG:
  {
    longlong tmp;
    *to= args[0]->get_longlong(&tmp);
    return false;
  }
  case MYSQL_TYPE_DOUBLE:
  {
    double tmp;
    *to= args[0]->get_double(&tmp);
    return false;
  }
  }
  return true;
}


Bool_null Item_func_isnull::to_bool_null()
{
  switch (m_arg0_field_type) {
  case MYSQL_TYPE_NULL:     return Bool_null(true);
  case MYSQL_TYPE_BOOL:     return Bool_null(args[0]->to_bool_null().is_null);
  case MYSQL_TYPE_LONGLONG: return Bool_null(args[0]->to_longlong_null().is_null);
  case MYSQL_TYPE_DOUBLE:   return Bool_null(args[0]->to_double_null().is_null);
  }
  //DBUG_ASSERT(0)
  return Bool_null(true);
}


/******************************************************************/

void Item_func_coalesce::print(string *to)
{
  to->append("COALESCE(");
  args[0]->print(to);
  for (uint i= 1; i < arg_count; i++)
  {
    to->append(",");
    args[i]->print(to);
  }
  to->append(")");
}


#ifdef HAVE_NULL_VALUE
bool Item_func_coalesce::val_bool()
{
  null_value= false;
  for (uint i=0 ; i < arg_count ; i++)
  {
    bool res=args[i]->val_bool();
    if (!args[i]->null_value)
      return res;
  }
  null_value= true;
  return 0;
}

double Item_func_coalesce::val_real()
{
  null_value= false;
  for (uint i=0 ; i < arg_count ; i++)
  {
    double res= args[i]->val_real();
    if (!args[i]->null_value)
      return res;
  }
  null_value= true;
  return 0;
}

longlong Item_func_coalesce::val_int()
{
  null_value= false;
  for (uint i=0 ; i < arg_count ; i++)
  {
    longlong res= args[i]->val_int();
    if (!args[i]->null_value)
      return res;
  }
  null_value= true;
  return 0;
}

int32 Item_func_coalesce::val_int32()
{
  null_value= false;
  for (uint i=0 ; i < arg_count ; i++)
  {
    int32 res= args[i]->val_int32();
    if (!args[i]->null_value)
      return res;
  }
  null_value= true;
  return 0;
}
#endif


bool Item_func_coalesce::val_bool_null(bool *null_value_arg)
{
  uint last= arg_count - 1;
  for (uint i=0 ; i < last ; i++)
  {
    bool res= args[i]->val_bool_null(null_value_arg);
    if (!*null_value_arg)
      return res;
  }
  return args[last]->val_bool_null(null_value_arg);
}


longlong Item_func_coalesce::val_int_null(bool *null_value_arg)
{
  uint last= arg_count - 1;
  for (uint i=0 ; i < last ; i++)
  {
    longlong res= args[i]->val_int_null(null_value_arg);
    if (!*null_value_arg)
      return res;
  }
  return args[last]->val_int_null(null_value_arg);
}


int32 Item_func_coalesce::val_int32_null(bool *null_value_arg)
{
 uint last= arg_count - 1;
   for (uint i=0 ; i < last ; i++)
  {
    int32 res= args[i]->val_int32_null(null_value_arg);
    if (!*null_value_arg)
      return res;
  }
  return args[last]->val_int32_null(null_value_arg);
}


double Item_func_coalesce::val_real_null(bool *null_value_arg)
{
  uint last= arg_count - 1;
  for (uint i=0 ; i < last ; i++)
  {
    double res= args[i]->val_real_null(null_value_arg);
    if (!*null_value_arg)
      return res;
  }
  return args[last]->val_real_null(null_value_arg);
}

bool Item_func_coalesce::get_bool(bool *to)
{
  uint last= arg_count - 1;
  for (uint i= 0; i < last; i++)
  {
    if (!args[i]->get_bool(to))
      return false;
  }
  return args[last]->get_bool(to);
}


bool Item_func_coalesce::get_longlong(longlong *to)
{
  uint last= arg_count - 1;
  for (uint i= 0; i < last; i++)
  {
    if (!args[i]->get_longlong(to))
      return false;
  }
  return args[last]->get_longlong(to);
}


bool Item_func_coalesce::get_int32(int32 *to)
{
  uint last= arg_count - 1;
  for (uint i= 0; i < last; i++)
  {
    if (!args[i]->get_int32(to))
      return false;
  }
  return args[last]->get_int32(to);
}


bool Item_func_coalesce::get_double(double *to)
{
  uint last= arg_count - 1;
  for (uint i= 0; i < last; i++)
  {
    if (!args[i]->get_double(to))
      return false;
  }
  return args[last]->get_double(to);
}


Bool_null Item_func_coalesce::to_bool_null()
{
  uint last= arg_count - 1;
  for (uint i= 0; i < last; i++)
  {
    Bool_null tmp= args[i]->to_bool_null();
    if (!tmp.is_null)
      return tmp;
  }
  return args[last]->to_bool_null();
}

Longlong_null Item_func_coalesce::to_longlong_null()
{
  uint last= arg_count - 1;
  for (uint i= 0; i < last; i++)
  {
    Longlong_null tmp= args[i]->to_longlong_null();
    if (!tmp.is_null)
      return tmp;
  }
  return args[last]->to_longlong_null();
}

Int32_null Item_func_coalesce::to_int32_null()
{
  uint last= arg_count - 1;
  for (uint i= 0; i < last; i++)
  {
    Int32_null tmp= args[i]->to_int32_null();
    if (!tmp.is_null)
      return tmp;
  }
  return args[last]->to_int32_null();
}

Double_null Item_func_coalesce::to_double_null()
{
  uint last= arg_count - 1;
  for (uint i= 0; i < last; i++)
  {
    Double_null tmp= args[i]->to_double_null();
    if (!tmp.is_null)
      return tmp;
  }
  return args[last]->to_double_null();
}

/****************************************************************/

void Item_func_last_value::print(string *to)
{
  to->append("LAST_VALUE(");
  args[0]->print(to);
  for (uint i= 1; i < arg_count; i++)
  {
    to->append(",");
    args[i]->print(to);
  }
  to->append(")");
}


#ifdef HAVE_NULL_VALUE
void Item_func_last_value::evaluate_sideeffects_val_int()
{
  for (uint i= 0; i < arg_count-1 ; i++)
    args[i]->val_int();
}

void Item_func_last_value::evaluate_sideeffects_val_int32()
{
  for (uint i= 0; i < arg_count-1 ; i++)
    args[i]->val_int32();
}

bool Item_func_last_value::val_bool()
{
  bool tmp;
  evaluate_sideeffects_val_int();
  tmp= last_value->val_bool();
  null_value= last_value->null_value;
  return tmp;
}

longlong Item_func_last_value::val_int()
{
  longlong tmp;
  evaluate_sideeffects_val_int();
  tmp= last_value->val_int();
  null_value= last_value->null_value;
  return tmp;
}

int32 Item_func_last_value::val_int32()
{
  longlong tmp;
  evaluate_sideeffects_val_int32();
  tmp= last_value->val_int32();
  null_value= last_value->null_value;
  return tmp;
}

double Item_func_last_value::val_real()
{
  double tmp;
  evaluate_sideeffects_val_int();
  tmp= last_value->val_real();
  null_value= last_value->null_value;
  return tmp;
}
#endif


void Item_func_last_value::evaluate_sideeffects_val_int_null()
{
  bool nl;
  for (uint i= 0; i < arg_count-1 ; i++)
    args[i]->val_int_null(&nl);
}

void Item_func_last_value::evaluate_sideeffects_val_int32_null()
{
  bool nl;
  for (uint i= 0; i < arg_count-1 ; i++)
    args[i]->val_int32_null(&nl);
}

bool Item_func_last_value::val_bool_null(bool *nl)
{
  evaluate_sideeffects_val_int_null();
  return last_value->val_bool_null(nl);
}

longlong Item_func_last_value::val_int_null(bool *nl)
{
  evaluate_sideeffects_val_int_null();
  return last_value->val_int_null(nl);
}

int32 Item_func_last_value::val_int32_null(bool *nl)
{
  evaluate_sideeffects_val_int32_null();
  return last_value->val_int32_null(nl);
}

double Item_func_last_value::val_real_null(bool *nl)
{
  evaluate_sideeffects_val_int_null();
  return last_value->val_real_null(nl);
}


void Item_func_last_value::evaluate_sideeffects_get_longlong()
{
  longlong nr;
  for (uint i= 0; i < arg_count-1 ; i++)
    args[i]->get_longlong(&nr);
}

bool Item_func_last_value::get_bool(bool *to)
{
  evaluate_sideeffects_get_longlong();
  return last_value->get_bool(to);
}

bool Item_func_last_value::get_longlong(longlong *to)
{
  evaluate_sideeffects_get_longlong();
  return last_value->get_longlong(to);
}

bool Item_func_last_value::get_double(double *to)
{
  evaluate_sideeffects_get_longlong();
  return last_value->get_double(to);
}


void Item_func_last_value::evaluate_sideeffects_get_int32()
{
  int32 nr;
  for (uint i= 0; i < arg_count-1 ; i++)
    args[i]->get_int32(&nr);
}


bool Item_func_last_value::get_int32(int32 *to)
{
  evaluate_sideeffects_get_int32();
  return last_value->get_int32(to);
}



void Item_func_last_value::evaluate_sideeffects_to_longlong_null()
{
  for (uint i= 0; i < arg_count-1 ; i++)
    args[i]->to_longlong_null();
}

void Item_func_last_value::evaluate_sideeffects_to_int32_null()
{
  for (uint i= 0; i < arg_count-1 ; i++)
    args[i]->to_int32_null();
}


Bool_null Item_func_last_value::to_bool_null()
{
  evaluate_sideeffects_to_longlong_null();
  return last_value->to_bool_null();
}

Longlong_null Item_func_last_value::to_longlong_null()
{
  evaluate_sideeffects_to_longlong_null();
  return last_value->to_longlong_null();
}

Int32_null Item_func_last_value::to_int32_null()
{
  evaluate_sideeffects_to_int32_null();
  return last_value->to_int32_null();
}

Double_null Item_func_last_value::to_double_null()
{
  evaluate_sideeffects_to_longlong_null();
  return last_value->to_double_null();
}

/******************************************************************/


void Item_func_eq::print(string *to)
{
  to->append("(");
  args[0]->print(to);
  to->append(" = ");
  args[1]->print(to);
  to->append(")");
}


bool Item_func_eq::gen(VM *vm)
{
  Item_int *ia, *ib;
  Item_real *da, *db;

  if (arg_count == 2 &&
      (ia= dynamic_cast<Item_int*>(args[0])) &&
      (ib= dynamic_cast<Item_int*>(args[1])))
  {
    vm->push_back(VM::Instr(VM::MOV_LL_TO_LL0, ia->to_longlong_null()));
    vm->push_back(VM::Instr(VM::EQ_LL0_LLI, ib->to_longlong_null()));
    return false;
  }

  if (arg_count == 2 &&
      (da= dynamic_cast<Item_real*>(args[0])) &&
      (db= dynamic_cast<Item_real*>(args[1])))
  {
    vm->push_back(VM::Instr(VM::MOV_D_TO_D0, da->to_double_null()));
    vm->push_back(VM::Instr(VM::EQ_D0_DI, db->to_double_null()));
    return false;
  }

  return true;
}


#ifdef HAVE_NULL_VALUE
bool Item_func_eq::val_bool()
{
  switch (cmp_type.m_field_type) {
  case MYSQL_TYPE_NULL:
  case MYSQL_TYPE_BOOL:
    return cmp.eq_bool_using_val_bool();
  case MYSQL_TYPE_LONGLONG:
    return cmp.eq_ll_using_val_int();
  case MYSQL_TYPE_DOUBLE:
    return cmp.eq_double_using_val_real();
  }
  return 0; // Make compliler happy
}
#endif


#ifdef HAVE_NULL_VALUE
double Item_func_eq::val_real()
{
  return Item_func_eq::val_bool();
}
#endif


#ifdef HAVE_NULL_VALUE
longlong Item_func_eq::val_int()
{
  return Item_func_eq::val_bool();
}
#endif


#ifdef HAVE_NULL_VALUE
int32 Item_func_eq::val_int32()
{
  return Item_func_eq::val_bool();
}
#endif


bool Item_func_eq::val_bool_null(bool *null_value_arg)
{
  return cmp.eq_ll_using_val_int_null(null_value_arg);
  switch (cmp_type.m_field_type) {
  case MYSQL_TYPE_NULL:
  case MYSQL_TYPE_BOOL:
    return cmp.eq_bool_using_val_bool_null(null_value_arg);
  case MYSQL_TYPE_LONGLONG:
    return cmp.eq_ll_using_val_int_null(null_value_arg);
  case MYSQL_TYPE_DOUBLE:
    return cmp.eq_double_using_val_real_null(null_value_arg);
  }
  // Make compiler happy
  *null_value_arg= false;
  return false;
}


longlong Item_func_eq::val_int_null(bool *null_value_arg)
{
  return Item_func_eq::val_bool_null(null_value_arg);
}


int32 Item_func_eq::val_int32_null(bool *null_value_arg)
{
  return Item_func_eq::val_bool_null(null_value_arg);
}


double Item_func_eq::val_real_null(bool *null_value_arg)
{
  return Item_func_eq::val_bool_null(null_value_arg);
}


bool Item_func_eq::get_bool(bool *to)
{
  return cmp.eq_ll_using_get_longlong(to);

  switch (cmp_type.m_field_type) {
  case MYSQL_TYPE_NULL:
  case MYSQL_TYPE_BOOL:
    return cmp.eq_bool_using_get_bool(to);
  case MYSQL_TYPE_LONGLONG:
    return cmp.eq_ll_using_get_longlong(to);
  case MYSQL_TYPE_DOUBLE:
    return cmp.eq_double_using_get_double(to);
  }
  *to= false;
  return false; // Make compiler happy
}


bool Item_func_eq::get_longlong(longlong *to)
{
  bool nr;
  bool isnull= Item_func_eq::get_bool(&nr);
  *to= nr;
  return isnull;
}


bool Item_func_eq::get_int32(int32 *to)
{
  bool nr;
  bool isnull= Item_func_eq::get_bool(&nr);
  *to= nr;
  return isnull;
}


bool Item_func_eq::get_double(double *to)
{
  bool nr;
  bool isnull= Item_func_eq::get_bool(&nr);
  *to= nr;
  return isnull;
}


Bool_null Item_func_eq::to_bool_null()
{
  return cmp.eq_ll_using_to_longlong_null();

  switch (cmp_type.m_field_type) {
  case MYSQL_TYPE_NULL:
  case MYSQL_TYPE_BOOL:
    return cmp.eq_bool_using_to_bool_null();
  case MYSQL_TYPE_LONGLONG:
    return cmp.eq_ll_using_to_longlong_null();
  case MYSQL_TYPE_DOUBLE:
    return cmp.eq_double_using_to_double_null();
  }
  return Bool_null(false);
}


Longlong_null Item_func_eq::to_longlong_null()
{
  const Bool_null nr= Item_func_eq::to_bool_null();
  return Longlong_null(nr.value, nr.is_null);
}


Int32_null Item_func_eq::to_int32_null()
{
  const Bool_null nr= Item_func_eq::to_bool_null();
  return Int32_null(nr.value, nr.is_null);
}


Double_null Item_func_eq::to_double_null()
{
  const Bool_null nr0= Item_func_eq::to_bool_null();
  return Double_null(nr0.value, nr0.is_null);
}

static inline size_t get_random_int() {
  static std::random_device rd;
  static std::default_random_engine dre(rd());
  static std::uniform_int_distribution<size_t> ud(1, 20000);
  return ud(dre);
}

Item *node_factory(size_t depth_left,
                   std::vector<std::unique_ptr<Item>> &storage)
{
  switch (get_random_int() % 16)
  {
  case 0:
    return new Item_int(0x27);
  case 1:
    return new Item_null;
  case 2:
    return new Item_bool(true);
  case 3:
    return new Item_real(123.432);
  case 4:
    return new Item_func_add(generate_node(depth_left - 1, storage),
                             generate_node(depth_left - 1, storage));
  case 5:
    return new Item_cond_or(generate_node(depth_left - 1, storage),
                            generate_node(depth_left - 1, storage));
  case 6:
    return new Item_cond_or(generate_node(depth_left - 1, storage),
                            generate_node(depth_left - 1, storage),
                            generate_node(depth_left - 1, storage));
  case 7:
    return new Item_func_uminus(generate_node(depth_left - 1, storage));
  case 8:
    return new Item_func_isnull(generate_node(depth_left - 1, storage));
  case 9:
    return new Item_func_coalesce(generate_node(depth_left - 1, storage));
  case 10:
    return new Item_func_coalesce(generate_node(depth_left - 1, storage),
                                  generate_node(depth_left - 1, storage));
  case 11:
    return new Item_func_coalesce(generate_node(depth_left - 1, storage),
                                  generate_node(depth_left - 1, storage),
                                  generate_node(depth_left - 1, storage));
  case 12:
    return new Item_func_eq(generate_node(depth_left - 1, storage),
                            generate_node(depth_left - 1, storage));
  case 13:
    return new Item_func_last_value(generate_node(depth_left - 1, storage));
  case 14:
    return new Item_func_last_value(generate_node(depth_left - 1, storage),
                                    generate_node(depth_left - 1, storage));
  case 15:
    return new Item_func_last_value(generate_node(depth_left - 1, storage),
                                    generate_node(depth_left - 1, storage),
                                    generate_node(depth_left - 1, storage));
  default:
    assert(0);
    return nullptr;
  }

  __builtin_unreachable();
}

Item *generate_node(size_t depth_left,
                    std::vector<std::unique_ptr<Item>> &storage)
{
  assert(depth_left > 0);

  if (depth_left == 1)
  {
    storage.emplace_back(new Item_int(0x27));
    return storage.back().get();
  }

  Item *node = node_factory(depth_left, storage);
  storage.emplace_back(node);

  return node;
}

namespace variant
{

Bool_null Item_func_add::to_bool_null()
{
  Double_null nr= Item_func_add::to_double_null();
  return Bool_null((bool)nr.value, nr.is_null);
}

Longlong_null Item_func_add::to_longlong_null()
{
  const Longlong_null nr0= variant::to_longlong_null(*args[0]);
  if (nr0.is_null)
    return nr0;
  const Longlong_null nr1= variant::to_longlong_null(*args[1]);
  return Longlong_null(nr0.value + nr1.value, nr1.is_null);
}

Int32_null Item_func_add::to_int32_null()
{
  const Int32_null nr0= variant::to_int32_null(*args[0]);
  if (nr0.is_null)
    return nr0;
  const Int32_null nr1= variant::to_int32_null(*args[1]);
  return Int32_null(nr0.value + nr1.value, nr1.is_null);
}

Double_null Item_func_add::to_double_null()
{
  const Double_null nr0= variant::to_double_null(*args[0]);
  if (nr0.is_null)
    return nr0;
  const Double_null nr1= variant::to_double_null(*args[1]);
  return Double_null(nr0.value + nr1.value, nr1.is_null);
}

Bool_null Item_func_uminus::to_bool_null()
{
  Double_null nr= Item_func_uminus::to_double_null();
  return Bool_null((bool)nr.value, nr.is_null);
}

Longlong_null Item_func_uminus::to_longlong_null()
{
  return variant::to_longlong_null(*args[0]);
}

Int32_null Item_func_uminus::to_int32_null()
{
  return variant::to_int32_null(*args[0]);
}

Double_null Item_func_uminus::to_double_null()
{
  return variant::to_double_null(*args[0]);
}

Bool_null Item_func_isnull::to_bool_null()
{
  switch (m_arg0_field_type)
  {
  case MYSQL_TYPE_NULL:
    return Bool_null(true);
  case MYSQL_TYPE_BOOL:
    return Bool_null(variant::to_bool_null(*args[0]).is_null);
  case MYSQL_TYPE_LONGLONG:
    return Bool_null(variant::to_longlong_null(*args[0]).is_null);
  case MYSQL_TYPE_DOUBLE:
    return Bool_null(variant::to_double_null(*args[0]).is_null);
  }
  assert(0);
  return Bool_null(true);
}

} // namespace variant
