/*
BELOW IS THE TESTING SUITE - googleTestSuite.cpp
- Includes code and (a lot of) comments from Testing Assignment &&
- Additional tests added after Testing Assignment
*/

#include <iostream>
#include <gtest/gtest.h>
#include "speechAssist.h"
#include <string>
using namespace std;

/*
COMPILE NOTE: make sure to link all .cpp files that the units are in

g++ -std=c++0x googleTestSuite.cpp speechAssist.cpp mcTranslator.cpp -lgtest 
-lgtest_main -pthread -o googleTestSuite
*/

/*  
White Box Test: Test isValidColor() (Completed, has code) from 
the MorseCodeMode class 
The 'isValidColor() White Box Test' 1-5 achieves branch coverage
*/

/* 
isValidColor() Code:

// If invalid, returns "". If valid, returns valid *in correct,
// upper-case format*
string MorseCodeMod::isValidColor(string const& rgbInHex) {
	string validRGB  = rgbInHex;
	int length = rgbInHex.length();
	if (length != 6) {
		return ""; // "" means not a valid RGB color 
	}
	for (int i = 0; i < length; i++) {
		if (isdigit(validRGB[i])) {
			continue;
		} else if (isalpha(validRGB[i])) {
			validRGB[i] = toupper(validRGB[i]);
			// if not inbetween A to F
			if (!((validRGB[i] >= 'A') && (validRGB[i] <= 'F'))) {
				return "";
			}
		} else { // a special character was found, invalid
			return "";
		}
	}
	return validRGB;
}
*/

// isValidColor() White Box Test 1
TEST(colorTestSuite, validColor) { // (testSuiteName, testCaseName)
	string validRGB = "FF0000";
	EXPECT_EQ(MorseCodeMod::isValidColor(validRGB), "FF0000");
}

// isValidColor() White Box Test 2
TEST(colorTestSuite, isNotValidColor) {
	string invalidRGB = "FF00001";
	EXPECT_EQ(MorseCodeMod::isValidColor(invalidRGB), "");
}

// isValidColor() White Box Test 3
TEST(colorTestSuite, validLowerCaseColor) {
	string lowerValidRGB = "ff00ff"; // magenta
	EXPECT_EQ(MorseCodeMod::isValidColor(lowerValidRGB), "FF00FF");
}

// isValidColor() White Box Test 4
TEST(colorTestSuite, inValidColorSpecialChar) {
	string invalidRGBSpecialChar = "FF00!0";
	EXPECT_EQ(MorseCodeMod::isValidColor(invalidRGBSpecialChar), "");
}

// isValidColor() White Box Test 5
TEST(colorTestSuite, invalidColorOOBChar) { // out of bounds char (not A-F)
	string invalidRGGoobChar = "FF000G";
	EXPECT_EQ(MorseCodeMod::isValidColor(invalidRGGoobChar), "");
}



/*  Black Box Test: Test colorInText() in Speech Class

-- NOTE (some context the black box tester/grader would need/use): 
The Speech class is currently a work in progress, and the method colorInText() 
is currently set to return true by default, so the google testing terminal 
outputs will not be as intended until further project development
(i.e. there will have some failure(s)).

As for creating the black box tests, all is known is that colorInText()'s
purpose is to check if the passed string (which due to the class's design is 
guaranteed to be a lowercase sentence that is not an empty string) contains 
"in ..." at the *end* of the string and ... is a color in the following global 
map in the program:

map<string, string> voiceToColors = { // map vals at start of program
	{"white", "FFFFFF"}, {"red", "FF0000"}, {"green", "008000"},
	{"blue", "0000FF"}, {"purple", "800080"}, {"yellow", "FFFF00"},
	{"pink", "FFC0CB"}, {"brown", "A52A2A"}
};

Meaning,
"sos help me in white" should return true
"sos help me in FFFFFF" should return false
"hello world in navy blue" should return false (left edgecase)
"hello world in blue hello" should return false (right edgecase)
"hello world" should return false 
"hello world in" should return false
*/

// acceptance test (colorInText 1)
TEST(voicedColorsDefaultMap, whiteValid) {
	string validVoicedColor = "sos help me in white";
	Speech* s = new Speech("WIP");
	EXPECT_EQ(s->colorInText(validVoicedColor), true);
	delete(s);
}

// acceptance test (colorInText 2)
TEST(voicedColorsDefaultMap, rgbInvalid) {
	string invalidVoicedRGB = "sos help me in FFFFFF";
	Speech* s = new Speech("WIP");
	EXPECT_EQ(s->colorInText(invalidVoicedRGB), false);
	delete(s);
}

// acceptance test (colorInText 3)
TEST(voicedColorsDefaultMap, navyBlueInvalid) {
	string navyBlueInvalid = "hello world in navy blue";
	Speech* s = new Speech("WIP");
	EXPECT_EQ(s->colorInText(navyBlueInvalid), false);
	delete(s);
}

// acceptance test (colorInText 4)
TEST(voicedColorsDefaultMap, colorNotAtEndInvalid) {
	string invalidMiddle = "hello world in blue hello";
	Speech* s = new Speech("WIP");
	EXPECT_EQ(s->colorInText(invalidMiddle), false);
	delete(s);
}

// acceptance test (colorInText 5)
TEST(voicedColorsDefaultMap, noColorSpecifiedAnywhere) {
	// user would want default color 
	string noColor = "hello world";
	Speech* s = new Speech("WIP");
	EXPECT_EQ(s->colorInText(noColor), false);
	delete(s);
}

// acceptance test (colorInText 6)
TEST(voicedColorsDefaultMap, inIsLastWord) {
	// would treat in as part of the sentence, so no color specification 
	string inLast = "hello world in";
	Speech* s = new Speech("WIP");
	EXPECT_EQ(s->colorInText(inLast), false);
	delete(s);
}


/*
-- REDO PORTION OF THE ASSIGNMENT -- 

Integration test - Top-Down Approach

This integration test will be a top down approach, as the Speech class' addSTTColor()
method and the Speech class itself are still a work in progress, while the Message 
class has finished development.

Unit 1: MorseCodeMod class
Unit 2: addSTTColor() from the Speech class 

- Worked correctly if colorInText() recognizes turquoise in the text
	(and returns true) 
- this will show the Message class is working correctly in that it decrypted the
morse code string correctly, and that decrypted message can be recognized as a 
colorInText command using the Speech class *after* the Speech class' 
addSTTColor() method adds the turquoise color for speech recognition 

NOTE: Speech's colorInText() method is WIP and is currently set to return true
always, so this test will currently always pass
*/   

TEST(TestVoiceCommandWithMessage, helloWorldInTurquoise) {
	// "hello world in turquoise" in MC
	string helloWorldInTurqMC = ".... . .-.. .-.. --- / .-- --- .-. .-.. -.. " 
								"/ .. -. / - ..- .-. --.- ..- --- .. ... .";
	Message* m = new Message(helloWorldInTurqMC, true); 
	string voiceCommand = m->getDecrypted();
	Speech::addSTTColor("turquoise", "30D5C8");
	Speech* s = new Speech("WIP");
	EXPECT_EQ(s->colorInText(voiceCommand), true);
	delete(s);
	delete(m);
} 


/* MORE TESTS (Not apart of assignment) */



int main(int argc, char **argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}










