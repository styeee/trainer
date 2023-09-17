import socket
import sys
import os

port=8888
ip="localhost"

if(len(sys.argv)>1):
	try:
		port=int(sys.argv[1])
	except:
		print("error: input parametr must been a number")

if(len(sys.argv)>2):
	try:
		port=int(sys.argv[1])
	except:
		print("error: input parametr must been a number")

s=socket.socket()
s.connect((ip,port))

m=bytearray()
b=s.recv(1)

while b!=b'\x00':
	m=m+b
	b=s.recv(1)
	
print(m.decode())

while True:
	seq=input().encode()
	s.send(seq)

s.close()