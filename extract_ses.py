# Michael Millian 2019

import sys
import socket
import base64
from scapy.all import *

def main():
    pkts = rdpcap(sys.argv[1],10)
    count = 0
    for pkt in pkts:
        count += 1
        if pkt.haslayer("TCP"):
            tcp=pkt['TCP']
            if tcp.haslayer("Raw"):
                data=str(tcp['Raw'])
                data=data[0:-6]
                

                #print str(count) + ":\t" + str(data).encode("HEX")
                hammer_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                #hammer_address = (parser_container_ip, DOCKER_PORT)
                hammer_address = ('172.17.0.1', 9003)
                hammer_socket.connect(hammer_address)
                hammer_socket.sendall(data)
                response = hammer_socket.recv(65535)
                print response
                hammer_socket.close()


if __name__ == '__main__':
    main()
