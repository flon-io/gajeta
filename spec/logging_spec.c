
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
    fgaj_set_logger(fgaj_string_logger);
    fgaj_set_level(10);
    fgaj_set_params(NULL);
  }
  after each
  {
    fgaj_set_logger(NULL);
  }

  describe "fgaj_log()"
  {
    it "logs"
    {
      fgaj_log('d', "flon.nada", "all green");

      ensure(fgaj_get_params() ===f "*** DEBUG flon.nada all green");

      fgaj_log('i', "flon.nada", "all green");

      ensure(fgaj_get_params() ===f "*** INFO flon.nada all green");
    }

    it "doesn't log if level < log level"
    {
      fgaj_set_level(30);

      fgaj_log('d', "flon.nada", "all green");

      ensure(fgaj_get_params() == NULL);

      fgaj_log('i', "flon.nada", "all green");

      ensure(fgaj_get_params() ===f "*** INFO flon.nada all green");
    }
  }
}

describe "fgaj_color_stdout_logger()"
{
  it "logs"
  {
    fgaj_set_level(10);

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

