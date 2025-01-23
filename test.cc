#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include "my_types.h"
#include "timer.h"

#include "opt.h"

#include "item.h"
#include "item_func.h"

constexpr ulonglong default_count= 10*1000*1000ULL;

volatile int x= 1;
volatile int y= 2;


class StatAllNative: public StatAll
{
public:
  StatAllNative(Item *item, const Options &opt)
   :StatAll(
#ifdef HAVE_NULL_VALUE
    item->test_native_old(opt),
#else
    Stat(),
#endif
    item->test_native_prm(opt),
    item->test_native_get(opt),
    item->test_native_new(opt))
  {}
};


void MethodStat::print(const char *method) const
{
  size_t length= strlen(method);
  int space_count= length < 9 ? (int) (9 - length) : 0;
  printf("val_%s        %-*s%f\n", method, space_count, "", val_xxx);
  printf("val_%s_null   %-*s%f\n", method, space_count, "", val_xxx_null);
  printf("get_%s        %-*s%f\n", method, space_count, "", get_xxx);
  printf("to_%s_null    %-*s%f\n", method, space_count, "", to_xxx_null);
  printf("\n");
}


void MethodStatByType::print() const
{
  st_bool.print_if("bool");
  st_int32.print_if("int32");
  st_longlong.print_if("longlong");
  st_double.print_if("double");
  st_decimal.print_if("decimal");
}


class Test
{
protected:
  bool name_matches(const char *mask) const
  {
    size_t mask_length= strlen(mask);
    return !strncmp(mask, name(), mask_length);
  }
public:
  Test() { }
  virtual ~Test() { }
  virtual const char *name() const = 0;
  virtual MethodStatByType run(const Options &opt) const = 0;
  virtual bool matches(const Options &opt, const char *mask) const
  {
    return opt.big_test() < 2 && name_matches(mask);
  }
  void print(Item *item, const Stat &st) const
  {
    string s;
    item->print(&s);
    if (st.use_sum_d)
      printf("%s %-16s %f %s %.2f\n",
             COMPILER, st.method, st.time_spent, s.c_str(), st.sum_d);
    else
      printf("%s %-16s %f %s %lld\n",
             COMPILER, st.method, st.time_spent, s.c_str(), st.sum_ll);
  }
  void print(Item *item, const StatAll &st) const
  {
#ifdef HAVE_NULL_VALUE
    print(item, st.val_xxx);
#endif
    print(item, st.val_xxx_null);
    print(item, st.get_xxx);
    print(item, st.to_xxx_null);
  }


  MethodStatByType test_b(Item *item, const Options &opt) const
  {
    StatAll st(
#ifdef HAVE_NULL_VALUE
      item->test_b_old(opt),
#else
      Stat(),
#endif
      item->test_b_prm(opt),
      item->test_b_get(opt),
      item->test_b_new(opt));
    print(item, st);
    printf("\n");
    MethodStatByType stt;
    stt.st_bool+= MethodStat(st);
    return stt;
  }

  MethodStatByType test_d(Item *item, const Options &opt) const
  {
    StatAll st(
#ifdef HAVE_NULL_VALUE
      item->test_d_old(opt),
#else
      Stat(),
#endif
      item->test_d_prm(opt),
      item->test_d_get(opt),
      item->test_d_new(opt));
    print(item, st);
    printf("\n");
    MethodStatByType stt;
    stt.st_double+= MethodStat(st);
    return stt;
  }

  MethodStatByType test_dec(Item *item, const Options &opt) const
  {
    StatAll st(
#ifdef HAVE_NULL_VALUE
      item->test_dec_old(opt),
#else
      Stat(),
#endif
      item->test_dec_prm(opt),
      item->test_dec_get(opt),
      item->test_dec_new(opt));
    print(item, st);
    printf("\n");
    MethodStatByType stt;
    stt.st_decimal+= MethodStat(st);
    return stt;
  }

