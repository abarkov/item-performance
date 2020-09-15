#ifndef SQL_TYPE_DOUBLE_H
#define SQL_TYPE_DOUBLE_H

class Double_null
{
public:
  double value;
  bool is_null;
  explicit Double_null(double val, bool is_null= false)
   :value(val), is_null(is_null)
  { }
  Double_null()
   :value(0), is_null(true)
  { }
  inline Double_null &operator+=(const Double_null &other)
  {
    if (!(is_null|= other.is_null))
    {
      value+= other.value;
      return *this;
    }
    value= 0;
    return *this;
  }
  inline Double_null operator+(double other) const
  {
    if (!is_null)
      return Double_null(value + other);
    return *this;
  }
};

#endif
