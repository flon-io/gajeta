
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
    //flu_list *l = NULL;
  }
  after each
  {
    //if (l != NULL) flu_list_free(l);
  }

  describe "fgaj_log()"
  {
    it "logs"
    {
      fgaj_log('d', "nada", "hello");

      ensure(1 == 1);
    }
  }
}

