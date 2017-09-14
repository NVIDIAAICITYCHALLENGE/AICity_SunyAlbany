#ifndef _MISC_H
#define _MISC_H

#include "misc.h"
#include <math.h>

#include "cliquer.h"

// YI:
// SYSTEMTIME, GetLocalTime not are WIN32 API, change to equivalent type in linux
// Definition in misc.h

#ifdef WIN32

int gettimeofday(struct timeval *tp, void *tzp) {
	time_t clock;
	struct tm vtm;
	SYSTEMTIME wtm;

	//GetLocalTime(&wtm);
	vtm.tm_year = wtm.wYear - 1900;
	vtm.tm_mon = wtm.wMonth - 1;
	vtm.tm_mday = wtm.wDay;
	vtm.tm_hour = wtm.wHour;
	vtm.tm_min = wtm.wMinute;
	vtm.tm_sec = wtm.wSecond;
	vtm. tm_isdst = -1;
	clock = mktime(&vtm);
	tp->tv_sec = (long)clock;
	tp->tv_usec = wtm.wMilliseconds * 1000;
	return (0);
}

#else

// https://stackoverflow.com/questions/3756323/getting-the-current-time-in-milliseconds
int gettimeofday(struct timeval *tp, void *tzp) {
	long            ms; // Milliseconds
    time_t          s;  // Seconds
    struct timespec spec;
    clock_gettime(CLOCK_REALTIME, &spec);
    s  = spec.tv_sec;
    ms = round(spec.tv_nsec / 1.0e6); // Convert nanoseconds to milliseconds
	tp->tv_sec = (long)s;
	tp->tv_usec = ms;
	return(0);
}

#endif

clock_t times (struct tms *__buffer) {

	__buffer->tms_utime = clock();
	__buffer->tms_stime = 0;
	__buffer->tms_cstime = 0;
	__buffer->tms_cutime = 0;
	return __buffer->tms_utime;
}

#endif



