
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

#include <stdarg.h>
#include <stdlib.h>

#include "flutil.h"
#include "gajeta.h"


//
// global

fgaj_logger *fgaj__logger = NULL;
char fgaj__level = 10;

//
// misc functions

char fgaj_normalize_level(char level)
{
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
  return "???";
}

void fgaj_set_logger(fgaj_logger *l)
{
  fgaj__logger = l;
}

void fgaj_set_level(char level)
{
  fgaj__level = fgaj_normalize_level(level);
}

//
// loggers

//void rdz_red() { if (isatty(1)) printf("[31m"); }
//void rdz_green() { if (isatty(1)) printf("[32m"); }
//void rdz_yellow() { if (isatty(1)) printf("[33m"); }
////void rdz_blue() { if (isatty(1)) printf("[34m"); }
////void rdz_magenta() { if (isatty(1)) printf("[35m"); }
//void rdz_cyan() { if (isatty(1)) printf("[36m"); }
////void rdz_white() { if (isatty(1)) printf("[37m"); }
//void rdz_clear() { if (isatty(1)) printf("[0m"); }

void *fgaj_color_stdout_logger(char level, const char *pref, const char *msg)
{
  // TODO: date
  // TODO: colour

  printf("*** %s %s %s\n", fgaj_level_to_string(level), pref, msg);
  return NULL;
}

void *fgaj_string_logger(char level, const char *pref, const char *msg)
{
  return flu_sprintf("*** %s %s %s", fgaj_level_to_string(level), pref, msg);
}


//
// logging functions

void fgaj_init()
{
  if (fgaj__logger != NULL) return;

  fgaj__logger = fgaj_color_stdout_logger;
  fgaj__level = 10;
}

static void *fgaj_do_log(
  char level, const char *pref, const char *format, va_list ap)
{
  level = fgaj_normalize_level(level);
  if (level < fgaj__level && level <= 50) return;

  char *msg = flu_svprintf(format, ap);

  fgaj_init();

  void *r = fgaj__logger(level, pref, msg);

  free(msg);

  return r;
}

void *fgaj_log(char level, const char *pref, const char *format, ...)
{
  va_list ap; va_start(ap, format);
  void *r = fgaj_do_log(level, pref, format, ap);
  va_end(ap);

  return r;
}

