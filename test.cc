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


class Test_null_misc: public Test
{
public:
  const char *name() const override { return "null_misc"; }
  MethodStat run(ulonglong count) const override
  {
    Item_null nl;                               // NULL
    Item_bool b0(false);                        // FALSE
    Item_real d0(x);                            // 0e0
    Item_int ll0(x);                            // 0
    Item_func_coalesce coalesce_nl(&nl);        // COALESCE(NULL)
    Item_func_coalesce coalesce_nl_nl(&nl,&nl); // COALESCE(NULL,NULL)

    MethodStat st;
    st+= nl.test_b(count);
    st+= nl.test_ll(count);
    st+= nl.test_d(count);
    st+= coalesce_nl.test_b(count);
    st+= coalesce_nl_nl.test_b(count);
    return st;
  }
};


class Test_bool_isnull: public Test
{
public:
  const char *name() const override { return "bool_isnull"; }
  MethodStat run(ulonglong count) const override
  {
    Item_null nl;                               // NULL
    Item_bool b0(false);                        // FALSE
    Item_real d0(x);                            // 0e0
    Item_int ll0(x);                            // 0
    Item_func_isnull isnull_nl(&nl);            // NULL IS NULL
    Item_func_isnull isnull_b0(&b0);            // FALSE IS NULL
    Item_func_isnull isnull_ll0(&ll0);          // 0 IS NULL
    Item_func_isnull isnull_d0(&d0);            // 0e0 IS NULL

    Item *items_b[]=
    {
      &isnull_nl,
      &isnull_b0,
      &isnull_ll0,
      &isnull_d0,
      NULL
    };
    MethodStat st;
    for (uint i= 0; items_b[i]; i++)
      st+= items_b[i]->test_b(count);
    return st;
  }
};


class Test_b: public Test
{
public:
  const char *name() const override { return "bool_misc"; }
  MethodStat run(ulonglong count) const override
  {
    Item_bool b0(false);                // FALSE
    Item_bool b1(true);                 // TRUE
    Item *items[]= {&b0, &b1, NULL};
    MethodStat st;
    for (uint i= 0 ; items[i]; i++)
      st+= items[i]->test_b(count);
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

    Item_cond_or or_nl_nl(&nl, &nl);        // NULL OR NULL
    Item_cond_or or_nl_b0(&nl, &b0);        // NULL OR FALSE
    Item_cond_or or_nl_b1(&nl, &b1);        // NULL OR TRUE
    Item_cond_or or_b0_nl(&b0, &nl);        // FALSE OR NULL
    Item_cond_or or_b0_b0(&b0, &b0);        // FALSE OR FALSE
    Item_cond_or or_b0_b1(&b0, &b1);        // FALSE OR TRUE
    Item_cond_or or_b1_b0(&b1, &b0);        // TRUE OR FALSE
    Item_cond_or or_b1_b1(&b1, &b1);        // TRUE OR TRUE

    Item_cond_or or_b0_nl_b1(&b0, &nl, &b1);   // FALSE OR NULL OR TRUE
    Item_cond_or or_b0_nl__b1(&or_b0_nl, &b1);  // (FALSE OR NULL) OR TRUE

    Item *items[]=
    {
      &or_nl_nl,
      &or_nl_b0,
      &or_nl_b1,
      &or_b0_nl,
      &or_b0_b0,
      &or_b0_b1,
      &or_b1_b0,
      &or_b1_b1,
      &or_b0_nl_b1,
      &or_b0_nl__b1,
      NULL
    };
    MethodStat st;
    for (uint i= 0; items[i]; i++)
      st+= items[i]->test_b(count);
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
    Item_func_coalesce coalesce_b0(&b0);         // COALESCE(FALSE)
    Item_func_coalesce coalesce_nl_b0(&nl, &b0); // COALESCE(NULL,FALSE)
 
    Item *items[]= {&coalesce_b0, &coalesce_nl_b0, NULL};
    MethodStat st;
    for (uint i= 0; items[i]; i++)
      st+= items[i]->test_b(count);
    return st;
  }
};


class Test_ll: public Test
{
public:
  const char *name() const override { return "ll_misc"; }
  MethodStat run(ulonglong count) const override
  {
    Item_int ll0(x);                                    // INT
    Item_func_uminus uminus_ll0(&ll0);                  // -INT
    Item_func_uminus uminus_uminus_ll0(&uminus_ll0);    // -(-INT)
 
    Item *items[]= {&ll0, &uminus_ll0, &uminus_uminus_ll0, NULL};
    MethodStat st;
    for (uint i= 0; items[i]; i++)
      st+= items[i]->test_ll(count);
    return st;
  }
};


