#ifndef MYSQL_COM_H
#define MYSQL_COM_H

#define likely(x)      __builtin_expect(!!(x), 1)
#define unlikely(x)    __builtin_expect(!!(x), 0)

#if defined (__clang__)
#define COMPILER "clang"
#elif defined(__GNUC__)
#define COMPILER "gcc"
#endif


enum enum_field_types
{
  MYSQL_TYPE_NULL=       0,
  MYSQL_TYPE_BOOL=       1,
  MYSQL_TYPE_LONGLONG=   2,
  MYSQL_TYPE_DOUBLE=     3,
  MYSQL_TYPE_NEWDECIMAL= 4
};

#endif
