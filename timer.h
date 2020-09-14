#ifndef TIMER_H
#define TIMER_H

#include <sys/time.h>

class Timer: public timeval
{
public:
  Timer()
  {
    gettimeofday(this, NULL);
  }
  long long to_usec() const
  {
    //printf("sec=%lld usec=%lld\n", (long long) tv_sec, (long long) tv_usec);
    return tv_sec * 1000000ULL + tv_usec;
  }
  double diff(const Timer &start)
  {
    return (to_usec() - start.to_usec()) / 1000000e0;
  }
};

#endif
