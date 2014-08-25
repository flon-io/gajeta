
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
      char *string = NULL;

      string = (char *)fgaj_log('d', "flon.nada", "all green");

      ensure(string ===f "*** DEBUG flon.nada all green");

      string = (char *)fgaj_log('i', "flon.nada", "all green");

      ensure(string ===f "*** INFO flon.nada all green");
    }
  }
}

