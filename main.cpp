#include <map>
#include <iostream>
#include <string>
#include <exception>
#include <vector>
#include <algorithm>
using namespace std;
#include "mcTranslator.h"
#include "speechAssist.h"

#define NUMMENUITEMS 9


struct MessageDetails {
	Message* m;
	MorseCodeMod* mcm;
};

namespace mcInfo {

	void printMCKey(map<char, string>& mcKey) { // morse code key used for conv
		cout << "\n** MORSE CODE KEY **\n\n<character> --> <morse code>\n";
		map<char, string>::iterator iter;
		for (iter = mcKey.begin(); iter != mcKey.end(); iter++) {
			string s(1, iter->first);
			cout << "<" + s + ">   -->   <" + iter->second + ">\n";
		}
	}

	void printTTSColors(map<string, string>& voiceToColors) { // print TTS map vals
		cout << "The following colors can be recognized when you speak your " <<
		"message\n into the microphone when 'in (color-name) is said.\n";
		cout << "\nTEXT TO SPEECH COLORS:\n";
		map<string, string>::iterator it;
		for (it = voiceToColors.begin(); it != voiceToColors.end(); it++) {
			cout << it->first + "  -->  " + it->second + "\n";
		}
	}

	void printMorseHelp() {
		cout << "<<MORSE CODE HELP>>\n" << "This application utilizes " <<
		"the following morse code conversion laws:\n" <<
		"1) '.' is a 'dit' -> a short LED flash\n" <<
		"2) '-' is a 'dah' -> LED pulse equivalent to 3 dits in length\n" <<
		"3) all characters in morse code are followed by a ' ' space.\n" <<
		"4) '/' represents a space inbetween words, and is also followed\n" <<
		"    by a ' ' space.\n\n";

		string resp;
		cout << "Type <Y> to see the Morse Code Key.\n";
		getline(cin, resp);

		mcInfo::printMCKey(mcKey);
	}

	void printWelcomeMessage() {
		cout << "-- DRONE MORSE CODE APPLICATION --\n\n" <<
		"Welcome! This application is used to aid in the process of\n" <<
		"communicating morse code via an attached LED on a drone. Follow\n" <<
		"the incoming prompts to store and send messages to the drone.\n\n";
		mcInfo::printMorseHelp();
	}
}




namespace userHandler {

	void printMenu() {
		cout << "\n[ - - - M E N U - - - ]\n\n" <<
		"(1) Add a Message\n" << // 2 routes, have message or have MC 
		"(2) Add a Message by speaking into your microphone\n" <<
		"(3) Print Saved Messages\n" << 
		"(4) Send Message to Drone\n" << // WORK IN PROG 
		"(5) Delete Messages\n" <<
		"(6) Print Morse Code Key\n" <<
		"(7) Print Colors options for Speech to Text (STT)\n" <<
		"(8) Add Colors for Speech to Text (STT)\n" <<
		"(9) Exit Program\n";

		cout << "\nWhat would you like to do? (Type #)" << endl;
	}

	bool yesNoResponse() {
		string resp;
		cin >> resp;

		// input needs to be size 1 (char) and either Y or N to continue
		while (resp.size() != 1 || !(resp[0] == 'Y' || resp[0] == 'N')) {
			cout << "Invalid Input. Please type 'Y' or 'N'\n";
			cin >> resp; // keep prompting 
		}
		return resp[0] == 'Y';
	}

	bool backToMenu() {
		cout << "\nReturn to Main Menu? ('Y' or 'N')" << endl;
		return userHandler::yesNoResponse();
	}

	void exitHandler(vector<MessageDetails>& vect) {
		for (int i = 0; i < vect.size(); i++) { // free objects in vector 
			free(vect.at(i).m); // free Message
			free(vect.at(i).mcm); // free Mod
		}
		vect.clear();
	}