  MethodStatByType test_int32(Item *item, const Options &opt) const
  {
    StatAll st(
#ifdef HAVE_NULL_VALUE
      item->test_int32_old(opt),
#else
      Stat(),
#endif
      item->test_int32_prm(opt),
      item->test_int32_get(opt),
      item->test_int32_new(opt));
    print(item, st);
    MethodStatByType stt;
    stt.st_int32+= MethodStat(st);
    return stt;
  }

  MethodStatByType test_ll(Item *item, const Options &opt) const
  {
    MethodStatByType stt;
    if (opt.int32api())
      stt+= test_int32(item, opt);

    StatAll st(
#ifdef HAVE_NULL_VALUE
      item->test_ll_old(opt),
#else
      Stat(),
#endif
      item->test_ll_prm(opt),
      item->test_ll_get(opt),
      item->test_ll_new(opt));
    print(item, st);
    stt.st_longlong+= MethodStat(st);
    printf("\n");

    return stt;
  }


  MethodStatByType test_native(Item *item, const Options &opt) const
  {

    MethodStatByType stt;
    switch (item->field_type()) {
    case MYSQL_TYPE_NULL:
    case MYSQL_TYPE_BOOL:
    {
      StatAllNative st(item, opt);
      print(item, st);
      stt.st_bool+= MethodStat(st);
      break;
    }
    case MYSQL_TYPE_LONGLONG:
    {
      Options opt_ll(opt);
      opt_ll.set_int32api(false);

      if (opt.int32api())
      {
        StatAllNative st_int32(item, opt);
        print(item, st_int32);
        stt.st_int32+= MethodStat(st_int32);
      }
      StatAllNative st_ll(item, opt_ll);
      print(item, st_ll);
      stt.st_longlong+= MethodStat(st_ll);
      break;
    }
    case MYSQL_TYPE_DOUBLE:
    {
      StatAllNative st(item, opt);
      print(item, st);

      stt.st_double+= MethodStat(st);
      break;
    }
    case MYSQL_TYPE_NEWDECIMAL:
    {
      StatAllNative st(item, opt);
      print(item, st);

      stt.st_decimal+= MethodStat(st);
      break;
    }
    }
    printf("\n");
    return stt;
  }

  MethodStatByType test_native(Item **items, const Options &opt) const
  {
    MethodStatByType st;
    for (uint i= 0; items[i]; i++)
      st+= test_native(items[i], opt);
    return st;
  }
};


class Test_null_misc: public Test
{
public:
  const char *name() const override { return "null_misc"; }
  MethodStatByType run(const Options &opt) const override
  {
    Item *nl= new Item_null();                           // NULL
    Item *b0= new Item_bool(false);                      // FALSE
    Item *d0= new Item_real(x);                          // 0e0
    Item *ll0= new Item_int(x);                          // 0
    Item *coalesce_nl= new Item_func_coalesce(nl);       // COALESCE(NULL)
    Item *coalesce_nl_nl= new Item_func_coalesce(nl,nl); // COALESCE(NULL,NULL)

    MethodStatByType st;
    st+= test_b(nl, opt);
    st+= test_ll(nl, opt);
    st+= test_d(nl, opt);
    st+= test_b(coalesce_nl, opt);
    st+= test_b(coalesce_nl_nl, opt);

    delete nl;
    delete b0;
    delete d0;
    delete ll0;
    delete coalesce_nl;
    delete coalesce_nl_nl;
    return st;
  }
};


