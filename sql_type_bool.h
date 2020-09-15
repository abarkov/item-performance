#ifndef SQL_TYPE_BOOL_H
#define SQL_TYPE_BOOL_H


class Bool_null
{
public:
  bool value;
  bool is_null;
  explicit Bool_null(bool val, bool is_null= false)
   :value(val), is_null(is_null)
  { }
  Bool_null()
   :value(false), is_null(true)
  { }
};

#endif
