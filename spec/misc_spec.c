
//
// specifying gajeta
//
// Mon Aug 25 11:41:45 JST 2014
//

#include <strings.h>

#include "gajeta.h"


context "misc"
{
  describe "fgaj_normalize_level()"
  {
    it "turns the input into a numeric level"
    {
      ensure(fgaj_normalize_level('t') == 10);
      ensure(fgaj_normalize_level('d') == 20);
      ensure(fgaj_normalize_level('i') == 30);
      ensure(fgaj_normalize_level('w') == 40);
      ensure(fgaj_normalize_level('e') == 50);
    }

    it "leaves numeric levels as is"
    {
      ensure(fgaj_normalize_level(10) == 10);
    }
  }

  describe "fgaj_level_to_string()"
  {
    it "turns a char level into a string"
    {
      ensure(fgaj_level_to_string('t') === "TRACE");
      ensure(fgaj_level_to_string('d') === "DEBUG");
      ensure(fgaj_level_to_string('i') === "INFO");
      ensure(fgaj_level_to_string('w') === "WARN");
      ensure(fgaj_level_to_string('e') === "ERROR");

      ensure(fgaj_level_to_string(10) === "TRACE");
      ensure(fgaj_level_to_string(20) === "DEBUG");
      ensure(fgaj_level_to_string(30) === "INFO");
      ensure(fgaj_level_to_string(40) === "WARN");
      ensure(fgaj_level_to_string(50) === "ERROR");

      ensure(fgaj_level_to_string(15) ===f "15");
    }
  }

  describe "fgaj_now()"
  {
    it "returns the current time detailed in a string"
    {
      char *s = fgaj_now();

      ensure(index(s, '-') == s + 4);
      ensure(index(s, ':') == s + 13);
      ensure(index(s, '.') == s + 19);

      free(s);
    }
  }
}