class Test_ll_plus: public Test
{
public:
  const char *name() const override { return "ll_plus"; }
  MethodStat run(ulonglong count) const override
  {
    Item_null nl;                                         // NULL
    Item_int ll0(x);                                      // INT
    Item_int ll1(y);                                      // INT

    Item_func_add add_ll0_nl(&ll0, &nl);             // (INT + NULL)
    Item_func_add add_nl_ll0(&nl, &ll0);             // (NULL + INT)

    Item_func_add add_ll_ll(&ll0, &ll1);                  // INT + INT
    Item_func_add add_ll_ll_ll0(&add_ll_ll, &ll0);  // (INT + INT) + INT
    Item_func_add add_ll_ll_ll1(&add_ll_ll, &ll1);         // (INT+INT)+INT
    Item_func_add add_ll_ll_ll_ll(&add_ll_ll_ll0, &ll1);   //
    Item_func_add add_ll_ll_ll_ll_ll0(&add_ll_ll_ll_ll, &ll0);// (INT + ...+ INT)

    Item *items[]=
    {
      &add_ll0_nl,
      &add_nl_ll0,
      &add_ll_ll,
      &add_ll_ll_ll0,
      &add_ll_ll_ll_ll_ll0,
      NULL
    };
    MethodStat st;
    for (uint i= 0; items[i]; i++)
      st+= items[i]->test_ll(count);
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
    Item_func_coalesce coalesce_ll(&ll0);           // COALESCE(INT)
    Item_func_coalesce coalesce_nl_nl(&nl, &nl);    // COALESCE(NULL,NULL)
    Item_func_coalesce coalesce_nl_ll0(&nl,&ll0);   // COALESCE(NULL,INT)
    Item_func_coalesce coalesce_nl_nl__ll0(&coalesce_nl_nl, &ll0);//((NULL,NULL),INT)

    Item *items[]=
    {
      &coalesce_ll,
      &coalesce_nl_ll0,
      &coalesce_nl_nl__ll0,
      NULL
    };

    MethodStat st;
    for (uint i= 0; items[i]; i++)
      st+= items[i]->test_ll(count);
    return st;
  }
};


class Test_d: public Test
{
public:
  const char *name() const override { return "double_misc"; }
  MethodStat run(ulonglong count) const override
  {
    Item_real d0(x);                                 // DBL
    Item_func_uminus uminus_d0(&d0);                 // -DBL
    Item_func_uminus uminus_uminus_d0(&uminus_d0);   // -(-DBL)

    MethodStat st;
    Item *items[]=
    {
      &d0,
      &uminus_d0,
      &uminus_uminus_d0,
      NULL
    };
    for (uint i= 0; items[i]; i++)
      st+= items[i]->test_d(count);
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

    Item_func_add add_nl_d0(&nl, &d0);                // (NULL+ DBL)
    Item_func_add add_d0_nl(&d0, &nl);                // (DBL + NULL)
    Item_func_add add_d_d(&d0, &d1);                  // (DBL + DBL)
    Item_func_add add_d_d_d0(&add_d_d, &d0);          // (DBL + DBL) + DBL
    Item_func_add add_d_d_d1(&add_d_d, &d1);          // (DBL + DBL) + DBL
    Item_func_add add_d_d_d_d(&add_d_d_d1, &d1);      // (DBL +...+DBL)
    Item_func_add add_d_d_d_d_d(&add_d_d_d_d, &d0);   // (DBL +...+DBL)

    Item *items[]=
    {
      &add_nl_d0,
      &add_d0_nl,
      &add_d_d,
      &add_d_d_d0,
      &add_d_d_d_d_d,
      NULL
    };
    MethodStat st;
    for (uint i= 0; items[i]; i++)
      st+= items[i]->test_d(count);
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
    Item_func_coalesce coalesce_d0(&d0);         // COALESCE(DBL)
    Item_func_coalesce coalesce_nl_d0(&nl, &d0); // COALESCE(NULL,DBL)
    Item_func_coalesce coalesce_nl_nl(&nl, &nl); // COALESCE(NULL,NULL)
    Item_func_coalesce coalesce_nl_nl_d0(&nl, &nl, &d0); // (NULL,NULL,DBL)
    Item_func_coalesce coalesce_nl_nl__d0(&coalesce_nl_nl, &d0);//((NULL,NULL),DBL)

    MethodStat st;
    Item *items[]=
    {
      &coalesce_d0,
      &coalesce_nl_d0,
      &coalesce_nl_nl_d0,
      &coalesce_nl_nl__d0,
      NULL
    };
    for (uint i= 0; items[i]; i++)
      st+= items[i]->test_d(count);
    return st;
  }
};



void run(const char *name, ulonglong count)
{
  static const Test_null_misc   test_null_misc;
  static const Test_b           test_b;
  static const Test_b_or        test_b_or;
  static const Test_b_coalesce  test_b_coalesce;
  static const Test_bool_isnull test_null_isnull;
  static const Test_ll          test_ll;
  static const Test_ll_plus     test_ll_plus;
  static const Test_ll_coalesce test_ll_coalesce;
  static const Test_d           test_d;
  static const Test_d_plus      test_d_plus;
  static const Test_d_coalesce  test_d_coalesce;

  static const Test *tests[]=
  {
    &test_null_misc,
    &test_null_isnull,
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
