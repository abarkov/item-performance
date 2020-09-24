#ifndef SQL_TYPE_DECIMAL_H
#define SQL_TYPE_DECIMAL_H

#include "my_decimal_limits.h"
#include "decimal.h"


class my_decimal:public decimal_t
{
  public:
  decimal_digit_t buffer[DECIMAL_BUFF_LENGTH];
  my_decimal(decimal_t &val): decimal_t(val)
  {
    init();
    for (uint i= 0; i < DECIMAL_BUFF_LENGTH; i++)
      buffer[i]= val.buf[i];
  }

  my_decimal()
  {
    init();
    decimal_make_zero(this);
  }

  my_decimal(const longlong &val)
  {
    init();
    longlong2decimal(val, this);
  }

  my_decimal(const double &val)
  {
    init();
    double2decimal(val, this);
  }

  void init()
  {
    len= DECIMAL_BUFF_LENGTH;
    buf= buffer;
    TRASH_ALLOC(buffer, sizeof(buffer));
  }
};

class Decimal_null: public my_decimal
{
public:
  bool is_null;

  Decimal_null(decimal_t &val, bool is_null= false)
   :my_decimal(val), is_null(is_null)
  { }

  Decimal_null(const Decimal_null &other)
   :my_decimal(other), is_null(other.is_null)
  { }

  Decimal_null()
   :is_null(true)
  { }

  Decimal_null(const longlong &val, const bool is_null= false)
   :my_decimal(val), is_null(is_null)
  { }

  Decimal_null(const double &val, const bool is_null= false)
   :my_decimal(val), is_null(is_null)
  { }

  inline Decimal_null & neg()
  {
    decimal_neg(this);
    return *this;
  }

  Decimal_null & operator+=(const Decimal_null &other)
  {
    if (!other.is_null && !is_null)
    {
      my_decimal res;
      decimal_add(this, &other, &res);
      (*this)= Decimal_null(res, false);
    }
    else
      is_null= true;

    return *this;
  }
};

#endif
