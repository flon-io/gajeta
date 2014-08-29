
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
#include <errno.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#include "flutil.h"
#include "gajeta.h"


//
// conf

fgaj_conf *fgaj__conf = NULL;

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

  // color or not?

  s = fgaj_getenv("FLON_LOG_COLOR", "FGAJ_COLOR");

  fgaj__conf->color = s == NULL ? 't' : s[0];

  // utc or not?

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
  if (level == 'w') return 40;
  if (level == 'e' || level == 'r') return 50;
  return level;
}

char *fgaj_level_to_string(char level)
{
  level = fgaj_normalize_level(level);

  if (level == 10) return "TRACE";
  if (level == 20) return "DEBUG";
  if (level == 30) return "INFO";
  if (level == 40) return "WARN";
  if (level == 50) return "ERROR";
  return flu_sprintf("%d", level);
}

void fgaj_level_string_free(char *s)
{
  if (s[0] >= '0' && s[0] <= '9') free(s);
}

//
// loggers

static short fgaj_color()
{
  if (fgaj__conf->color == 'T') return 1;
  if (fgaj__conf->color == 'f') return 0;
  return isatty(1);
}

static char *fgaj_red() { return fgaj_color() ? "[31m" : ""; }
static char *fgaj_green() { return fgaj_color() ? "[32m" : ""; }
static char *fgaj_yellow() { return fgaj_color() ? "[33m" : ""; }
static char *fgaj_blue() { return fgaj_color() ? "[34m" : ""; }
//static char *fgaj_magenta() { return fgaj_color() ? "[35m" : ""; }
static char *fgaj_cyan() { return fgaj_color() ? "[36m" : ""; }
static char *fgaj_white() { return fgaj_color() ? "[37m" : ""; }
static char *fgaj_clear() { return fgaj_color() ? "[0m" : ""; }

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

  char *lcolor = NULL;
  if (level >= 50) lcolor = fgaj_red();          // error
  else if (level >= 40) lcolor = fgaj_yellow();  // warn
  else if (level >= 30) lcolor = fgaj_white();   // info
  else if (level >= 20) lcolor = fgaj_cyan();    // debug
  else if (level >= 10) lcolor = fgaj_green();   // trace
  else lcolor = fgaj_blue();                     // ...

  char *lstr = fgaj_level_to_string(level);

  printf(
    "%s%s %s%s %s%d/%d %s%5s %s%s %s%s%s\n",
    fgaj_yellow(), now,
    fgaj_white(), fgaj__conf->host,
    fgaj_yellow(), getppid(), getpid(),
    lcolor, lstr,
    fgaj_green(), pref,
    fgaj_white(), msg,
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
  char level,
  const char *file, int line, const char *func,
  const char *format, va_list ap, short err)
{
  fgaj_init();

  if (fgaj__conf->logger == NULL) return;

  level = fgaj_normalize_level(level);
  if (level < fgaj__conf->level && level <= 50) return;

  flu_sbuffer *b = NULL;

  b = flu_sbuffer_malloc();
  flu_sbputs(b, file);
  if (line > -1)
  {
    flu_sbprintf(b, ":%d", line);
    if (func != NULL) flu_sbprintf(b, ":%s", func);
  }
  //
  char *subject = flu_sbuffer_to_string(b);

  b = flu_sbuffer_malloc();
  flu_sbvprintf(b, format, ap);
  if (err) flu_sbprintf(b, ": %s", strerror(errno));
  //
  char *msg = flu_sbuffer_to_string(b);

  fgaj__conf->logger(level, subject, msg);

  free(subject);
  free(msg);
}

void fgaj_log(
  char level,
  const char *file, int line, const char *func,
  const char *format, ...)
{
  va_list ap; va_start(ap, format);
  fgaj_do_log(level, file, line, func, format, ap, tolower(level) == 'r');
  va_end(ap);
}

