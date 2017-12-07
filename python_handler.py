import sys
import socket
import time

import sockets

sockets.connect_socket("192.168.1.1", 42880)

# # clear history
history_file = open("roomba_history.txt", "w")
history_file.write("")
history_file.close()

while 1:

	# send input
	f = open("roomba_input.txt", "r")
	lines = "".join([line for line in f])
	f.close()
	if lines != "":
		print lines[0]
		sockets.send_data(lines[0])
		j = open("roomba_input.txt", "w")
		j.write("")
		j.close()
		time.sleep(1)
	else:
		# get output
		i = open("roomba_history.txt", "a")
		char = sockets.get_data()
		print "char is: " + char
		if char == "\n":
			i.write("<br>")
			time.sleep(2)
		i.write(char)

		if char == "":
			i.write("")
		i.close()
