import speech_recognition as sr
import os 
import time 
import playsound
import pyaudio
from gtts import gTTS

# Can speak into the microphone and then returns what's 
# said as a string 
def speakToMic():
	recObj = sr.Recognizer()
	# microphone is what will be the input stream
	with sr.Microphone() as inputSource:
		print("Speak your morse code message")
		audio = recObj.listen(inputSource)
		capturedAudio = ""

		try:
			capturedAudio = recObj.recognize_google(audio) # use google API for STT

		# Can get an error if:
			# API doesnt understand what you said
			# mic isn't picking up anything
			# etc.
		except Exception as e: 
			print("Exception found while recording audio.\n Exception: " + e)

	return capturedAudio

# output text variable as speaker audio 
def speakToYou(text):
	tts = gTTS(text="you said "+text, lang="en")
	tts.save("voice.mp3")
	playsound.playsound("voice.mp3")


#recordSound()
cap = speakToMic()
print(cap)
speakToYou(cap)






