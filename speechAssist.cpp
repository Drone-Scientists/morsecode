#include "speechAssist.h"
#include <iostream>
#include <map>
#include <algorithm>
#include <stdlib.h>
#include <stdexcept>
#include <array>
#include <ctype.h>
#include <string>
using namespace std;

map<string, string> voiceToColors = {
	{"white", "FFFFFF"}, {"red", "FF0000"}, {"green", "008000"},
	{"blue", "0000FF"}, {"purple", "800080"}, {"yellow", "FFFF00"},
	{"pink", "FFC0CB"}, {"brown", "A52A2A"}
};

// used when user specifies speed vocally in morse code command 
// 
map<string, int> voiceToSpeeds  {
	{"one", 1}, {"won", 1}, {"two", 2}, {"to", 2}, {"too", 2},
	{"three", 3}, {"four", 4}, {"for", 4}, {"five", 5}
};

MorseCodeMod::MorseCodeMod() { 
	rgbColor = DEFAULTCOLOR;	
	tempo = DEFAULTTEMPO;
}

MorseCodeMod::MorseCodeMod(std::string const& color, int speed) {
	constSpeedHandler(speed);
	constColorHandler(color);
}

MorseCodeMod::MorseCodeMod(std::string const& color) {
	constColorHandler(color);
	tempo = DEFAULTTEMPO;
}

MorseCodeMod::MorseCodeMod(int speed) {
	constSpeedHandler(speed);
	rgbColor = DEFAULTCOLOR;
}

void MorseCodeMod::constSpeedHandler(int speed) {
	if (speed > 0 && speed <= NUMLEDSPEEDS) {
		tempo = speed;
	} else {
		throw "Invalid speed value\n";
	}
}

void MorseCodeMod::constColorHandler(std::string const& color) {
	string newColor = isValidColor(color);
	if (newColor.length() != 0) { 
		rgbColor = newColor;
	} else {
		throw "" + color + " is an invalid Hex RGB color\n";
	}
}

bool MorseCodeMod::setColor(string const& rgbInHex) {
	string newColor = isValidColor(rgbInHex);
	 if (newColor.length() != 0) { // if == 0, then iVC returned ""
	 	rgbColor = newColor; // user sets new color 
	 	return true;
	 }
	 return false;
}

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

string MorseCodeMod::getColor() {
	return rgbColor;
}

bool MorseCodeMod::setTempo(int newTempo) {
	if (newTempo > 0 && newTempo <= NUMLEDSPEEDS) {
		tempo = newTempo;
		return true;
	}
	return false;
}

int MorseCodeMod::getTempo() {
	return tempo;
}

// add colorName -> color to voiceToColors, Speech class
// uses this. 
bool MorseCodeMod::addClrRecog(string const& clrName, string const& rgbHex) {
	string newColorInHex = isValidColor(clrName);
	if (newColorInHex.length() == 0) return false;
	voiceToColors.insert({clrName, newColorInHex}); 
	return true;
	
}




// SPEECH CLASS 

// "hello world in speed five in color navy blue"
// "hello world in speed five"
// "hello world in color navy blue"

// So, if find "in color" then rest of string is the color name
// If find "in speed", then next word is speed (five NOT 5)

Speech::Speech() { 
	decryptedMessage = "";
	rgbColor = "haha";
	tempo = 5;
	string text = voiceToText();
	// Check if text has commands in them 

	// at very end, if decrypted > MAXLENGTH, then throw exception 
}

string Speech::voiceToText() { 
	//cin.ignore();
	//getline(cin, messageFromPython);
	//system("/usr/bin/python3 speechRec.py");
	//getline(cin, messageFromPython);
	// retrieve message that python file printed to stdout
	//cin.ignore();
	//getline(cin, messageFromPython);
	//return "hello world";

	string spokenMessage;
	std::array<char, 256> buff;

	FILE* fp; // file pointer
	// calls python file, pipe, and read what file prints to stdout
	cout << "Speak your morse code message" << std::endl;
	fp = popen("/usr/bin/python3 speechRec.py", "r");
	if (fp == NULL) {
		std::cerr << "Error calling python STT program.\n" << std::endl;
		cout << "error !!\n";
	}
	while(fgets(buff.data(), 256, fp) != NULL) {
		spokenMessage += buff.data();
	}
	pclose(fp);

	cout << "YOU SAID: " + spokenMessage;
	return spokenMessage;

}

string Speech::colorInText(string text) { 
	return "";
}

string Speech::speedInText(string text) {
	return "";
}

bool Speech::addSTTColor(string const& colorName, string const& color) {
	string correctRGBFormat = MorseCodeMod::isValidColor(color);
	if (correctRGBFormat.length() == 0) return false;
	voiceToColors.insert({colorName, color});
	return true;
}





from graphics import *

# Variables / Constants
WINDOW_WIDTH = 500
WINDOW_HEIGHT = 500

def main():
	window = GraphWin("Drone Window", WINDOW_WIDTH, WINDOW_HEIGHT)
	window.setBackground() # black background


	# respond to user hitting red x
	win.getMouse()
	win.close()

main()











