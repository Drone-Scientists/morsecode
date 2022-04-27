#include "speechAssist.h"
#include "mcTranslator.h"
#include <iostream>
#include <map>
#include <algorithm>
#include <stdlib.h>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <array>
#include <ctype.h>
#include <string>
using namespace std;

map<string, string> voiceToColors = {
	{"white", "FFFFFF"}, {"red", "FF0000"}, {"green", "008000"},
	{"blue", "0000FF"}, {"purple", "800080"}, {"yellow", "FFFF00"},
	{"pink", "FFC0CB"}, {"brown", "A52A2A"}, {"magenta", "FF00FF"},
	{"fuchsia", "FF00FF"}
};

// used when user specifies speed vocally in morse code command 
// when using speech to text
map<string, int> voiceToSpeeds  {
	{"one", 1}, {"won", 1}, {"two", 2}, {"to", 2}, {"too", 2},
	{"three", 3}, {"four", 4}, {"for", 4}, {"five", 5}, {"1", 1},
	{"2", 2}, {"3", 3}, {"4", 4}, {"5", 5}
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
// state speed command first

// "hello world in speed five and in color red"
// "hello world in speed five"
// "hello world in color navy blue"

// So, if find "in color" then rest of string is the color name
// If find "in speed", then next word is speed (five NOT 5)

// Constructor that prompts user for STT
Speech::Speech() { 
	string text = voiceToText(); // full text, including optional commands
	text = toLowerCase(text);
	// Check if text has commands in them (in speed <int> and in color <string>)
	text = colorInText(text);
	text = speedInText(text);
	if (text.size() > MAXDECRYPTEDTLEN) { // check if command is too long
		throw "Your command <" + text + "> exceeds the max character length of " 
				+ std::to_string(MAXDECRYPTEDTLEN) + "\n";
	}
	decryptedMessage = text;
}

// Constructor that does NOT prompt user for STT, used when user has done
// STT prior, and also used for testing purposes for the Speech class 
Speech::Speech(string text) {
	text = toLowerCase(text);
	text = colorInText(text);
	text = speedInText(text);
	if (text.size() > MAXDECRYPTEDTLEN) { // check if command is too long
		throw "Your command <" + text + "> exceeds the max character length of " 
				+ std::to_string(MAXDECRYPTEDTLEN) + "\n";
	}
	decryptedMessage = text;

}

void Speech::printSpeech() {
	cout << "Message: <" + decryptedMessage + ">\n";
	cout << "Color (in hex): <" + rgbColor + ">\n";
	cout << "Tempo: <" + std::to_string(tempo) + ">" << endl;
	return;
}

string Speech::voiceToText() { 
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
	while (fgets(buff.data(), 256, fp) != NULL) {
		spokenMessage += buff.data();
	}
	pclose(fp);
	cout << "You said:\n" << spokenMessage << endl;
	return spokenMessage;

}

// "... in color navy blue"
// checks if there is a color command in the text, if there is 
// if removes it from the command and changes color field value, returns
// the command
string Speech::colorInText(string text) { 
	vector<string> words = splitStringBySpaces(text); // words in text
	string textNoColorCommand = words[0];

	bool foundInColor = false;
	string prevWord = words[0]; 
	string color = "";
	int numOfWords = words.size();
	for (int i = 1; i < numOfWords; i++) { // iterate through words in vector 
		string twoWords = prevWord + ' ' + words[i];
		if (foundInColor && (color.size() == 0)) { // first word of color
			color += words[i];

		} else if (foundInColor && (color.size() != 0)) {
			color = color + ' ' + words[i];

		} else if (twoWords.compare("in color") == 0) { // color cmd keyword found 
			foundInColor = true;
			for (int j = 0; j < 3; j++) { // remove " in" from string 
				textNoColorCommand.pop_back();
			}

		} else {
			textNoColorCommand = textNoColorCommand + " " + words[i];
		}

		prevWord = words[i];
	}

	if (color.size() != 0) { // a color command was found, change color field value 
		std::map<string, string>::iterator it = voiceToColors.find(color);
		if (it != voiceToColors.end()) { // color was found in STT map! 
			rgbColor = voiceToColors[color];

		} else { 
			throw "" + color + " is not a STT color. You may add this color using\n"
				+ "the menu option 8\n";
		}
	}

	return textNoColorCommand;
}

// Checks if 
string Speech::speedInText(string text) {
	vector<string> words = splitStringBySpaces(text); // words in text
	string textNoSpeedCommand = words[0];

	bool foundInSpeed = false;
	string prevWord = words[0]; 
	int numOfWords = words.size();
	for (int i = 1; i < numOfWords; i++) { // iterate through words in vector 
		string twoWords = prevWord + ' ' + words[i];
		if (twoWords.compare("in speed") == 0) {
			foundInSpeed = true;

		} else if (foundInSpeed) {
			string speedRequest = words[i];
			std::map<string, int>::iterator it = voiceToSpeeds.find(speedRequest);
			if (it != voiceToSpeeds.end()) { // speed was found in voiceToSpeed map
				tempo = voiceToSpeeds[speedRequest];

			} else { // speed was not found 
				throw "" + speedRequest + " is not a morse code speed. The available\n"
					+ "speeds are 1-" + std::to_string(NUMLEDSPEEDS) + "\n";
			}

			for (int j = 0; j < 3; j++) { // remove " in" from string 
				textNoSpeedCommand.pop_back();
			}
			break; // skips possible trailing 'and'

		} else {
			textNoSpeedCommand = textNoSpeedCommand + " " + words[i];
		}
		prevWord = words[i];
	}

	return textNoSpeedCommand;
}

bool Speech::addSTTColor(string const& colorName, string const& color) {
	string correctRGBFormat = MorseCodeMod::isValidColor(color);
	if (correctRGBFormat.length() == 0) return false;
	voiceToColors.insert({colorName, color});
	return true;
}

vector<string> Speech::splitStringBySpaces(string text) {
	vector<string> words;
	stringstream ss(text);
	string word;
	while (ss >> word) {
		words.push_back(word); // add word to vector
	}
	return words;
}

// return string passed but all chars are lowercase 
string Speech::toLowerCase(string text) {
	int textLength = text.size(); // bound
	string lowerCaseText = text;
	for (int i = 0; i < textLength; i++) {
		lowerCaseText[i] = tolower(lowerCaseText[i]); // char = to itself lower
	}
	return lowerCaseText; 
}

bool Speech::setDecryptedMessage(string newText) {
	int newTextLength = newText.length();
	// invalid if new text is too long or if is ""
	if ((newTextLength > MAXDECRYPTEDTLEN) || (newTextLength == 0)) {
		return false;
	}
	decryptedMessage = Speech::toLowerCase(newText);
	return true;
}

bool Speech::setTempo(int speed) {
	if ((speed >= 1) && (speed <= NUMLEDSPEEDS)) {
		tempo = speed;
		return true;
	}
	return false;
}

bool Speech::setColor(string color) {
	// check if a valid RGB formatted string
	string reformatColor = MorseCodeMod::isValidColor(color);
	if (reformatColor.size() != 0) { // "" if not valid RGB 
		rgbColor = color;
		return true;
	}
	return false;
}

string Speech::getDecryptedMessage() {
	return decryptedMessage;
}

string Speech::getColor() {
	return rgbColor;
}

int Speech::getTempo() {
	return tempo;
}










