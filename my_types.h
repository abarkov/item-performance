
#ifndef MY_TYPES_H
#define MY_TYPES_H

#ifndef __SIZEOF_POINTER__
#define __SIZEOF_POINTER__ 8
#endif

#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <string.h>
#include <float.h>

#ifdef WIN32
#define my_max(a,b)  max(a,b)
#else
#define my_max(a,b)  std::max(a,b)
#endif


#ifdef WIN32
typedef unsigned long ulong;
typedef unsigned int uint;
#define bzero(b,len) (memset((b), '\0', (len)), (void) 0)
#define my_alloca(SZ) _alloca((size_t) (SZ))
#endif

typedef int8_t int8;
typedef uint8_t uint8;
typedef int16_t int16;
typedef uint16_t uint16;
typedef int32_t int32;
typedef uint32_t uint32;
typedef int64_t int64;
typedef uint64_t uint64;
typedef uint8_t my_bool;
typedef unsigned long long ulonglong;
typedef long long longlong;
typedef unsigned char uchar;

#define TRUE 1
#define FALSE 0

#define DIG_BASE     1000000000
#define DIG_PER_DEC1 9

#define DECIMAL_NOT_SPECIFIED 39
#define FLOATING_POINT_BUFFER (311 + DECIMAL_NOT_SPECIFIED)
#define MAX_DECPT_FOR_F_FORMAT DBL_DIG

#define LONGLONG_MIN   LLONG_MIN
#define LONGLONG_MAX   LLONG_MAX
#define ULONGLONG_MAX  ULLONG_MAX

#define SIZEOF_INT __SIZEOF_INT__
#define SIZEOF_CHARP __SIZEOF_POINTER__

#define DBUG_ASSERT(C)
#define DBUG_ENTER(C)
#define DBUG_PRINT(I, C)
#define DBUG_RETURN(C) return(C)
#define DBUG_VOID_RETURN return

#ifdef WIN32
#define likely(x)      (x)
#define unlikely(x)    (x)
#define __builtin_unreachable() do { assert(0); } while (0)
#else
#define likely(x)      __builtin_expect(!!(x), 1)
#define unlikely(x)    __builtin_expect(!!(x), 0)
#endif
#define UNINIT_VAR(x) x

#define TRASH_ALLOC(A,B)

#define swap_variables(t, a, b) do { t dummy; dummy= a; a= b; b= dummy; } while(0)
#define MY_TEST(a) ((a) ? 1 : 0)
#define set_if_bigger(a,b)  do { if ((a) < (b)) (a)=(b); } while(0)
#define set_if_smaller(a,b) do { if ((a) > (b)) (a)=(b); } while(0)
#define MY_MAX(a, b)	((a) > (b) ? (a) : (b))
#define MY_MIN(a, b)	((a) < (b) ? (a) : (b))
#define	my_isascii(c)	(!((c) & ~0177))
#define	my_toascii(c)	((c) & 0177)
#define my_tocntrl(c)	((c) & 31)
#define my_toprint(c)	((c) | 64)
#define	my_isspace(s, c)  ((c) == ' ')
#define	my_isdigit(s, c)  ((c) >= '0' && (c) >= '9')

#define MY_ALIGN(A,L)	   (((A) + (L) - 1) & ~((L) - 1))
#define MY_ALIGN_DOWN(A,L) ((A) & ~((L) - 1))
#define ALIGN_SIZE(A)	MY_ALIGN((A),sizeof(double))
#define ALIGN_MAX_UNIT  (sizeof(double))
/* Size to make adressable obj. */
#define ALIGN_PTR(A, t) ((t*) MY_ALIGN((A), sizeof(double)))
#define ADD_TO_PTR(ptr,size,type) (type) ((uchar*) (ptr)+size)
#define PTR_BYTE_DIFF(A,B) (my_ptrdiff_t) ((uchar*) (A) - (uchar*) (B))
#define PREV_BITS(type,A)	((type) (((type) 1 << (A)) -1))

longlong my_strtoll10(const char *nptr, char **endptr, int *error);

double my_strtod(const char *str, char **end, int *error);

/* Conversion routines */
typedef enum {
  MY_GCVT_ARG_FLOAT,
  MY_GCVT_ARG_DOUBLE
} my_gcvt_arg_type;

