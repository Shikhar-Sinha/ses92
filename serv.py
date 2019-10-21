import sys
import socket
#import pcap
import signal
from scapy.all import *
from subprocess import call


class SesServer:
    def __init__(self):
        self.ses_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.server_address = ('0.0.0.0', 9003)
    #
    def listen(self):
        self.ses_socket.bind(self.server_address)
        self.ses_socket.listen(5)
        while True:
            connection, client_address = self.ses_socket.accept()
            data = connection.recv(65000)
            # Process with data here
            m2rtu = call(["./m2rtu", str(data.encode("HEX"))])
            rtu2m = call(["./rtu2m", str(data.encode("HEX"))])
            if m2rtu=="boo" and rtu2m=="boo":
                connection.sendall('False')
            else:
                connection.sendall('True')
    #
    def close(self):
        self.ses_socket.close()

def signal_handler(signal, frame):
    global server
    server.close()
    sys.exit(0)

server = SesServer()

try:
    server.listen()
except KeyboardInterrupt:
    server.close()
