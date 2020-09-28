#ifndef TIMER_H
#define TIMER_H

#ifdef WIN32

#include <Windows.h>
struct timezone {
    int tz_minuteswest;
    int tz_dsttime;
};

static inline int gettimeofday(struct timeval *tv, struct timezone *tz)
{
    if (tv) {
        FILETIME               filetime; /* 64-bit value representing the number of 100-nanosecond intervals since January 1, 1601 00:00 UTC */
        ULARGE_INTEGER         x;
        ULONGLONG              usec;
        static const ULONGLONG epoch_offset_us = 11644473600000000ULL; /* microseconds betweeen Jan 1,1601 and Jan 1,1970 */

#if _WIN32_WINNT >= _WIN32_WINNT_WIN8
        GetSystemTimePreciseAsFileTime(&filetime);
#else
        GetSystemTimeAsFileTime(&filetime);
#endif
        x.LowPart =  filetime.dwLowDateTime;
        x.HighPart = filetime.dwHighDateTime;
        usec = x.QuadPart / 10  -  epoch_offset_us;
        tv->tv_sec  = (time_t)(usec / 1000000ULL);
        tv->tv_usec = (long)(usec % 1000000ULL);
    }
    if (tz) {
        TIME_ZONE_INFORMATION timezone;
        GetTimeZoneInformation(&timezone);
        tz->tz_minuteswest = timezone.Bias;
        tz->tz_dsttime = 0;
    }
    return 0;
}

#else
#include <sys/time.h>
#endif


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
