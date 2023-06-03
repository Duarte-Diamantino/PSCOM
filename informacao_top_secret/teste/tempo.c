#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>

int main()
{
  time_t t;
  struct tm tm;
  struct timespec ts;
  char time [50];

  clock_gettime(CLOCK_REALTIME, &ts);

  t = ts.tv_sec;
  memset(&tm, 0, sizeof(struct tm));
  localtime_r (&t, &tm);
  strftime(time, sizeof(time), "%d/%m/%Y %H:%M:%S", &tm);
  printf ("Data:%s %ld \n", time, ts.tv_sec);
  
  return 0;
}
