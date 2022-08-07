#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hh"

extern "C" {
    #include "../src/baby_vm.h"
}

/*
  test plan:
  1. normal flow
     rv == 0
     return error status == status ok
  2. rv test
     set rv to x
     assert rv == x
     return error status == status ok
  3. label test
     set label for something (maybe a loop)
     test rv == x
     return error status == status ok
 */

TEST_CASE( "Normal startup/shutdown flow", "[baby_vm]" ) {

    Arguments args = {0};
    char* test_file = "tests/normal-flow.txt";

    int16_t rv = process(&args, &test_file);
    REQUIRE( rv == 0 );
}
