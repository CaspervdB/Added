import socket
import numpy as np

def recvall(sock):
    BUFF_SIZE = 2 #
    data = b''
    print("Waiting for server...")
    i = 0
    while True:
        print("Waiting for server...")
        part = sock.recv(BUFF_SIZE)
        print("Received...")
        data += part
        i+=1
        print(i)
        if len(part) < BUFF_SIZE or len(part) == 0:
            # either 0 or end of data
            break
    return data.decode()

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_address = ("169.254.15.31", 23)
print("Start connection")
s.connect(server_address) 
cmd = "admin\r\n\r\n".encode()
print("Sending command: 'SE8'")
s.sendall(cmd)

s.sendall("SE8\n\r".encode())
# s.sendall("admin".encode())
# s.sendall("".encode())
print("Command sent!")
# res = s.recv(len(cmd))
# print("Start receiving!")
# while True: 
#     res = recvall(s)
#     print("Server response: \r\n" + res)
#     print("169.254.15.31:23/:")
#     userInput = input()
#     if userInput.lower() == 'quit':
#         break
#     s.sendall(userInput.encode())
# while c != '': 
#     res += c
#     c = s.recv(1)
#     print(res.decode())
print("End connection!")
