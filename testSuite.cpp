// #include "boost/test/included/unit_test.hpp"
//#include <boost/1.78.0_1/include/boost/test/included/unit_test.hpp>

#include </opt/homebrew/Cellar/boost/1.78.0_1/include/boost/test/included>
// #include <boost/lambda/lambda.hpp>
#include "speechAssist.h"
#include <string>
using namespace std;
#define BOOST_TEST_MODULE MorseCodeTest


BOOST_AUTO_TEST_SUITE(s1)
// Note - method calls other method (that has its own seperate branch 
// testing) that validates RGB values. RGB names are not validated b/c
// user decides what speech to text will recognize as the color name.

BOOST_AUTO_TEST_CASE(test1) { // doesn't enter if
    MorseCodeMod mcm;
    string invalidRGB = "FFFFFF1";
    string rgbName = "White";
    BOOST_CHECK(false == mcm.addClrRecog(invalidRGB, rgbName));
}

BOOST_AUTO_TEST_CASE(test2) { // enters if
    MorseCodeMod mcm;
    string validRGB = "FFFFFF";
    string rgbName = "White";
    BOOST_CHECK(false == mcm.addClrRecog(validRGB, rgbName));
}

BOOST_AUTO_TEST_SUITE_END(s1)


// FIRST ASSIGNMENT 
BOOST_AUTO_TEST_CASE(my_boost_test) {
    MorseCodeMod mcm;
    string invalidRGB = "FF00FF2";
    BOOST_CHECK(false == mcm.isValidColor(invalidRGB));
}

BOOST_AUTO_TEST_CASE(my_boost_test) {
    MorseCodeMod mcm;
    string validRGB = "FF0000";
    BOOST_CHECK(true == mcm.isValidColor(validRGB));
}