	void printMessageList(vector<MessageDetails>& vect) { 
		cout << "\n";
		int numVectEles = vect.size();
		if (numVectEles == 0) {
			cout <<  "There are no Messages" << endl;
			return;
		}
		for (int i = 0; i < numVectEles; i++) { // each vector element
			cout << "Message " + to_string(i + 1) + ":\n" << 
			"Decrypted: <" << (vect.at(i).m)->getDecrypted() << ">\n" << // Message
			"Encrypted: <" << (vect.at(i).m)->getEncrypted() << ">\n" <<
			"    Color: " << (vect.at(i).mcm)->getColor() << "\n" << // MorseCodeMod
			"    Speed: " << (vect.at(i).mcm)->getTempo() << "\n" << endl;
		}
	}

	void addMessageHandler(vector<MessageDetails>& vect) { 
		cout << "\nADD MESSAGE - Do you have ...\n" <<
		"    (1) A decrypted message OR\n" <<
		"    (2) An encrypted (in morse code) message?" << endl;
		int numResp;
		cin >> numResp;

		while (cin.fail() || !(numResp == 1 || numResp == 2)) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
			cout << "Invalid input. Please enter '1' or '2'" << endl;
			cin >> numResp;
		}

		string resp;
		Message* mess;

		// USER MAKING MESSAGE OBJECT 
		while (1) {
			cin.ignore(); // fixes so dont have to hit enter twice
			try {
				if (numResp == 1) { // user has normal message
					string maxDec = to_string(MAXDECRYPTEDTLEN);
					cout << "\nPlease enter your decrypted message of max length " +
					maxDec << endl; 
					getline(cin, resp);
					mess = new Message(resp, false);

				} else { // user has their message in MC 
					string maxEnc = to_string(MAXENCRPYEDTLEN);
					cout << "\nPlease enter your encrypted message of max length " +
					maxEnc << endl;
					getline(cin, resp);
					mess = new Message(resp, true);
				}
			} catch (exception& e) {
				cout << e.what() << "type 'N' if you'd like to try again" << endl;
				if (!backToMenu()) continue;
			}
			break; // creating new Message was a success
		}
		MorseCodeMod* mod;
		int speed = 0; // user un-specified values 
		string rgbColor = "";

		// USER MAKING MOD OBJECT 
		cout << "\nWould you like to specify a color for the drone's LED? " <<
		"('Y' or 'N')\n (The default color is 'FFFFFF' - white)" << endl;
		if (userHandler::yesNoResponse()) { // yes wants to specify color 
			cout << "Give color in hex. Example: give 'FF0000' for red" << endl;
			cin >> rgbColor;
		} 

		cout << "\nWould you like to specify a blinking speed for the morse " <<
		"code? ('Y' or 'N')" << endl;
		if (userHandler::yesNoResponse()) { // user wants to specify speed
			cout << "Type 1-" + to_string(NUMLEDSPEEDS) + ", where 1 is " << 
			"the slowest speed." << endl;
			cin >> speed;
		}

		while (1) {
			try {
				if (rgbColor.size() == 0) {
					if (speed == 0) {
						mod = new MorseCodeMod();
					} else {
						mod = new MorseCodeMod(speed);
					}
				} else {
					if (speed == 0) {
						mod = new MorseCodeMod(rgbColor);
					} else {
						mod = new MorseCodeMod(rgbColor, speed);
					}
				}
			} catch (exception& e) {
				cout << e.what() << "type 'N' if you'd like to try again." << endl;
				if (!userHandler::backToMenu()) continue;
			}
			break; // creating new Mod obj was a success 
		}
		struct MessageDetails md = {mess, mod};
		vect.push_back(md);

