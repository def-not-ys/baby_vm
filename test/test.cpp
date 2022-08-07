#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hh"

extern "C" {
    #include "../src/baby_vm.h"
}

/*
  test plan:
  3. label test
     set label for something (maybe a loop)
     test rv == x
     return error status == status ok
 */

TEST_CASE( "Normal startup/shutdown flow", "[baby_vm]" ) {

    Arguments args = {0};
    char* test_file = "tests/normal-flow.txt";
    int expected = 0;

    int16_t rv = process(&args, &test_file);
    REQUIRE( rv == expected );
}

TEST_CASE( "Set return value flow", "[baby_vm]" ) {

    Arguments args = {0};
    char* test_file = "tests/set-rv.txt";
    int expected = 102;

    int16_t rv = process(&args, &test_file);
    REQUIRE( rv == expected );
}
