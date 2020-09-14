#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <sys/time.h>
#include "timer.h"

#include "item.h"
#include "item_func.h"

constexpr ulonglong default_count= 10*1000*1000ULL;

volatile int x= 1;
volatile int y= 2;


class Test
{
public:
  Test() { }
  virtual ~Test() { }
  virtual const char *name() const = 0;
  virtual MethodStat run(ulonglong count) const = 0;
  bool matches(const char *mask) const
  {
    size_t mask_length= strlen(mask);
    return !strncmp(mask, name(), mask_length);
  }
};


class Test_n: public Test
{
public:
  const char *name() const override { return "null"; }
  MethodStat run(ulonglong count) const override
  {
    Item_null nl;
    Item_bool b0(false);
    Item_real d0(x);
    Item_int ll0(x);
    MethodStat st;

    st+= nl.test_b(count);                            // NULL
    st+= nl.test_ll(count);                           // NULL
    st+= nl.test_d(count);                            // NULL
    st+= Item_func_isnull(&nl).test_b(count);         // NULL IS NULL
    st+= Item_func_isnull(&b0).test_b(count);         // FALSE IS NULL
    st+= Item_func_isnull(&ll0).test_b(count);        // 0 IS NULL
    st+= Item_func_isnull(&d0).test_b(count);         // 0e0 IS NULL
    st+= Item_func_coalesce(&nl).test_b(count);       // COALESCE(NULL)
    st+= Item_func_coalesce(&nl,&nl).test_b(count);   // COALESCE(NULL,NULL)
    return st;
  }
};


class Test_b: public Test
{
public:
  const char *name() const override { return "bool_misc"; }
  MethodStat run(ulonglong count) const override
  {
    Item_bool b0(false);
    Item_bool b1(true);
    MethodStat st;
    st+= b0.test_b(count);                          // TRUE
    st+= b1.test_b(count);                          // FALSE
    return st;
  }
};


class Test_b_or: public Test
{
public:
  const char *name() const override { return "bool_or"; }
  MethodStat run(ulonglong count) const override
  {
    Item_null nl;
    Item_bool b0(false);
    Item_bool b1(true);
    Item_cond_or or_b0_nl(&b0, &nl);
    MethodStat st;

    st+= Item_cond_or(&b0, &nl).test_b(count);        // FALSE OR NULL
    st+= Item_cond_or(&nl, &b0).test_b(count);        // NULL OR FALSE
    st+= Item_cond_or(&nl, &b1).test_b(count);        // NULL OR TRUE
    st+= Item_cond_or(&nl, &nl).test_b(count);        // NULL OR NULL
    st+= Item_cond_or(&b0, &nl, &b1).test_b(count);   // FALSE OR NULL OR TRUE
    st+= Item_cond_or(&or_b0_nl, &b1).test_b(count);  // (FALSE OR NULL) OR TRUE
    st+= Item_cond_or(&b1, &b1).test_b(count);        // TRUE OR TRUE
    st+= Item_cond_or(&b1, &b0).test_b(count);        // TRUE OR FALSE
    st+= Item_cond_or(&b0, &b0).test_b(count);        // FALSE OR FALSE
    st+= Item_cond_or(&b0, &b1).test_b(count);        // FALSE OR TRUE
    return st;
  }
};


class Test_b_coalesce: public Test
{
public:
  const char *name() const override { return "bool_coalesce"; }
  MethodStat run(ulonglong count) const override
  {
    Item_null nl;
    Item_bool b0(false);
    MethodStat st;
    st+= Item_func_coalesce(&b0).test_b(count);      // COALESCE(FALSE)
    st+= Item_func_coalesce(&nl,&b0).test_b(count);  // COALESCE(NULL,FALSE)
    return st;
  }
};


class Test_ll: public Test
{
public:
  const char *name() const override { return "ll_misc"; }
  MethodStat run(ulonglong count) const override
  {
    Item_int ll0(x);
    Item_func_uminus uminus_ll0(&ll0);
    Item_func_uminus uminus_uminus_ll0(&uminus_ll0);
    MethodStat st;

    st+= ll0.test_ll(count);                     // INT
    st+= uminus_ll0.test_ll(count);              // -INT
    st+= uminus_uminus_ll0.test_ll(count);       // -(-INT)
    return st;
  }
};


class Test_ll_plus: public Test
{
public:
  const char *name() const override { return "ll_plus"; }
  MethodStat run(ulonglong count) const override
  {
    Item_null nl;
    Item_int ll0(x);
    Item_int ll1(y);
    Item_func_add add_ll_ll(&ll0, &ll1);
    Item_func_add add_ll_ll_ll(&add_ll_ll, &ll1);
    Item_func_add add_ll_ll_ll_ll(&add_ll_ll_ll, &ll1);
    MethodStat st;

    st+= Item_func_add(&ll0, &nl).test_ll(count);             // (INT + NULL)
    st+= Item_func_add(&nl, &ll0).test_ll(count);             // (NULL + INT)
    st+= add_ll_ll.test_ll(count);                            // (INT + INT)
    st+= Item_func_add(&add_ll_ll, &ll0).test_ll(count);      // (INT + INT) + INT
    st+= Item_func_add(&add_ll_ll_ll_ll, &ll0).test_ll(count);// (INT + ...+ INT)
    return st;
  }
};


