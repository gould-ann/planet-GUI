import sys
import socket

import sockets

sockets.connect_socket("192.168.1.1", 42880)

while 1:
	f = open("roomba_output.txt", "a")
	char = sockets.get_data()
	if char == "\n":
		f.write("<br>")
	f.write(char)