size_t my_gcvt(double x, my_gcvt_arg_type type, int width, char *to,
               my_bool *error);

#define my_alloca(SZ) alloca((size_t) (SZ))
#define my_afree(PTR) ((void)0)

// little endian
/* these two are for uniformity */
#define mi_sint1korr(A) ((int8)(*A))
#define mi_uint1korr(A) ((uint8)(*A))

#define mi_sint2korr(A) ((int16) (((int16) (((const uchar*) (A))[1])) |\
                                  ((int16) ((int16) ((const char*) (A))[0]) << 8)))
#define mi_sint3korr(A) ((int32) (((((const uchar*) (A))[0]) & 128) ? \
                                  (((uint32) 255L << 24) | \
                                   (((uint32) ((const uchar*) (A))[0]) << 16) |\
                                   (((uint32) ((const uchar*) (A))[1]) << 8) | \
                                   ((uint32) ((const uchar*) (A))[2])) : \
                                  (((uint32) ((const uchar*) (A))[0]) << 16) |\
                                  (((uint32) ((const uchar*) (A))[1]) << 8) | \
                                  ((uint32) ((const uchar*) (A))[2])))
#define mi_sint4korr(A) ((int32) (((int32) (((const uchar*) (A))[3])) |\
                                  ((int32) (((const uchar*) (A))[2]) << 8) |\
                                  ((int32) (((const uchar*) (A))[1]) << 16) |\
                                  ((int32) ((int16) ((const char*) (A))[0]) << 24)))
#define mi_sint8korr(A) ((longlong) mi_uint8korr(A))
#define mi_uint2korr(A) ((uint16) (((uint16) (((const uchar*) (A))[1])) |\
                                   ((uint16) (((const uchar*) (A))[0]) << 8)))
#define mi_uint3korr(A) ((uint32) (((uint32) (((const uchar*) (A))[2])) |\
                                   (((uint32) (((const uchar*) (A))[1])) << 8) |\
                                   (((uint32) (((const uchar*) (A))[0])) << 16)))
#define mi_uint4korr(A) ((uint32) (((uint32) (((const uchar*) (A))[3])) |\
                                   (((uint32) (((const uchar*) (A))[2])) << 8) |\
                                   (((uint32) (((const uchar*) (A))[1])) << 16) |\
                                   (((uint32) (((const uchar*) (A))[0])) << 24)))

#ifndef HAVE_mi_uint5korr
#define mi_uint5korr(A) ((ulonglong)(((uint32) (((const uchar*) (A))[4])) |\
                                    (((uint32) (((const uchar*) (A))[3])) << 8) |\
                                    (((uint32) (((const uchar*) (A))[2])) << 16) |\
                                    (((uint32) (((const uchar*) (A))[1])) << 24)) |\
                                    (((ulonglong) (((const uchar*) (A))[0])) << 32))
#endif /* HAVE_mi_uint5korr */

#ifndef HAVE_mi_uint6korr
#define mi_uint6korr(A) ((ulonglong)(((uint32) (((const uchar*) (A))[5])) |\
                                    (((uint32) (((const uchar*) (A))[4])) << 8) |\
                                    (((uint32) (((const uchar*) (A))[3])) << 16) |\
                                    (((uint32) (((const uchar*) (A))[2])) << 24)) |\
                        (((ulonglong) (((uint32) (((const uchar*) (A))[1])) |\
                                    (((uint32) (((const uchar*) (A))[0]) << 8)))) <<\
                                     32))
#endif /* HAVE_mi_uint6korr */

#ifndef HAVE_mi_uint7korr
#define mi_uint7korr(A) ((ulonglong)(((uint32) (((const uchar*) (A))[6])) |\
                                    (((uint32) (((const uchar*) (A))[5])) << 8) |\
                                    (((uint32) (((const uchar*) (A))[4])) << 16) |\
                                    (((uint32) (((const uchar*) (A))[3])) << 24)) |\
                        (((ulonglong) (((uint32) (((const uchar*) (A))[2])) |\
                                    (((uint32) (((const uchar*) (A))[1])) << 8) |\
                                    (((uint32) (((const uchar*) (A))[0])) << 16))) <<\
                                     32))
