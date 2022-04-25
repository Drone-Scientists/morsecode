from graphics import *
import time

'''
Uses the graphics module created by John Zelle in Graphics.py 
located in the same directory as this file.
'''

'''
MORSE CODE TIMING (in units, units can be anything, like 1 second)
".--. .- .-. .. .../" PARIS (example)

Dit (.): 1 unit 
Dah (-): 3 units
Between each Dit & Dah: 1 unit SILENCE
Between each letter in word ( ): 3 units SILENCE 
Word space (/) (gap between 2 words): 7 units SILENCE

Meaning
(.) -> LED for 1 unit
(-) -> LED for 3 units
between .- -> LED off for 1 unit
( ) -> LED off for 3 units
(/) -> LED off for 7 units

Note: used morsecode.world/international/timing.html as a guide
'''

# Variables / Constants
WINDOW_WIDTH = 500
WINDOW_HEIGHT = 500
skyBlue = (137, 207, 240)
mcSpeeds = {
	"1" : 1, "2" : .5, "3" : .2, "4" : .1, "5" : .05
}

# mc is morsecode message
def CommMorseCodeLED(morsecode, R, G, B, tempo):
	win = GraphWin("Drone Window", WINDOW_WIDTH, WINDOW_HEIGHT)
	win.setBackground(color_rgb(skyBlue[0], skyBlue[1], skyBlue[2])) # sky
	droneImage = Image(Point(250, 250), "drone1.png") # transparent drone png
	droneImage.draw(win)
	LED = Circle(Point(250, 200), 20)
	LED.draw(win)

	UNIT = mcSpeeds[tempo]

	# <.... . .-.. .-.. --- / .-- --- .-. .-.. -.. > has trailing space

	# after a . or -, always silence for 1 unit, then if next char
	# requires a silence (' ' or '/'), then subtract 1 unit from its
	# silence
	for char in morsecode:	# LED then small silence
		if char == '.':
			LED.setFill(color_rgb(R,G,B)) # LED on
			time.sleep(UNIT) # sleep 1 unit
			LED.setFill("black") # LED off 
			time.sleep(UNIT) # sleep 1 unit

		elif char == '-': # LED then small silence
			LED.setFill(color_rgb(R,G,B)) # LED on
			time.sleep(UNIT * 3) # sleep 3 unit
			LED.setFill("black") # LED off 
			time.sleep(UNIT) # sleep 1 unit

		elif char == ' ':  # silence!
			LED.setFill("black") # LED off
			time.sleep((3 * UNIT) - (UNIT)) # sleep 3 units - 1 unit 

		else: # it's a '/'    # silence!
			LED.setFill("black") # LED off 
			time.sleep((7 * UNIT) - (UNIT)) # sleep 7 units - 1 unit

	# done with Morse Code, now exit program 
	#win.getMouse() # waits for a mouse response
	print("Morse Code Finished! Bye now.")
	win.close()



def main():
	# First, grab morse code info from message.txt
	messageFile = open('message.txt', 'r')
	lines = messageFile.readlines()
	morseCode = (lines[0])[:-1] # removes last char, which is a \n
	rgbColor = (lines[1])[:-1]
	tempo = (lines[2])[:-1]
	messageFile.close() # close file stream 

	# **decimal value** of each R G B, message.txt has in hex, but
	# graphics module needs colors in decimal
	Rdecimal = int(rgbColor[0:2], 16) # first 2 chars
	Gdecimal = int(rgbColor[2:4], 16)
	Bdecimal = int(rgbColor[4:6], 16)

	CommMorseCodeLED(morseCode, Rdecimal, Gdecimal, Bdecimal, tempo)

main()















