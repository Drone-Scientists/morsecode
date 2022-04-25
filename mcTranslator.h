#ifndef MCTRANSLATOR_H
#define MCTRANSLATOR_H

#include <string>
#include <map>

extern std::map<char, std::string> mcKey;

#define MAXDECRYPTEDTLEN 100
#define MAXENCRPYEDTLEN 500 // longest possible mc message (num of chars)

// Morse Code Timing:
// in between letters will add " " equal to 3 * dit length 
// in between words will add "/" indicating a space
// Meaning, a space in morse code = " / " equal to 7 * dit length



// Message class ensures the correctness of the English and Morse Code strings,
// even when the user changes a field's value 

class Message {
private: // privacy to enforce correct relationship between the 2 fields, 
		 // and correctness of the morse code syntax and/or valid morse code 
	std::string inDecrypted;
	std::string inEncrypted;

public:
	Message(std::string s, bool isEncryped);
	bool setDecrypted(std::string s); // sets will automatically update the opposing 
	bool setEncrypted(std::string s); // field value 
	std::string getDecrypted();
	std::string getEncrypted();
	bool toMorseCode(std::string const& message);
	bool decryptMC(std::string const& mc);

};

#endif
