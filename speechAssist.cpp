#include "speechAssist.h"
#include <map>
#include <stdexcept>
#include <ctype.h>
#include <string>
using namespace std;

map<string, string> voiceToColors = {
	{"white", "FFFFFF"}, {"red", "FF0000"}, {"green", "008000"},
	{"blue", "0000FF"}, {"purple", "800080"}, {"yellow", "FFFF00"},
	{"pink", "FFC0CB"}, {"brown", "A52A2A"}
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
	string newColor = isValidColor(clrName);
	if (newColor.length() != 0) { 
		voiceToColors.insert({clrName, newColor}); 
		return true;
	}
	return false;
}




// SPEECH CLASS - WIP

Speech::Speech(string mp3) { // WIP
	// decryptedMessage = Speech::voiceToText(mp3); **how should work**
	decryptedMessage = mp3;
	// call colorInText
}

string Speech::voiceToText(string mp3) { // WIP
	return mp3; // default while working on other methods 
}

bool Speech::colorInText(string text) { // WIP
	// if find "in (color)", change color field to specified color 
	return true;
}

bool Speech::addSTTColor(string const& colorName, string const& color) {
	string correctRGBFormat = MorseCodeMod::isValidColor(color);
	if (correctRGBFormat.length() == 0) return false;
	voiceToColors.insert({colorName, color});
	return true;
}


















