#ifndef SQL_TYPE_INT_H
#define SQL_TYPE_INT_H

typedef int32_t int32;

typedef long long longlong;
typedef unsigned long long ulonglong;

class Int32_null
{
public:
  int32 value;
  bool is_null;
  explicit Int32_null(longlong val, bool is_null= false)
   :value(val), is_null(is_null)
  { }
  Int32_null()
   :value(0), is_null(true)
  { }
  inline Int32_null operator+(int32 other) const
  {
    if (!is_null)
      return Int32_null(value + other);
    return *this;
  }
};


class Longlong_null
{
public:
  longlong value;
  bool is_null;
  explicit Longlong_null(longlong val, bool is_null= false)
   :value(val), is_null(is_null)
  { }
  Longlong_null()
   :value(0), is_null(true)
  { }
  inline Longlong_null &operator+=(const Longlong_null &other)
  {
    if (!(is_null|= other.is_null))
      value+= other.value;
    else
    {
      value= 0;
      is_null= true;
    }
    return *this;
  }
  inline Longlong_null &operator+=(longlong other)
  {
    if (!is_null)
      value+= other;
    return *this;
  }
  inline Longlong_null operator+(longlong other) const
  {
    if (!is_null)
      return Longlong_null(value + other);
    return *this;
  }
};

#endif
