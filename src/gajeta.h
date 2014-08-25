
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

// gajeta.h

#ifndef FLON_GAJETA_H
#define FLON_GAJETA_H

#include <stdio.h>
#include <stdarg.h>


#define GAJ_VERSION "1.0.0"

// 10 't' trace
// 20 'd' debug
// 30 'i' info
// 40 'e' error
// 50 'w' warn

typedef void fgaj_log_func(void *params, char *s);

typedef struct fgaj_logger {
  char level;
  char *name;
  fgaj_log_func *log_func;
  void *params;
} fgaj_logger;

fgaj_logger *fgaj_stdout_logger_malloc();
fgaj_logger *fgaj_file_logger_malloc(FILE *f);
fgaj_logger *fgaj_child_logger_malloc(fgaj_logger *parent, char *name);

void fgaj_logger_free(fgaj_logger *l);

void fgaj_log(fgaj_logger *l, char level, const char *format, ...);
void fgaj_l(fgaj_logger *l, char level, const char *format, va_list ap);

void fgaj_t(fgaj_logger *l, const char *format, ...);
void fgaj_d(fgaj_logger *l, const char *format, ...);
void fgaj_i(fgaj_logger *l, const char *format, ...);
void fgaj_e(fgaj_logger *l, const char *format, ...);
void fgaj_w(fgaj_logger *l, const char *format, ...);

#endif // FLON_GAJETA_H

