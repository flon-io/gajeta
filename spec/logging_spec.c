
//
// specifying gajeta
//
// Mon Aug 25 10:47:09 JST 2014
//

#include <errno.h>

#include "gajeta.h"


context "logging"
{
  before each
  {
    fgaj_conf_get()->logger = fgaj_string_logger;
    fgaj_conf_get()->level = 10;
    fgaj_conf_get()->out = NULL;
    fgaj_conf_get()->params = NULL;
  }
  after each
  {
    fgaj_conf_reset();
  }

  describe "fgaj_ll()"
  {
    it "logs"
    {
      fgaj_ll('d', "flon.nada", "all green");

      ensure(fgaj_conf_get()->out ===f "*** DEBUG flon.nada all green");

      fgaj_ll('i', "flon.nada", "all green");

      ensure(fgaj_conf_get()->out ===f "*** INFO flon.nada all green");
    }

    it "doesn't log if level < log level"
    {
      fgaj_conf_get()->level = 30;

      fgaj_ll('d', "flon.nada", "all green");

      ensure(fgaj_conf_get()->out == NULL);

      fgaj_ll('i', "flon.nada", "all green");

      ensure(fgaj_conf_get()->out ===f "*** INFO flon.nada all green");
    }
  }

  describe "fgaj_r()"
  {
    after each
    {
      errno = 0;
    }

    it "appends the strerror() for the current errno"
    {
      errno = 1;
      fgaj_ll('r', "flon.testing", "error while reading %s", "book");

      ensure(fgaj_conf_get()->out ===f ""
        "*** ERROR flon.testing error while reading book"
        ": Operation not permitted");
    }

    it "is OK if errno is 0 or negative"
    {
      errno = 0;
      fgaj_ll('r', "flon.testing", "zero");

      ensure(fgaj_conf_get()->out ===f ""
        "*** ERROR flon.testing zero: Success");

      errno = -1;
      fgaj_ll('r', "flon.testing", "nega");

      ((char *)fgaj_conf_get()->out)[43] = '\0';

      ensure(fgaj_conf_get()->out ===f ""
        "*** ERROR flon.testing nega: Unknown error ");
    }
  }

  context "when fgaj_conf()->logger is null"
  {
    it "doesn't log"
    {
      fgaj_conf_get()->logger = NULL;
      fgaj_conf_get()->out = NULL;
      fgaj_conf_get()->params = NULL;

      fgaj_ll('w', "flon.nada", "all green");

      ensure(fgaj_conf_get()->out == NULL);
    }
  }
}

describe "fgaj_color_file_logger()"
{
  it "logs"
  {
    fgaj_conf_get()->level = 5;

    printf("---8<---\n");
    fgaj_t("performed an addition %i %s", 1, "nada");
    fgaj_t("performed an addition");
    fgaj_d("all green");
    fgaj_i("all green");
    fgaj_w("something is going wrong");
    fgaj_e("error somewhere");
    fgaj_r("error");
    printf("\n");
    fgaj_l(7, "custom low log level");
    fgaj_l(22, "custom log level");
    printf("\n");
    fgaj_ll('e', "flon.testing", "custom subject");
    printf("--->8---\n");

    ensure(1 == 1);
  }
}

describe "fgaj_grey_logger()"
{
  it "logs"
  {
    fgaj_conf_get()->logger = fgaj_grey_logger;
    fgaj_conf_get()->level = 5;

    printf("---8<---\n");
    fgaj_t("performed a substraction %i %s", 1, "nada");
    fgaj_d("all grey");
    fgaj_i("all grey");
    printf("--->8---\n");

    ensure(1 == 1);
  }

  it "accepts a flu_dict in fgaj_conf_get()->params"
  {
    fgaj_conf_get()->logger = fgaj_grey_logger;
    fgaj_conf_get()->level = 5;
    fgaj_conf_get()->params = flu_d("indent", 2, NULL);

    printf("---8<---\n");
    fgaj_t("performed a substraction %i %s", 1, "nada");
    fgaj_d("all grey");
    fgaj_i("all grey");
    printf("--->8---\n");

    ensure(1 == 1);
  }
}

