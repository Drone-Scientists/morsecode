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
	"1" : 1, 
	"2" : .5, 
	"3" : .2, 
	"4" : .1, 
	"5" : .05
}

# SPECIAL COLOR - Expansion pack! 

rainbow = [
	[255, 0, 0], # red
	[255, 125, 0], # orange
	[255, 255, 0], # yellow
	[0, 255, 0], # green
	[0, 0, 255], # blue
	[75, 0, 130], # indigo
	[238, 130, 238] # violet
]

cautiouslyPatriotic = [
	[255, 0, 0], # red
	[255, 255, 255], # white
	[0, 0, 255]  # blue
]	

goCougs = [
	[202, 18, 55], # red
	[0, 0, 0], # black
	[255, 255, 255] # white
]

''' 
CITATION:
The color scheme below was created with the assistance
of Derek Schreiner, a level 1 programmer and loyal Rams fan.
'''
NUMGORAMSSUPERBOWLCHAMPS2022COLORS = 2 # "The *OFFICIAL* name"
NUMRAMSCOLORS = 2 # cute, short alias 
goRamsSuperBowlChamps2022 = [
	[0, 53, 148], # Rams Royal Blue ©
	[255, 209, 0] # Sol Yellow ©
]

holidayCheer = [
	[255, 0, 0], # red
	[0, 0, 255] # blue 
]


# communicates morse code, but every changes colors based on the
# special color that is used
# Want whole new method, so normal method isn't checking 
# "if special" / incrementing next color all the time (would slow 
# down performance)
def CommMorseCodeLEDSpecial(win, morsecode, special, tempo):
	sc = 0 # sc for specialColors
	if special == "rainbow":
		sc = rainbow
	elif special == "patriotic":
		sc = cautiouslyPatriotic
	elif special == "cougars":
		sc = goCougs
	elif special == "rams":
		sc = goRamsSuperBowlChamps2022
	elif special == "holidays":
		sc = holidayCheer
	else: 
		print("Incorrect special LED flag")
		return 

	numColors = len(specialColors)

	LED = Circle(Point(250, 200), 20)
	LED.draw(win)
	UNIT = tempo

	# <.... . .-.. .-.. --- / .-- --- .-. .-.. -.. > has trailing space

	# after a . or -, always silence for 1 unit, then if next char
	# requires a silence (' ' or '/'), then subtract 1 unit from its
	# silence
	R = 0
	G = 1
	B = 2
	i = 0 # colors in array of colors 
	for char in morsecode:	# LED then small silence
		if char == '.':
			LED.setFill(color_rgb(sc[i][R],sc[i][G],sc[i][B])) # LED on
			time.sleep(UNIT) # sleep 1 unit
			LED.setFill("black") # LED off 
			time.sleep(UNIT) # sleep 1 unit
			i += 1
			if i >= numColors:
				i = 0

		elif char == '-': # LED then small silence
			LED.setFill(color_rgb(sc[i][R],sc[i][G],sc[i][B])) # LED on
			time.sleep(UNIT * 3) # sleep 3 unit
			LED.setFill("black") # LED off 
			time.sleep(UNIT) # sleep 1 unit
			i += 1
			if i >= numColors:
				i = 0

		elif char == ' ':  # silence!
			LED.setFill("black") # LED off
			time.sleep((3 * UNIT) - (UNIT)) # sleep 3 units - 1 unit 

		else: # it's a '/'    # silence!
			LED.setFill("black") # LED off 
			time.sleep((7 * UNIT) - (UNIT)) # sleep 7 units - 1 unit

	# done with Morse Code, now exit program 
	print("Morse Code Finished! Bye now.")
	win.close()



# mc is morsecode message
def CommMorseCodeLED(win, morsecode, R, G, B, tempo):
	'''
	win = GraphWin("Drone Window", WINDOW_WIDTH, WINDOW_HEIGHT)
	win.setBackground(color_rgb(skyBlue[0], skyBlue[1], skyBlue[2])) # sky
	droneImage = Image(Point(250, 250), "drone1.png") # transparent drone png
	droneImage.draw(win)
	'''
	LED = Circle(Point(250, 200), 20)
	LED.draw(win)

	UNIT = tempo

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

	# Start up graphics 
	win = GraphWin("Drone Window", WINDOW_WIDTH, WINDOW_HEIGHT)
	win.setBackground(color_rgb(skyBlue[0], skyBlue[1], skyBlue[2])) # sky
	droneImage = Image(Point(250, 250), "drone1.png") # transparent drone png
	droneImage.draw(win)

	CommMorseCodeLED(win, morseCode, Rdecimal, Gdecimal, Bdecimal, mcSpeeds[tempo])

main()