class Test_bool_isnull: public Test
{
public:
  const char *name() const override { return "bool_isnull"; }
  MethodStatByType run(const Options &opt) const override
  {
    Item *nl= new Item_null;                       // NULL
    Item *b0= new Item_bool(false);                // FALSE
    Item *d0= new Item_real(x);                    // 0e0
    Item *ll0= new Item_int(x);                    // 0
    Item *isnull_nl= new Item_func_isnull(nl);     // NULL IS NULL
    Item *isnull_b0= new Item_func_isnull(b0);     // FALSE IS NULL
    Item *isnull_ll0= new Item_func_isnull(ll0);   // 0 IS NULL
    Item *isnull_d0= new Item_func_isnull(d0);     // 0e0 IS NULL

    Item *items[]=
    {
      isnull_nl,
      isnull_b0,
      isnull_ll0,
      isnull_d0,
      NULL
    };
    return test_native(items, opt);
  }
};


class Test_bool_misc: public Test
{
public:
  const char *name() const override { return "bool_misc"; }
  MethodStatByType run(const Options &opt) const override
  {
    Item *b0= new Item_bool(false);                // FALSE
    Item *b1= new Item_bool(true);                 // TRUE
    Item *items[]= {b0, b1, NULL};
    return test_native(items, opt);
  }
};


class Test_bool_eq: public Test
{
public:
  const char *name() const override { return "bool_eq"; }
  MethodStatByType run(const Options &opt) const override
  {
    Item *nl= new Item_null;
    Item *b0= new Item_bool(false);
    Item *cmp1= new Item_func_eq(nl, nl);
    Item *cmp2= new Item_func_eq(b0, nl);
    Item *cmp3= new Item_func_eq(b0, b0);

    Item *items[]= {cmp1, cmp2, cmp3, NULL};
    return test_native(items, opt);
  }
};


class Test_bool_or: public Test
{
public:
  const char *name() const override { return "bool_or"; }
  MethodStatByType run(const Options &opt) const override
  {
    Item *nl= new Item_null;
    Item *b0= new Item_bool(false);
    Item *b1= new Item_bool(true);

    Item *or_nl_nl= new Item_cond_or(nl, nl);        // NULL OR NULL
    Item *or_nl_b0= new Item_cond_or(nl, b0);        // NULL OR FALSE
    Item *or_nl_b1= new Item_cond_or(nl, b1);        // NULL OR TRUE
    Item *or_b0_nl= new Item_cond_or(b0, nl);        // FALSE OR NULL
    Item *or_b0_b0= new Item_cond_or(b0, b0);        // FALSE OR FALSE
    Item *or_b0_b1= new Item_cond_or(b0, b1);        // FALSE OR TRUE
    Item *or_b1_b0= new Item_cond_or(b1, b0);        // TRUE OR FALSE
    Item *or_b1_b1= new Item_cond_or(b1, b1);        // TRUE OR TRUE

    Item *or_b0_nl_b1= new Item_cond_or(b0, nl, b1);   // FALSE OR NULL OR TRUE
    Item *or_b0_nl__b1= new Item_cond_or(or_b0_nl, b1);  // (FALSE OR NULL) OR TRUE

    Item *items[]=
    {
      or_nl_nl,
      or_nl_b0,
      or_nl_b1,
      or_b0_nl,
      or_b0_b0,
      or_b0_b1,
      or_b1_b0,
      or_b1_b1,
      or_b0_nl_b1,
      or_b0_nl__b1,
      NULL
    };
    return test_native(items, opt);
  }
};


class Test_bool_coalesce: public Test
{
public:
  const char *name() const override { return "bool_coalesce"; }
  MethodStatByType run(const Options &opt) const override
  {
    Item *nl= new Item_null;
    Item *b0= new Item_bool(false);
    Item *coalesce_b0= new Item_func_coalesce(b0);        // COALESCE(FALSE)
    Item *coalesce_nl_b0= new Item_func_coalesce(nl, b0); // COALESCE(NULL,FALSE)
 
    Item *items[]= {coalesce_b0, coalesce_nl_b0, NULL};
    return test_native(items, opt);
  }
};


