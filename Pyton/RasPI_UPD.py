import socket
import time

buffersize = 1024
msgFromServer = "It is server!"
ServerPort = 2222
ServerIP = '123.123.12.12'
bytesToSend = msgFromServer.encode('utf-8')
RasPIsocket = socket.socket(socket.AF_INET, socket.SOC_DGRAM)
RasPIsocket.bind((ServerIP, ServerPort))
print("Server listening . . .")
message, address = RasPIsocket.recvform(buffersize)
message.decode('utf-8')
print(f"{message} from {address[0]}.")
RasPIsocket.sendto(bytesToSend, address)

