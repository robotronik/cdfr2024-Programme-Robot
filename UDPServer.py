#!/usr/bin/env python
# -*- coding: utf-8 -*-

#Libraries
import socket    #https://wiki.python.org/moin/UdpCommunication
import sys
#Parameters
localPort=8888
bufferSize=1024

#Objects
sock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)  ## Internet,UDP
# function init 
def init():
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEPORT, 1)
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1) #enable broadcasting mode
    sock.bind(('', localPort))
    print("UDP server : {}:{}".format(get_ip_address(),localPort))
    start_time=sys.argv[1]
    team=sys.argv[2]
    return start_time,team
# function main 
def main():
    start_time,team=init()
    while True:
        data, addr = sock.recvfrom(1024) # get data
        #print("received message: {} from {}\n".format(data,addr))
        
        sock.sendto(str(start_time)+":"+str(team),addr)  # write data
  

# function get_ip_address 
def get_ip_address():
    """get host ip address"""
    ip_address = '';
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.connect(("8.8.8.8",80))
    ip_address = s.getsockname()[0]
    s.close()
    return ip_address




if __name__ == '__main__':
    main()