		cout << "Successfully added Message! Hit 'N' to add another." << endl;
		if (!userHandler::backToMenu()) userHandler::addMessageHandler(vect);
	}


	void sendMessage(vector<MessageDetails>& vect) { 
		// WORK IN PROGRESS - post midterm mile
	}

	void deleteHandler(vector<MessageDetails>& vect) {
		int numMessages = vect.size();
		userHandler::printMessageList(vect);
		int delPick;
		cout << "Enter the # of the Message you want to delete." << endl;
		cin >> delPick;
		while (cin.fail() || (delPick < 1 || delPick > numMessages)) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
			cout << "Invalid input. Please enter <#> of Message to delete." << endl;
			cin >> delPick;
		}
		free(vect.at(delPick - 1).m); // free Message
		free(vect.at(delPick - 1).mcm); // free Mod
		vect.erase(vect.begin() + delPick - 1); // remove from vector 

		cout << "Successfully deleted Message. Hit 'N' to delete another." << endl;
		if (!userHandler::backToMenu()) userHandler::addMessageHandler(vect);
	}

	void addColorHandler() {
		cout << "\nIs there a color you'd like to add for STT? ('Y' or 'N')" << endl;
		if (!userHandler::yesNoResponse()) return;
		cin.ignore();
		while (1) { // user can keep adding colors until want go back to Menu
			string newSTTrgb;
			string newSTTColorName;

			cout << "Enter in the color's hex value. Example: 'FF0000'" << endl;
			getline(cin, newSTTrgb);

			cout << "Enter the color name that will be associated with " + 
			newSTTrgb + " for STT" << endl;
			getline(cin, newSTTColorName);

			if (!Speech::addSTTColor(newSTTColorName, newSTTrgb)) {
				cout << "INVALID. Try again. See example: 'red' and 'FF0000'." << endl;
				continue;
			}

			cout << "Success! The color " + newSTTColorName + " (" + newSTTrgb + ")"
			+ " has been added\nfor Speech-to-Text Conversion." << endl;
			cout << "Would you like to add another color for STT? ('Y' or 'N')" << endl;
			if (!userHandler::yesNoResponse()) break;
			cin.ignore();
		}
	}

	void addMP3MessageHandler(vector<MessageDetails>& vect) { 
		Speech* s;
		s = new Speech();
		cout << "\nSPEECH:\n";
		s->printSpeech();
		return;
	}

};

namespace voiceToTextInfo {
	void printVoiceToTextInfo() {
		cout << "To add a your Message via speech, you can speak into\n" <<
		"your microphone when prompted. You may \n";
	}
};




int main() {
	mcInfo::printWelcomeMessage();

	cout << "\nType <Y> when you're ready to begin." << endl;
	string response;
	cin >> response;

	// if user gives non-char answer / not == 'Y', keep prompting
	while (response.size() != 1 || response[0] != 'Y') { 
		cout << "Invalid input. Please enter <Y> to begin" << endl;
		cin >> response; 
	}

	vector<MessageDetails> vect;

	int menuPick;
	while (menuPick != NUMMENUITEMS) { // until user wants to exit prog
		userHandler::printMenu();
		cin >> menuPick;

		// if user gives non-int answer / out of bounds, keep prompting
		while (cin.fail() || (menuPick < 1 || menuPick > NUMMENUITEMS)) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
			cout << "Invalid input. Please enter <#> to choose menu option." << endl;
			cin >> menuPick;
		}

		if (menuPick == 1) { // Add a Message
			userHandler::addMessageHandler(vect);

		} else if (menuPick == 2) { // add via mp3
			userHandler::addMP3MessageHandler(vect);
			userHandler::backToMenu();

		} else if (menuPick == 3) { // print saved messages
			userHandler::printMessageList(vect);
			userHandler::backToMenu();

		} else if (menuPick == 4) { // send message to drone 
			// WIP 

		} else if (menuPick == 5) { // delete messages 
			userHandler::deleteHandler(vect);

		} else if (menuPick == 6) {
			mcInfo::printMorseHelp();
			userHandler::backToMenu();

		} else if (menuPick == 7) { // print colors
			mcInfo::printTTSColors(voiceToColors);
			userHandler::backToMenu();

		} else if (menuPick == 8) { // add colors 
			mcInfo::printTTSColors(voiceToColors);
			userHandler::addColorHandler();

		} else {
			userHandler::exitHandler(vect);
		}
	}

	return 0;
}