class Test_bool_last_value: public Test
{
public:
  const char *name() const override { return "bool_last_value"; }
  MethodStatByType run(const Options &opt) const override
  {
    Item *b0= new Item_bool(false);
    Item *lv_b0= new Item_func_last_value(b0);
    Item *lv_b0_b0_b0= new Item_func_last_value(b0, b0, b0);
 
    Item *items[]= {lv_b0, lv_b0_b0_b0, NULL};
    return test_native(items, opt);
  }
};


class Test_ll_misc: public Test
{
public:
  const char *name() const override { return "ll_misc"; }
  MethodStatByType run(const Options &opt) const override
  {
    Item *ll0= new Item_int(x);                                   // INT
    Item *uminus_ll0= new Item_func_uminus(ll0);                  // -INT
    Item *uminus_uminus_ll0= new Item_func_uminus(uminus_ll0);    // -(-INT)
 
    Item *items[]= {ll0, uminus_ll0, uminus_uminus_ll0, NULL};
    return test_native(items, opt);
  }
};


class Test_ll_plus: public Test
{
public:
  const char *name() const override { return "ll_plus"; }
  MethodStatByType run(const Options &opt) const override
  {
    Item *nl= new Item_null;                              // NULL
    Item *ll0= new Item_int(x);                               // INT
    Item *ll1= new Item_int(y);                               // INT

    Item *add_ll0_nl= new Item_func_add(ll0, nl);             // (INT + NULL)
    Item *add_nl_ll0= new Item_func_add(nl, ll0);             // (NULL + INT)

    Item *add_ll_ll= new Item_func_add(ll0, ll1);             // INT + INT
    Item *add_ll_ll_ll0= new Item_func_add(add_ll_ll, ll0);   // (INT + INT) + INT
    Item *add_ll_ll_ll1= new Item_func_add(add_ll_ll, ll1);   // (INT+INT)+INT
    Item *add_ll_ll_ll_ll= new Item_func_add(add_ll_ll_ll0, ll1);   //
    Item *add_ll_ll_ll_ll_ll0= new Item_func_add(add_ll_ll_ll_ll, ll0);// (INT + ...+ INT)

    Item *items[]=
    {
      add_ll0_nl,
      add_nl_ll0,
      add_ll_ll,
      add_ll_ll_ll0,
      add_ll_ll_ll1,
      add_ll_ll_ll_ll_ll0,
      NULL
    };
    return test_native(items, opt);
  }
};


class Test_ll_coalesce: public Test
{
public:
  const char *name() const override { return "ll_coalesce"; }
  MethodStatByType run(const Options &opt) const override
  {
    Item *nl= new Item_null;
    Item *ll0= new Item_int(x);
    Item *coalesce_ll= new Item_func_coalesce(ll0);          // COALESCE(INT)
    Item *coalesce_nl_nl= new Item_func_coalesce(nl, nl);    // COALESCE(NULL,NULL)
    Item *coalesce_nl_ll0= new Item_func_coalesce(nl,ll0);   // COALESCE(NULL,INT)
    Item *coalesce_nl_nl__ll0= new Item_func_coalesce(coalesce_nl_nl, ll0);//((NULL,NULL),INT)

    Item *items[]=
    {
      coalesce_ll,
      coalesce_nl_ll0,
      coalesce_nl_nl__ll0,
      NULL
    };

    return test_native(items, opt);
  }
};


class Test_ll_last_value: public Test
{
public:
  const char *name() const override { return "ll_last_value"; }
  MethodStatByType run(const Options &opt) const override
  {
    Item *b0= new Item_int(x);
    Item *lv_b0= new Item_func_last_value(b0);
    Item *lv_b0_b0_b0= new Item_func_last_value(b0, b0, b0);
 
    Item *items[]= {lv_b0, lv_b0_b0_b0, NULL};
    return test_native(items, opt);
  }
};


