#include "speechAssist.h"
#include <iostream>
#include <map>
#include <algorithm>
#include <stdexcept>
#include <ctype.h>
#include <python2.7/Python.h> // qualify the include
#include <string>
using namespace std;

map<string, string> voiceToColors = {
	{"white", "FFFFFF"}, {"red", "FF0000"}, {"green", "008000"},
	{"blue", "0000FF"}, {"purple", "800080"}, {"yellow", "FFFF00"},
	{"pink", "FFC0CB"}, {"brown", "A52A2A"}
};

map<string, int> voiceToSpeeds  {
	{"one", 1}, {"two", 2}, {"three", 3}, {"four", 4}, {"five", 5}
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
	// string text = voiceToText();
	// Check if text has commands in them 

	// at very end, if decrypted > MAXLENGTH, then throw exception 
}

string Speech::voiceToText() { 
	// PyObject: Generic type Python object in C
	PyObject *pyMod;
	PyObject *pyClass;
	PyObject *pyArgs;
	PyObject *pyInst;
	PyObject *pyMethod; 
	PyObject *pyRes;

	Py_Initialize();
	pyMod = PyImport_ImportModule("speechRec"); // import the python module
	pyClass = PyObject_GetAttrString(pyMod, "Speak"); // perform attr quals
	Py_DECREF(pyMod); // give away ownership of obj passed to C from API 

	pyArgs = Py_BuildValue(" () ");
	pyInst = PyEval_CallObject(pyClass, pyArgs);
	Py_DECREF(pyClass);
	Py_DECREF(pyArgs);

	pyMethod = PyObject_GetAttrString(pyInst, "speakToMic");
	Py_DECREF(pyInst);
	// pyArgs = PyBuildValue(" () ");
	pyRes = PyEval_CallObject(pyMethod, pyArgs); // calls the python method stM
	Py_DECREF(pyMethod);
	Py_DECREF(pyArgs);

	char* cstr;
	PyArg_Parse(pyRes, "s", &cstr);
	printf("%s\n", cstr);
	Py_DECREF(pyRes);

	return "hello world";

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


















