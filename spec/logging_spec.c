
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
      pending();
    }
  }
}

describe "fgaj_color_stdout_logger()"
{
  it "logs"
  {
    printf("---8<---\n");
    fgaj_log('i', "flon.nada", "all green");
    fgaj_log('d', "flon.nada", "all green");
    printf("--->8---\n");

    ensure(1 == 1);
  }
}