class Test_ll_eq: public Test
{
public:
  const char *name() const override { return "ll_eq"; }
  MethodStatByType run(const Options &opt) const override
  {
    Item *nl= new Item_null;
    Item *b0= new Item_int(x);
    Item *cmp1= new Item_func_eq(nl, nl);
    Item *cmp2= new Item_func_eq(b0, nl);
    Item *cmp3= new Item_func_eq(b0, b0);

    Item *items[]= {cmp1, cmp2, cmp3, NULL};
    return test_native(items, opt);
  }
};


class Test_double_misc: public Test
{
public:
  const char *name() const override { return "double_misc"; }
  MethodStatByType run(const Options &opt) const override
  {
    Item *d0= new Item_real(x);                                 // DBL
    Item *uminus_d0= new Item_func_uminus(d0);                 // -DBL
    Item *uminus_uminus_d0= new Item_func_uminus(uminus_d0);   // -(-DBL)

    MethodStatByType st;
    Item *items[]=
    {
      d0,
      uminus_d0,
      uminus_uminus_d0,
      NULL
    };
    return test_native(items, opt);
  }
};


class Test_double_eq: public Test
{
public:
  const char *name() const override { return "double_eq"; }
  MethodStatByType run(const Options &opt) const override
  {
    Item *nl= new Item_null;
    Item *b0= new Item_real(x);
    Item *cmp1= new Item_func_eq(nl, nl);
    Item *cmp2= new Item_func_eq(b0, nl);
    Item *cmp3= new Item_func_eq(b0, b0);

    Item *items[]= {cmp1, cmp2, cmp3, NULL};
    return test_native(items, opt);
  }
};


class Test_double_plus: public Test
{
public:
  const char *name() const override { return "double_plus"; }
  MethodStatByType run(const Options &opt) const override
  {
    Item *nl= new Item_null();
    Item *d0= new Item_real(x);
    Item *d1= new Item_real(y);

    Item *add_nl_d0= new Item_func_add(nl, d0);                // (NULL+ DBL)
    Item *add_d0_nl= new Item_func_add(d0, nl);                // (DBL + NULL)
    Item *add_d_d= new Item_func_add(d0, d1);                  // (DBL + DBL)
    Item *add_d_d_d0= new Item_func_add(add_d_d, d0);          // (DBL + DBL) + DBL
    Item *add_d_d_d1= new Item_func_add(add_d_d, d1);          // (DBL + DBL) + DBL
    Item *add_d_d_d_d= new Item_func_add(add_d_d_d1, d1);      // (DBL +...+DBL)
    Item *add_d_d_d_d_d= new Item_func_add(add_d_d_d_d, d0);   // (DBL +...+DBL)

    Item *items[]=
    {
      add_nl_d0,
      add_d0_nl,
      add_d_d,
      add_d_d_d0,
      add_d_d_d_d_d,
      NULL
    };
    return test_native(items, opt);
  }
};


class Test_double_coalesce: public Test
{
public:
  const char *name() const override { return "double_coalesce"; }
  MethodStatByType run(const Options &opt) const override
  {
    Item *nl= new Item_null;
    Item *d0= new Item_real(x);
    Item *coalesce_d0= new Item_func_coalesce(d0);        // COALESCE(DBL)
    Item *coalesce_nl_d0= new Item_func_coalesce(nl, d0); // COALESCE(NULL,DBL)
    Item *coalesce_nl_nl= new Item_func_coalesce(nl, nl); // COALESCE(NULL,NULL)
    Item *coalesce_nl_nl_d0= new Item_func_coalesce(nl, nl, d0); // (NULL,NULL,DBL)
    Item *coalesce_nl_nl__d0= new Item_func_coalesce(coalesce_nl_nl, d0);//((NULL,NULL),DBL)

    MethodStatByType st;
    Item *items[]=
    {
      coalesce_d0,
      coalesce_nl_d0,
      coalesce_nl_nl_d0,
      coalesce_nl_nl__d0,
      NULL
    };
    return test_native(items, opt);
  }
};


