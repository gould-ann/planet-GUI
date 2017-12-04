import socket
import sys

########################
# STEP ONE: make the socket
try:
    #                        AF_INET (IPv4)          STREAM (TCP)
    my_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
except socket.error, msg:
    print 'Failed to create socket. Error code: ' + str(msg[0]) + ' , Error message : ' + msg[1]
    sys.exit()
print 'Socket Created'
#######################






def connect_socket(host, port):
    try:
        ip = socket.gethostbyname(host)
        my_socket.connect((ip , port))
        my_socket.settimeout(1)
        print "connected socket to port:", port, "of host", host, "at ip:", ip
    except socket.gaierror:
        #could not resolve
        print 'Hostname could not be resolved. Exiting'
    




def send_data(message):
    try :
        #Set the whole message
        my_socket.sendall(message)
    except socket.error:
        #Send failed
        print 'Send failed'
        sys.exit()
    print 'Message send successfully'

def get_data():
    try:
        reply = my_socket.recv(1)
        return reply
    except socket.timeout:
        return ""



# connect_socket("192.168.1.1", 42880)

#Send some data to remote server 
# send_data("GET / HTTP/1.1\nHost: stackoverflow.com\n\n")

#Now receive data
# while 1:
#     sys.stdout.write(get_data())


# can only send one char at a time
# send_data("z")



