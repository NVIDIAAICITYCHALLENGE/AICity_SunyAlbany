
#ifndef CLIQUER_MISC_H
#define CLIQUER_MISC_H

//#include <windows.h>
#include <time.h>
#include "cliquerconf.h"

/*
 * We #define boolean instead of using a typedef because nauty.h uses it
 * also.  AFAIK, there is no way to check for an existing typedef, and
 * re-typedefing is illegal (even when using exactly the same datatype!).
 */
#ifndef boolean
#define boolean int
#endif


/*
 * Default value for UNUSED_FUNCTION:  use "__attribute__((unused))" for
 * GCC versions that support it, otherwise leave blank.
 */
#ifndef UNUSED_FUNCTION
# if     __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ > 4)
#  define UNUSED_FUNCTION __attribute__((unused))
# else
#  define UNUSED_FUNCTION
# endif
#endif  /* !UNUSED_FUNCTION */


/*
 * Default inlining directive:  "inline"
 */
#ifndef INLINE
#define INLINE inline
#endif


#include <stdio.h>
#include <stdlib.h>

#ifndef ASSERT
#define ASSERT(expr) \
        if (!(expr)) { \
		fprintf(stderr,"cliquer file %s: line %d: assertion failed: " \
			"(%s)\n",__FILE__,__LINE__,#expr); \
		abort(); \
	}
#endif /* !ASSERT */


#ifndef FALSE
#define FALSE (0)
#endif
#ifndef TRUE
#define TRUE (!FALSE)
#endif


#ifndef MIN
#define MIN(a,b) (((a)<(b))?(a):(b))
#endif
#ifndef MAX
#define MAX(a,b) (((a)>(b))?(a):(b))
#endif
#ifndef ABS
#define ABS(v)  (((v)<0)?(-(v)):(v))
#endif


/*
* gettimeofday()
*
* Get the time of the day, which is an inner function in linux.
* Add by lywen, 2014.05.21
*/

struct tms {
	clock_t tms_utime;
	clock_t tms_stime;
	clock_t tms_cutime;
	clock_t tms_cstime;
};

int gettimeofday(struct timeval *tp, void *tzp);


clock_t times (struct tms *__buffer);

// YI: Add declaration of SYSTEMTIME
typedef struct _SYSTEMTIME {
		int wMilliseconds; 
        int wSecond;    /* Seconds (0-60) */
        int wMinute;    /* Minutes (0-59) */
        int wHour;   /* Hours (0-23) */
        int wDay;   /* Day of the month (1-31) */
		int wDayOfWeek;
        int wMonth;    /* Month (0-11) */
        int wYear;   /* Year - 1900 */
        int tm_wday;   /* Day of the week (0-6, Sunday = 0) */
        int tm_yday;   /* Day in the year (0-365, 1 Jan = 0) */
        int tm_isdst;  /* Daylight saving time */
} SYSTEMTIME;

#endif /* !CLIQUER_MISC_H */