class Test_double_last_value: public Test
{
public:
  const char *name() const override { return "double_last_value"; }
  MethodStatByType run(const Options &opt) const override
  {
    Item *d0= new Item_real(x);
    Item *lv_b0= new Item_func_last_value(d0);
    Item *lv_b0_b0_b0= new Item_func_last_value(d0, d0, d0);

    Item *items[]= {lv_b0, lv_b0_b0_b0, NULL};
    return test_native(items, opt);
  }
};


class Test_decimal_misc: public Test
{
public:
  const char *name() const override { return "decimal_misc"; }
  MethodStatByType run(const Options &opt) const override
  {
    Item *d0= new Item_decimal((longlong)x);                   // DEC
    Item *uminus_d0= new Item_func_uminus(d0);                 // -DEC
    Item *uminus_uminus_d0= new Item_func_uminus(uminus_d0);   // -(-DEC)

    MethodStatByType st;
    Item *items[]=
    {
      d0,
      uminus_d0,
      uminus_uminus_d0,
      NULL
    };
    return test_native(items, opt);
  }
};

class Test_decimal_eq: public Test
{
public:
  const char *name() const override { return "decimal_eq"; }
  MethodStatByType run(const Options &opt) const override
  {
    Item *nl= new Item_null;
    Item *b0= new Item_decimal((longlong)x);
    Item *cmp1= new Item_func_eq(nl, nl);
    Item *cmp2= new Item_func_eq(b0, nl);
    Item *cmp3= new Item_func_eq(b0, b0);

    Item *items[]= {cmp1, cmp2, cmp3, NULL};
    return test_native(items, opt);
  }
};

class Test_decimal_plus: public Test
{
public:
  const char *name() const override { return "decimal_plus"; }
  MethodStatByType run(const Options &opt) const override
  {
    Item *nl= new Item_null();
    Item *d0= new Item_decimal((longlong)x);
    Item *d1= new Item_decimal((longlong)y);

    Item *add_nl_d0= new Item_func_add(nl, d0);                // (NULL+ DBL)
    Item *add_d0_nl= new Item_func_add(d0, nl);                // (DBL + NULL)
    Item *add_d_d= new Item_func_add(d0, d1);                  // (DBL + DBL)
    Item *add_d_d_d0= new Item_func_add(add_d_d, d0);          // (DBL + DBL) + DBL
    Item *add_d_d_d1= new Item_func_add(add_d_d, d1);          // (DBL + DBL) + DBL
    Item *add_d_d_d_d= new Item_func_add(add_d_d_d1, d1);      // (DBL +...+DBL)
    Item *add_d_d_d_d_d= new Item_func_add(add_d_d_d_d, d0);   // (DBL +...+DBL)

    Item *items[]=
    {
      add_nl_d0,
      add_d0_nl,
      add_d_d,
      add_d_d_d0,
      add_d_d_d_d_d,
      NULL
    };
    return test_native(items, opt);
  }
};

class Test_decimal_coalesce: public Test
{
public:
  const char *name() const override { return "decimal_coalesce"; }
  MethodStatByType run(const Options &opt) const override
  {
    Item *nl= new Item_null;
    Item *d0= new Item_decimal((longlong)x);
    Item *coalesce_d0= new Item_func_coalesce(d0);        // COALESCE(DBL)
    Item *coalesce_nl_d0= new Item_func_coalesce(nl, d0); // COALESCE(NULL,DBL)
    Item *coalesce_nl_nl= new Item_func_coalesce(nl, nl); // COALESCE(NULL,NULL)
    Item *coalesce_nl_nl_d0= new Item_func_coalesce(nl, nl, d0); // (NULL,NULL,DBL)
    Item *coalesce_nl_nl__d0= new Item_func_coalesce(coalesce_nl_nl, d0);//((NULL,NULL),DBL)

    MethodStatByType st;
    Item *items[]=
    {
      coalesce_d0,
      coalesce_nl_d0,
      coalesce_nl_nl_d0,
      coalesce_nl_nl__d0,
      NULL
    };
    return test_native(items, opt);
  }
};