class Test_ll_coalesce: public Test
{
public:
  const char *name() const override { return "ll_coalesce"; }
  MethodStat run(ulonglong count) const override
  {
    Item_null nl;
    Item_int ll0(x);
    Item_int ll1(y);
    Item_func_coalesce coalesce_ll(&ll0);
    Item_func_coalesce coalesce_nl_nl(&nl, &nl);
    Item_func_coalesce coalesce_nl_nl__ll0(&coalesce_nl_nl, &ll0);
    MethodStat st;
    st+= coalesce_ll.test_ll(count);                 // COALESCE(INT)
    st+= coalesce_nl_nl__ll0.test_ll(count); // COALESCE(COALESCE(NULL,NULL), INT)
    st+= Item_func_coalesce(&nl,&ll0).test_ll(count); // COALESCE(NULL,INT)
    return st;
  }
};


class Test_d: public Test
{
public:
  const char *name() const override { return "double_misc"; }
  MethodStat run(ulonglong count) const override
  {
    Item_real d0(x);
    Item_func_uminus uminus_d0(&d0);
    Item_func_uminus uminus_uminus_d0(&uminus_d0);
    MethodStat st;

    st+= d0.test_d(count);               // DBL
    st+= uminus_d0.test_d(count);        // -DBL
    st+= uminus_uminus_d0.test_d(count); // -(-DBL)
    return st;
  }
};


class Test_d_plus: public Test
{
public:
  const char *name() const override { return "double_plus"; }
  MethodStat run(ulonglong count) const override
  {
    Item_null nl;
    Item_real d0(x);
    Item_real d1(y);
    Item_func_add add_d_d(&d0, &d1);
    Item_func_add add_d_d_d(&add_d_d, &d1);
    Item_func_add add_d_d_d_d(&add_d_d_d, &d1);
    MethodStat st;
    st+= Item_func_add(&nl, &d0).test_d(count);          // (NULL+ DBL)
    st+= Item_func_add(&d0, &nl).test_d(count);          // (DBL + NULL)
    st+= add_d_d.test_d(count);                          // (DBL + DBL)
    st+= Item_func_add(&add_d_d, &d0).test_d(count);     // (DBL + DBL) + DBL
    st+= Item_func_add(&add_d_d_d_d, &d0).test_d(count); // (DBL +...+DBL)
    return st;
  }
};


class Test_d_coalesce: public Test
{
public:
  const char *name() const override { return "double_coalesce"; }
  MethodStat run(ulonglong count) const override
  {
    Item_null nl;
    Item_real d0(x);
    Item_func_coalesce coalesce_nl_nl(&nl, &nl);
    Item_func_coalesce coalesce_nl_nl_d0(&nl, &nl, &d0);
    Item_func_coalesce coalesce_nl_nl__d0(&coalesce_nl_nl, &d0);
    MethodStat st;
    st+= Item_func_coalesce(&d0).test_d(count);      // COALESCE(DBL)
    st+= Item_func_coalesce(&nl,&d0).test_ll(count); // COALESCE(NULL,DBL)
    st+= coalesce_nl_nl_d0.test_ll(count);           // COALESCE(NULL,NULL,DBL)
    st+= coalesce_nl_nl__d0.test_ll(count);          // COALESCE((NULL,NULL),DBL)
    return st;
  }
};



void run(const char *name, ulonglong count)
{
  static const Test_n           test_n;
  static const Test_b           test_b;
  static const Test_b_or        test_b_or;
  static const Test_b_coalesce  test_b_coalesce;
  static const Test_ll          test_ll;
  static const Test_ll_plus     test_ll_plus;
  static const Test_ll_coalesce test_ll_coalesce;
  static const Test_d           test_d;
  static const Test_d_plus      test_d_plus;
  static const Test_d_coalesce  test_d_coalesce;

  static const Test *tests[]=
  {
    &test_n,
    &test_b,
    &test_b_or,
    &test_b_coalesce,
    &test_ll,
    &test_ll_plus,
    &test_ll_coalesce,
    &test_d,
    &test_d_plus,
    &test_d_coalesce,
    NULL
  };

  MethodStat st;
  for (uint i= 0; tests[i]; i++)
  {
    if (tests[i]->matches(name))
      st+= tests[i]->run(count);
  }
  printf("Total:\n");
  printf("val_xxx      %f\n", st.val_xxx);
  printf("val_xxx_null %f\n", st.val_xxx_null);
  printf("get_xxx      %f\n", st.get_xxx);
  printf("to_xxx_null  %f\n", st.to_xxx_null);
}


int main(int argc, char **argv)
{
  const char *ecount= getenv("COUNT");
  ulonglong count= ecount ? (ulonglong) atoll(ecount) : default_count;

  if (argc <= 1) // Run all tests
  {
    run("", count);
  }
  else           // Run specified tests
  {
    for (int i= 1; i < argc; i++)
      run(argv[i], count);
  }

  return 0;
}