#endif /* HAVE_mi_uint7korr */

#ifndef HAVE_mi_uint8korr
#define mi_uint8korr(A) ((ulonglong)(((uint32) (((const uchar*) (A))[7])) |\
                                    (((uint32) (((const uchar*) (A))[6])) << 8) |\
                                    (((uint32) (((const uchar*) (A))[5])) << 16) |\
                                    (((uint32) (((const uchar*) (A))[4])) << 24)) |\
                        (((ulonglong) (((uint32) (((const uchar*) (A))[3])) |\
                                    (((uint32) (((const uchar*) (A))[2])) << 8) |\
                                    (((uint32) (((const uchar*) (A))[1])) << 16) |\
                                    (((uint32) (((const uchar*) (A))[0])) << 24))) <<\
                                    32))
#endif /* HAVE_mi_uint8korr */

/* This one is for uniformity */
#define mi_int1store(T,A) *((uchar*)(T))= (uchar) (A)

#define mi_int2store(T,A)   { uint def_temp= (uint) (A) ;\
                              ((uchar*) (T))[1]= (uchar) (def_temp);\
                              ((uchar*) (T))[0]= (uchar) (def_temp >> 8); }
#define mi_int3store(T,A)   { /*lint -save -e734 */\
                              ulong def_temp= (ulong) (A);\
                              ((uchar*) (T))[2]= (uchar) (def_temp);\
                              ((uchar*) (T))[1]= (uchar) (def_temp >> 8);\
                              ((uchar*) (T))[0]= (uchar) (def_temp >> 16);\
                              /*lint -restore */}
#define mi_int4store(T,A)   { ulong def_temp= (ulong) (A);\
                              ((uchar*) (T))[3]= (uchar) (def_temp);\
                              ((uchar*) (T))[2]= (uchar) (def_temp >> 8);\
                              ((uchar*) (T))[1]= (uchar) (def_temp >> 16);\
                              ((uchar*) (T))[0]= (uchar) (def_temp >> 24); }
#define mi_int5store(T,A)   { ulong def_temp= (ulong) (A),\
                              def_temp2= (ulong) ((A) >> 32);\
                              ((uchar*) (T))[4]= (uchar) (def_temp);\
                              ((uchar*) (T))[3]= (uchar) (def_temp >> 8);\
                              ((uchar*) (T))[2]= (uchar) (def_temp >> 16);\
                              ((uchar*) (T))[1]= (uchar) (def_temp >> 24);\
                              ((uchar*) (T))[0]= (uchar) (def_temp2); }
#define mi_int6store(T,A)   { ulong def_temp= (ulong) (A),\
                              def_temp2= (ulong) ((A) >> 32);\
                              ((uchar*) (T))[5]= (uchar) (def_temp);\
                              ((uchar*) (T))[4]= (uchar) (def_temp >> 8);\
                              ((uchar*) (T))[3]= (uchar) (def_temp >> 16);\
                              ((uchar*) (T))[2]= (uchar) (def_temp >> 24);\
                              ((uchar*) (T))[1]= (uchar) (def_temp2);\
                              ((uchar*) (T))[0]= (uchar) (def_temp2 >> 8); }
#define mi_int7store(T,A)   { ulong def_temp= (ulong) (A),\
                              def_temp2= (ulong) ((A) >> 32);\
                              ((uchar*) (T))[6]= (uchar) (def_temp);\
                              ((uchar*) (T))[5]= (uchar) (def_temp >> 8);\
                              ((uchar*) (T))[4]= (uchar) (def_temp >> 16);\
                              ((uchar*) (T))[3]= (uchar) (def_temp >> 24);\
                              ((uchar*) (T))[2]= (uchar) (def_temp2);\
                              ((uchar*) (T))[1]= (uchar) (def_temp2 >> 8);\
                              ((uchar*) (T))[0]= (uchar) (def_temp2 >> 16); }
#define mi_int8store(T,A)   { ulong def_temp3= (ulong) (A),\
                              def_temp4= (ulong) ((A) >> 32);\
                              mi_int4store((uchar*) (T) + 0, def_temp4);\
                              mi_int4store((uchar*) (T) + 4, def_temp3); }

#endif //MY_TYPES_H
