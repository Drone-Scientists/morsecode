from graphics import *
import time

'''
Uses the graphics module created by John Zelle in Graphics.py 
located in the same directory as this file.
'''

# Variables / Constants
WINDOW_WIDTH = 500
WINDOW_HEIGHT = 500
skyBlue = (137, 207, 240)

# mc is morsecode message
def morseCode(mc, R, G, B, tempo):
	win = GraphWin("Drone Window", WINDOW_WIDTH, WINDOW_HEIGHT)
	win.setBackground(color_rgb(skyBlue[0], skyBlue[1], skyBlue[2])) # sky
	droneImage = Image(Point(250, 250), "drone1.png") # transparent drone png
	droneImage.draw(win)

	win.getMouse() # waits for a mouse response
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

	morseCode(morseCode, Rdecimal, Gdecimal, Bdecimal, tempo)

main()















