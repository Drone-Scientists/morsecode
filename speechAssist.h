#ifndef SPEECHASSIST_H
#define SPEECHASSIST_H

// The MorseCodeMod class handles modifications that are possible
// to apply to the drone's LED morse code communication like 
// LED color and tempo/speed of the 'dits/dahs'
#include "mcTranslator.h"
#include <string>
#include <vector>
#include <map>

extern std::map<std::string, std::string> voiceToColors;

// higher value = faster MC communication = shorter 'dit' time
#define NUMLEDSPEEDS 5
#define DEFAULTTEMPO 3
#define DEFAULTCOLOR "FFFF00" // yellow in hex
#define DEFAULTCOLORNAME "yellow"

class MorseCodeMod {
private: // privacy due to needing to check drone compatibility to field vals 
	// FIELDS: specifications for morse code that can be communicated
	// to the drone 
	std::string rgbColor; 
	int tempo; // different mc speed conversion, higher = faster 

public: 
	MorseCodeMod(); // mod values go to drone's default values corr. to the fields
	MorseCodeMod(std::string const& color, int speed); // user spec. all mods
	MorseCodeMod(std::string const& color); // only spec color mod
	MorseCodeMod(int speed); // only specify dit MC speed 

	void constSpeedHandler(int speed);
	void constColorHandler(std::string const& color);

	bool setColor(std::string const& rgbInHex);
	std::string getColor();
	bool setTempo(int newTempo);
	int getTempo();

	static bool addClrRecog(std::string const& clrName, std::string const& rgbHex); 
					// adds color/RGB to the map that's used for the 
					// voice color commands API 

	static std::string isValidColor(std::string const& rbgInHex);

};



// SPEECH CLASS 
// The Speech class allows the user to provide mp3 audio files that will undergo 
// speech-to-text (STT) conversion with the help of Google's API in the PyAudio module.
// The converted text will contain the decypted message the user wants to communicate 
// with morse code and special (optional) key phrases like "in (color)." 

class Speech {
private:
	// all start with default vals, constructor changes vals if STT has command
	std::string decryptedMessage = "";
	std::string rgbColor = DEFAULTCOLOR;
	int tempo = DEFAULTTEMPO;

public:
	Speech(); // user needs to do STT
	Speech(std::string text); // user did STT prior, so pass string from STT here
	std::string voiceToText();
	void printSpeech();
	std::string colorInText(std::string text); // find things like "in red" after tts conversion 
	std::string speedInText(std::string text);
	static bool addSTTColor(std::string const& colorName, std::string const& color);
	static std::vector<std::string> splitStringBySpaces(std::string text); 
	static std::string toLowerCase(std::string text);

	// Getters and Setters - private fields so user can't change values that are
	// incompatible with the drone (Ex. a tempo > NUMLEDSPEEDS)
	bool setDecryptedMessage(std::string newText); // make sure size !> max dec length
	bool setColor(std::string color); // make sure is a valid RGB color 
	bool setTempo(int speed); // new speed needs to be 1-NUMLEDSPEEDS
	std::string getDecryptedMessage();
	std::string getColor();
	int getTempo();
};

#endif













