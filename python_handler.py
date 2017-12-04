import sys
import socket
import time

import sockets

sockets.connect_socket("192.168.1.1", 42880)

while 1:

	# send input
	f = open("roomba_input.txt", "r")
	lines = "".join([line for line in f])
	f.close()
	if lines != "":
		print lines
		sockets.send_data(lines[0])
		j = open("roomba_input.txt", "w")
		j.write("")
		j.close()
		time.sleep(1)
	else:
		# get output
		i = open("roomba_output.txt", "a")
		char = sockets.get_data()
		print "char is: " + char
		if char == "\n":
			i.write("<br>")
		i.write(char)
		i.close()
