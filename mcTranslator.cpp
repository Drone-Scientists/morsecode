#include "mcTranslator.h"
#include <ctype.h>
#include <map>
#include <stdexcept>
#include <string>
using namespace std;

map<char, string> mcKey = { 
	{'A', ".-"}, {'B', "-..."}, {'C', "-.-."},
	{'D', "-.."}, {'E', "."}, {'F', "..-."},
	{'G', "--."}, {'H', "...."}, {'I', ".."},
	{'J', ".---"}, {'K', "-.-"}, {'L', ".-.."},
	{'M', "--"}, {'N', "-."}, {'O', "---"},
	{'P', ".--."}, {'Q', "--.-"}, {'R', ".-."},
	{'S', "..."}, {'T', "-"}, {'U', "..-"},
	{'V', "...-"}, {'W', ".--"}, {'X', "-..-"},
	{'Y', "-.--"}, {'Z', "--.."}, {'1', ".----"},
	{'2', "..---"}, {'3', "...--"}, {'4', "....-"},
	{'5', "....."}, {'6', "-...."}, {'7', "--..."},
	{'8', "---.."}, {'9', "----."}, {'0', "-----"},
	{'?', "..--.."}, {'!', "-.-.--"}, {'.', ".-.-.-"},
	{',', "--..--"}, {';', "-.-.-."}, {':', "---..."},
	{'+', ".-.-."}, {'-', "-....-"}, {'/', "-..-."},
	{'=', "-...-"}, {' ', "/"} 
};

map<string, char> mcKeyRev = {
	{".-",'A'}, {"-...",'B'}, {"-.-.",'C'},
	{"-..",'D'}, {".",'E'}, {"..-.",'F'},
	{"--.",'G'}, {"....",'H'}, {"..",'I'},
	{".---",'J'}, {"-.-",'K'}, {".-..",'L'},
	{"--",'M'}, {"-.",'N'}, {"---",'O'},
	{".--.",'P'}, {"--.-",'Q'}, {".-.",'R'},
	{"...",'S'}, {"-",'T'}, {"..-",'U'},
	{"...-",'V'}, {".--",'W'}, {"-..-",'X'},
	{"-.--",'Y'}, {"--..",'Z'}, {".----",'1'},
	{"..---",'2'}, {"...--",'3'}, {"....-",'4'},
	{".....",'5'}, {"-....",'6'}, {"--...",'7'},
	{"---..",'8'}, {"----.",'9'}, {"-----",'0'},
	{"..--..",'?'}, {"-.-.--",'!'}, {".-.-.-",'.'},
	{"--..--",','}, {"-.-.-.",';'}, {"---...",':'},
	{".-.-.",'+'}, {"-....-",'-'}, {"-..-.",'/'},
	{"-...-",'='}, {"/",' '}
};

Message::Message(string s, bool isEncryped) { 
	if (isEncryped) { // user passed morse code message
		if (s.length() > MAXENCRPYEDTLEN) {
			string maxEnc = to_string(MAXENCRPYEDTLEN);
			throw "Encrypted text cannot exceed " + maxEnc + " characters\n";

		} else if (decryptMC(s)) { // decrypted successfully
			inEncrypted = s;

		} else {
			throw "" + s + "is not a valid morse code message and could"
						 + " not be decrypted successfully.\n";
		}
	} else { // user passed English / decrypted message
		if (s.length() > MAXDECRYPTEDTLEN) {
			string maxDec = to_string(MAXDECRYPTEDTLEN);
			throw "Decrypted text cannot exceed " + maxDec + " characters\n";

		} else if (toMorseCode(s)) {
			inDecrypted = s;

		} else {
			throw "" + s + "contains character(s) that could not be"
						 + " translated to morse code\n";
		}
	}
}

bool Message::setDecrypted(string s) {
	if (toMorseCode(s)) { // updates MC field if successful
		inDecrypted = s; // user set new valid decrypted value 
		return true;
	}
	return false;
}

bool Message::setEncrypted(string s) {
	if (decryptMC(s)) { // updates decrypted field if successful
		inEncrypted = s; // new encrypted field val 
		return true;
	}
	return false;
}

string Message::getDecrypted() {
	return inDecrypted;
}

string Message::getEncrypted() {
	return inEncrypted;
}

bool Message::toMorseCode(string const& message) { 
	string mc = "";
	int messLen = message.length();
	if (messLen == 0) {
		return "";
	}

	for (int i = 0; i < messLen; i++) {
		char c = message[i];
		if (isalpha(c)) {
			c = toupper(c);

		} else if (mcKey.find(c) == mcKey.end()) { // iter through map, & did not find key 
			return false;
		}
		mc += mcKey[c] + " ";
	}
	inEncrypted = mc;
	return true;
}

bool Message::decryptMC(string const& mc) {
	string decryptMe = mc; // copy so if need to append ' '
	int mcLength = decryptMe.length();

	if (mcLength == 0) {
		return "";
	}
	if (decryptMe[mcLength - 1] != ' ') { // should have a space at the end so can decrypt
		decryptMe += ' ';
		mcLength++;
	}	

	string decrypted = "";
	string findChar = "";
	for (int i = 0; i < mcLength; i++) {
		if (decryptMe[i] == ' ') { // end of dits/dahs for a char
			if (mcKeyRev.find(findChar) == mcKeyRev.end()) {
				return false;

			} else {
				char charFound = mcKeyRev[findChar];
				string s(1, charFound);
				decrypted += s;
			}
			findChar = ""; // reset to find the next char 

		} else {
			string s(1, decryptMe[i]);
			findChar += s;
		}
	}
	inDecrypted = decrypted;
	return true;
}





