#ifndef MYSQL_COM_H
#define MYSQL_COM_H

#if defined (__clang__)
#define COMPILER "clang"
#elif defined(__GNUC__)
#define COMPILER "gcc"
#elif defined(WIN32)
#define COMPILER "WIN32"
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
