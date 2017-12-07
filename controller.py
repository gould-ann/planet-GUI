import pygame


# clears the file then writes data to it
def write_to_file(data):
    f = open("roomba_input.txt", "w")
    f.write(data)
    f.close()

class PS4Controller(object):

    def init(self):
        """Initialize the joystick components"""
        
        pygame.init()
        pygame.joystick.init()
        self.controller = pygame.joystick.Joystick(0)
        self.controller.init()

    def listen(self):
        """Listen for events to happen"""

        while True:
            for event in pygame.event.get():
                if event.type == pygame.JOYBUTTONDOWN:
                    #print(event)
                    if(event.button == 4):
                        print('up')
                        #s.sendall('w'.encode())
                        write_to_file('u')
                    elif(event.button == 6):
                        print('down')
                        #s.sendall('b'.encode())
                        write_to_file('d')
                    elif(event.button == 5):
                        print('right')
                        #s.sendall('d'.encode())
                        write_to_file('r')
                    elif(event.button == 7):
                        print('left')
                        #s.sendall('s'.encode())
                        write_to_file('l')
                    elif(event.button == 15):
                        print('sweep')
                        #s.sendall('p'.encode())
                        write_to_file('s')



if __name__ == "__main__":
    #s = socket.socket()
    #s.settimeout(5)   # 5 seconds
    #s.connect(('192.168.1.1', 42880)) 
    #print('connected')

    ps4 = PS4Controller()
    ps4.init()
    ps4.listen()