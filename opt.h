#ifndef OPT_H_INCLUDED
#define OPT_H

typedef unsigned long long ulonglong;
typedef long long longlong;


class Options
{
  const char *m_progname;
  uint m_used_options;
  uint m_verbose;
  bool m_error;
  ulonglong m_count;

  void shift(int *ac, char ***av)
  {
    (*ac)--;
    (*av)++;
  }

  static bool get_ulonglong(ulonglong *to,
                            const char *name,
                            const char *value,
                            char **endptr)
  {
    long long val= strtoll(value, endptr, 10);
    if (val < 0)
    {
      printf("Bad %s value\n", name);
      return true;
    }
    *to= (ulonglong) val;
    return false;
  }

  static bool get_ulonglong(ulonglong *to, const char *name, const char *value)
  {
    char *endptr;
    return get_ulonglong(to, name, value, &endptr);
  }

  static bool get_ulonglong_size(ulonglong *to, const char *name, const char *value)
  {
    char *endptr;
    if (get_ulonglong(to, name, value, &endptr))
      return true;
    switch (endptr[0]) {
    case 'k':
    case 'K':
      *to*= 1024;
      break;
    case 'm':
    case 'M':
      *to*= 1024*1024;
    default:
      break;
    }
    return false;
  }

  bool get_ulonglong(ulonglong *var,
                     const char *av,
                     const char *name,
                     size_t length)
  {
    if (strncmp(av, name, length) || av[length] != '=')
      return true;
    m_error|= get_ulonglong(var, name, av + length + 1);
    return false;
  }
public:
  Options(int ac, char **av)
   :m_progname(av[0]),
    m_used_options(0),
    m_verbose(0),
    m_error(0),
    m_count(10*1000*1000ULL)
  {
    shift(&ac, &av);
    m_used_options++;
    for ( ; ac > 0; shift(&ac, &av))
    {
      if (!get_one_option(av[0]))
        m_used_options++;
      else
        break;
    }
  }
  bool get_one_option(const char *av)
  {
    if (!strncmp(av, "-?", 2))             m_error= true;
    else if (!strncmp(av, "--verbose=", 10)) m_verbose= atoi(av+10);
    else if (!strncmp(av, "--verbose", 9)) m_verbose= 1;
    else if (!strncmp(av, "--help", 6))    m_error= true;
    else if (!strncmp(av, "--count=", 8))
    {
      if (get_ulonglong_size(&m_count, "--count", av + 8))
        m_error= true;
    }
    else
    {
      if (!strncmp(av, "--", 2))
        m_error= true; // Unknown option
      return true;     // Not an option - normal arguments started
    }
    return false;
  }
  uint used_options() const { return m_used_options; }
  bool error() const { return m_error; }
  uint verbose() const { return m_verbose; }
  ulonglong count() const { return m_count; }
  void usage() const
  {
    printf("Usage:\n%s [--verbose[=val]] [--count=val] [test ...]\n", m_progname);
  }
};

#endif
