
//
// specifying gajeta
//
// Mon Aug 25 10:47:09 JST 2014
//

#include "gajeta.h"


context "logging"
{
  before each
  {
    fgaj_conf_get()->logger = fgaj_string_logger;
    fgaj_conf_get()->level = 10;
    fgaj_conf_get()->params = NULL;
  }
  after each
  {
    fgaj_conf_reset();
  }

  describe "fgaj_log()"
  {
    it "logs"
    {
      fgaj_log('d', "flon.nada", "all green");

      ensure(fgaj_conf_get()->params ===f "*** DEBUG flon.nada all green");

      fgaj_log('i', "flon.nada", "all green");

      ensure(fgaj_conf_get()->params ===f "*** INFO flon.nada all green");
    }

    it "doesn't log if level < log level"
    {
      fgaj_conf_get()->level = 30;

      fgaj_log('d', "flon.nada", "all green");

      ensure(fgaj_conf_get()->params == NULL);

      fgaj_log('i', "flon.nada", "all green");

      ensure(fgaj_conf_get()->params ===f "*** INFO flon.nada all green");
    }
  }
}

describe "fgaj_color_stdout_logger()"
{
  it "logs"
  {
    fgaj_conf_get()->level = 10;

    printf("---8<---\n");
    fgaj_log('t', "flon.nada", "performed an addition");
    fgaj_log('d', "flon.nada", "all green");
    fgaj_log('i', "flon.nada", "all green");
    fgaj_log('e', "flon.nada", "error somewhere");
    fgaj_log('w', "flon.nada", "something went badly wrong");
    fgaj_log(22, "flon.nada", "custom log level");
    printf("--->8---\n");

    ensure(1 == 1);
  }
}

