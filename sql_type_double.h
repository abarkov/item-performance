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
  inline Double_null & neg()
  {
    value= -value;
    return *this;
  }
};

#endif