class Test_decimal_last_value: public Test
{
public:
  const char *name() const override { return "decimal_last_value"; }
  MethodStatByType run(const Options &opt) const override
  {
    Item *d0= new Item_real((longlong)x);
    Item *lv_b0= new Item_func_last_value(d0);
    Item *lv_b0_b0_b0= new Item_func_last_value(d0, d0, d0);

    Item *items[]= {lv_b0, lv_b0_b0_b0, NULL};
    return test_native(items, opt);
  }
};

class Test_random: public Test
{
public:
  const char *name() const override { return "random"; }
  bool matches(const Options &opt, const char *mask) const override
  {
    return opt.big_test() > 0 && name_matches(mask);
  }
  MethodStatByType run(const Options &opt) const override
  {
    std::pair<Item *, std::vector<std::unique_ptr<Item>>> g_random_tree=
      generate_tree(10);
    MethodStatByType st;
    st+= test_native(g_random_tree.first, opt);

    return st;
  }
};


MethodStatByType run(const char *name, const Options &opt)
{
  static const Test_null_misc         test_null_misc;
  static const Test_bool_misc         test_bool_misc;
  static const Test_bool_eq           test_bool_eq;
  static const Test_bool_or           test_bool_or;
  static const Test_bool_coalesce     test_bool_coalesce;
  static const Test_bool_isnull       test_null_isnull;
  static const Test_bool_last_value   test_bool_last_value;
  static const Test_ll_misc           test_ll_misc;
  static const Test_ll_eq             test_ll_eq;
  static const Test_ll_plus           test_ll_plus;
  static const Test_ll_coalesce       test_ll_coalesce;
  static const Test_ll_last_value     test_ll_last_value;
  static const Test_double_misc       test_double_misc;
  static const Test_double_eq         test_double_eq;
  static const Test_double_plus       test_double_plus;
  static const Test_double_coalesce   test_double_coalesce;
  static const Test_double_last_value test_double_last_value;
  static const Test_decimal_misc       test_decimal_misc;
  static const Test_decimal_eq         test_decimal_eq;
  static const Test_decimal_plus       test_decimal_plus;
  static const Test_decimal_coalesce   test_decimal_coalesce;
  static const Test_decimal_last_value test_decimal_last_value;
  static const Test_random            test_random;

  static const Test *tests[]=
  {
    &test_null_misc,
    &test_null_isnull,
    &test_bool_misc,
    &test_bool_eq,
    &test_bool_or,
    &test_bool_coalesce,
    &test_bool_last_value,
    &test_ll_misc,
    &test_ll_eq,
    &test_ll_plus,
    &test_ll_coalesce,
    &test_ll_last_value,
    &test_double_misc,
    &test_double_eq,
    &test_double_plus,
    &test_double_coalesce,
    &test_double_last_value,
    &test_decimal_misc,
    &test_decimal_eq,
    &test_decimal_plus,
    &test_decimal_coalesce,
    &test_decimal_last_value,
    &test_random,
    NULL
  };

  MethodStatByType stt;
  for (uint i= 0; tests[i]; i++)
  {
    if (tests[i]->matches(opt, name))
      stt+= tests[i]->run(opt);
  }
  return stt;
}


int main(int argc, char **argv)
{
  Options opt(argc, argv);

  if (opt.error())
  {
    opt.usage();
    return 1;
  }

  MethodStatByType stt;
  if (opt.used_options() == (uint) argc) // Run all tests
  {
    stt+= run("", opt);
  }
  else           // Run specified tests
  {
    argv+= opt.used_options();
    argc-= opt.used_options();
    for (int i= 0; i < argc; i++)
      stt+= run(argv[i], opt);
  }

  printf("By type:\n");
  stt.print();
  MethodStat st= stt.total();
  printf("Total:\n");
  st.print("xxx");

  return 0;
}
