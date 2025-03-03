import socket

buffersize = 1024
msgFromClient = "It is client!"
serverAdress = ('123.123.12.12', 2222)
bytesToSend = msgFromClient.encode('utf-8')
UDPClient = socket.socket(socket.AF_INET, socket.SOC_DGRAM)
UDPClient.sendto(bytesToSend, serverAddress)
data, address = UDPClient.recvform(buffersize)
data.decode('utf-8')
print(f"{message} from {address[0]} in potr {address[1]}.")

