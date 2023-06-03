#define _XOPEN_SOURCE
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>


//       char *asctime(const struct tm *tm);
//       char *asctime_r(const struct tm *tm, char *buf);
//
//       char *ctime(const time_t *timep);
//       char *ctime_r(const time_t *timep, char *buf);
//
//       struct tm *gmtime(const time_t *timep);
//       struct tm *gmtime_r(const time_t *timep, struct tm *result);
//
//       struct tm *localtime(const time_t *timep);
//       struct tm *localtime_r(const time_t *timep, struct tm *result);
//
//       time_t mktime(struct tm *tm);
//
//
//              struct tm {
//                      int     tm_sec;         /* seconds */
//                      int     tm_min;         /* minutes */
//                      int     tm_hour;        /* hours */
//                      int     tm_mday;        /* day of the month */
//                      int     tm_mon;         /* month */
//                      int     tm_year;        /* year */
//                      int     tm_wday;        /* day of the week */
//                      int     tm_yday;        /* day in the year */
//                      int     tm_isdst;       /* daylight saving time */
//              };
//
//
//       size_t strftime(char *s, size_t max, const char *format,
//                           const struct tm *tm);
//
//       char *strptime(const char *s, const char *format, struct tm *tm);


int main()
{
  time_t t;
  struct timeval tv;
  struct tm tm;
  char str[26];

  time(&t);
  gettimeofday(&tv, NULL);

  printf("time returns:          %ld s\n", t);
  printf("gettimeofday returns:  %ld s and %ld us\n", tv.tv_sec, tv.tv_usec);

  localtime_r(&t, &tm);
  asctime_r(&tm, &str[0]);

  printf("ctime(t): %s\n", ctime(&t));
  printf("tm: dd=%d, mm=%d, yyyy=%d hh=%d mm=%d ss=%d\n", 
	 tm.tm_mday, tm.tm_mon, tm.tm_year, tm.tm_hour, tm.tm_min, tm.tm_sec);
  printf("str: %s\n", str);

  strftime(&str[0], sizeof(str), "%d/%m/%Y %H:%M:%S\n", &tm);
  printf("str: %s\n", str);

  printf("data/hora (dd/mm/yyyy hh:mm:ss):");
  fgets(&str[0], sizeof(str), stdin);

  memset(&tm, 0, sizeof(struct tm));
  strptime(str, "%d/%m/%Y %H:%M:%S\n", &tm);
  printf("tm: dd=%d, mm=%d, yyyy=%d hh=%d mm=%d ss=%d\n", 
	 tm.tm_mday, tm.tm_mon, tm.tm_year, tm.tm_hour, tm.tm_min, tm.tm_sec);
  //tm.tm_isdst = 0;
  tm.tm_isdst = 1;

  t = mktime(&tm);
  if (t == -1) perror("error mktime:\n");
  printf("ctime(t): %s\n", ctime(&t));

  asctime_r(&tm, &str[0]);
  
  printf("str: %s\n", str);


  return 0;
}
