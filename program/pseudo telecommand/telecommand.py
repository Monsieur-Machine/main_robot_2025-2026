#!/usr/bin/env python3

import socket
import struct

def start_udp_server():
    udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    udp_socket.bind(('0.0.0.0', 12345))

    print("Serveur UDP démarré sur le port 12345")

    while True:
        data = udp_socket.sendto()
"""
        if len(data) == 2:
            packet_num, instruction = struct.unpack('BB', data)
            print(f"Reçu: packet_number={packet_num}, instruction={instruction}")
"""

if __name__ == '__main__':
    start_udp_server()

#ulysse le femboy 