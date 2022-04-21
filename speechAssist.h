#ifndef SPEECHASSIST_H
#define SPEECHASSIST_H

// The MorseCodeMod class handles modifications that are possible
// to apply to the drone's LED morse code communication like 
// LED color and tempo/speed of the 'dits/dahs'
#include "mcTranslator.h"
#include <string>
#include <map>

extern std::map<std::string, std::string> voiceToColors;

// higher value = faster MC communication = shorter 'dit' time
#define NUMLEDSPEEDS 5
#define DEFAULTTEMPO 3
#define DEFAULTCOLOR "FFFFFF" // white 

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
// speech-to-text (STT) conversion with the help of the API AssemblyAI (www.assemblyai.com). 
// The converted text will contain the decypted message the user wants to communicate 
// with morse code and special (optional) key phrases like "in (color)." 

class Speech {
public:
	std::string decryptedMessage;
	std::string rgbColor; // taken from speech-to-text conversion 
						  // if not specified, then stays default color
	int tempo;
public:
	Speech(); 
	std::string voiceToText();
	std::string colorInText(std::string text); // find things like "in red" after tts conversion 
	std::string speedInText(std::string text);
	static bool addSTTColor(std::string const& colorName, std::string const& color); 
};

#endif













