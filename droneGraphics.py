from graphics import *
import time

'''
Uses the graphics module created by John Zelle in Graphics.py 
located in the same directory as this file.
'''

# Variables / Constants
WINDOW_WIDTH = 500
WINDOW_HEIGHT = 500

def main():
	# First, grab morse code info from message.txt
	messageFile = open('message.txt', 'r')
	lines = messageFile.readlines()
	morseCode = (lines[0])[:-1] # removes last char, which is a \n
	rgbColor = (lines[1])[:-1]
	tempo = (lines[2])[:-1]
	messageFile.close() # close file stream 

	# decimal value of each R G B, which is 2 digit hex (need to convert)
	R = rgbColor[0:2] # first 2 chars
	G = rgbColor[2:4]
	B = rgbColor[4:6]
	print(rgbColor)
	print(R)
	print(G)
	print(B)

	win = GraphWin("Drone Window", WINDOW_WIDTH, WINDOW_HEIGHT)
	win.setBackground(color_rgb(255, 0, 0)) # black background
	#droneImage = Image(Point(250, 250), "drone1.png")
	#droneImage.draw(win)
	c = Circle(Point(50, 50), 10)
	c.draw(win)

	# respond to user hitting red x
	#quit()
	#window.getMouse
	window.close()

main()















