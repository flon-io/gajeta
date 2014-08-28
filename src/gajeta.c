
//
// Copyright (c) 2013-2014, John Mettraux, jmettraux+flon@gmail.com
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// Made in Japan.
//

#define _POSIX_C_SOURCE 200809L

#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#include "flutil.h"
#include "gajeta.h"


//
// global

//typedef struct fgaj_conf {
//  fgaj_logger *logger;
//  char level;
//  short utc;
//  char *host;
//  void *params;
//} fgaj_conf;

fgaj_conf *fgaj__conf = NULL;

// // short fgaj__ready = 0;
// // fgaj_logger *fgaj__logger = NULL;
// // char fgaj__level = 10;
// // short fgaj__utc = 0;
// // char *fgaj__host = NULL;
// // void *fgaj__params = NULL;

static char *fgaj_getenv(const char *k0, const char *k1)
{
  char *s = getenv(k0);
  if (s == NULL) s = getenv(k1);

  return s;
}

static void fgaj_init()
{
  if (fgaj__conf) return;

  fgaj__conf = calloc(1, sizeof(fgaj_conf));

  char *s = NULL;

  // utc or not ?

  s = fgaj_getenv("FLON_LOG_UTC", "FGAJ_UTC");

  fgaj__conf->utc = (s != NULL && (s[0] == '1' || tolower(s[0]) == 't'));

  // determine host

  fgaj__conf->host = fgaj_getenv("FLON_LOG_HOST", "FGAJ_HOST");

  if (fgaj__conf->host == NULL)
  {
    char *h = calloc(16, sizeof(char));
    gethostname(h, 16);
    fgaj__conf->host = h;
  }

  // determine level

  fgaj__conf->level = 30;

  s = fgaj_getenv("FLON_LOG_LEVEL", "FGAJ_LEVEL");
  //
  if (s != NULL)
  {
    if (s[0] > '0' && s[1] < '9') fgaj__conf->level = atoi(s);
    else fgaj__conf->level = fgaj_normalize_level(s[0]);
  }
  //printf("level: %i\n", fgaj__level);

  // determine logger

  fgaj__conf->logger = fgaj_color_stdout_logger;
}

fgaj_conf *fgaj_conf_get()
{
  if (fgaj__conf == NULL) fgaj_init();
  return fgaj__conf;
}

void fgaj_conf_reset()
{
  if (fgaj__conf == NULL) return;
  if (fgaj__conf->host) free(fgaj__conf->host);
  free(fgaj__conf);
  fgaj__conf = NULL;
}

//
// misc functions

char fgaj_normalize_level(char level)
{
  if (level >= 'A' && level <= 'Z') level = tolower(level);

  if (level == 't') return 10;
  if (level == 'd') return 20;
  if (level == 'i') return 30;
  if (level == 'e') return 40;
  if (level == 'w') return 50;
  return level;
}

char *fgaj_level_to_string(char level)
{
  level = fgaj_normalize_level(level);

  if (level == 10) return "TRACE";
  if (level == 20) return "DEBUG";
  if (level == 30) return "INFO";
  if (level == 40) return "ERROR";
  if (level == 50) return "WARN";
  return flu_sprintf("%d", level);
}

void fgaj_level_string_free(char *s)
{
  if (s[0] >= '0' && s[0] <= '9') free(s);
}

//
// loggers

static char *fgaj_red() { return isatty(1) ? "[31m" : ""; }
static char *fgaj_green() { return isatty(1) ? "[32m" : ""; }
static char *fgaj_yellow() { return isatty(1) ? "[33m" : ""; }
static char *fgaj_blue() { return isatty(1) ? "[34m" : ""; }
//static char *fgaj_magenta() { return isatty(1) ? "[35m" : ""; }
//static char *fgaj_cyan() { return isatty(1) ? "[36m" : ""; }
static char *fgaj_white() { return isatty(1) ? "[37m" : ""; }
static char *fgaj_clear() { return isatty(1) ? "[0m" : ""; }

char *fgaj_now()
{
  fgaj_init();

  struct timeval tv;
  struct tm *tm;

  gettimeofday(&tv, NULL);
  tm = fgaj__conf->utc ? gmtime(&tv.tv_sec) : localtime(&tv.tv_sec);

  char *s = calloc(33, sizeof(char));
  strftime(s, 33, "%F %T.000000 %z", tm);
  snprintf(s + 20, 7, "%06ld", tv.tv_usec);
  s[26] = ' ';

  return s;
}

void fgaj_color_stdout_logger(char level, const char *pref, const char *msg)
{
  char *now = fgaj_now();

  char *lcolor = fgaj_clear();
  if (level >= 40) lcolor = fgaj_red();
  else if (level <= 20) lcolor = fgaj_blue();
  //
  char *lstr = fgaj_level_to_string(level);

  printf(
    "%s%s %s%s %s%d/%d %s%5s %s%s %s%s\n",
    fgaj_yellow(), now,
    fgaj_white(), fgaj__conf->host,
    fgaj_yellow(), getppid(), getpid(),
    lcolor, lstr,
    fgaj_green(), pref, msg,
    fgaj_clear()
  );

  free(now);
  fgaj_level_string_free(lstr);
}

void fgaj_string_logger(char level, const char *pref, const char *msg)
{
  char *l = fgaj_level_to_string(level);
  char *s = flu_sprintf("*** %s %s %s", l, pref, msg);
  fgaj_level_string_free(l);
  fgaj__conf->params = s;
}


//
// logging functions

static void fgaj_do_log(
  char level, const char *pref, const char *format, va_list ap)
{
  level = fgaj_normalize_level(level);
  if (level < fgaj__conf->level && level <= 50) return;

  fgaj_init();

  char *msg = flu_svprintf(format, ap);

  fgaj__conf->logger(level, pref, msg);

  free(msg);
}

void fgaj_log(char level, const char *pref, const char *format, ...)
{
  va_list ap; va_start(ap, format);
  fgaj_do_log(level, pref, format, ap);
  va_end(ap);
}

void fgaj_t(const char *pref, const char *format, ...)
{
  va_list ap; va_start(ap, format);
  fgaj_do_log('t', pref, format, ap);
  va_end(ap);
}

void fgaj_d(const char *pref, const char *format, ...)
{
  va_list ap; va_start(ap, format);
  fgaj_do_log('d', pref, format, ap);
  va_end(ap);
}

void fgaj_i(const char *pref, const char *format, ...)
{
  va_list ap; va_start(ap, format);
  fgaj_do_log('i', pref, format, ap);
  va_end(ap);
}

void fgaj_e(const char *pref, const char *format, ...)
{
  va_list ap; va_start(ap, format);
  fgaj_do_log('e', pref, format, ap);
  va_end(ap);
}

void fgaj_w(const char *pref, const char *format, ...)
{
  va_list ap; va_start(ap, format);
  fgaj_do_log('w', pref, format, ap);
  va_end(ap);
}

