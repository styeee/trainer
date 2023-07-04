import socket
import sys

port=8888

if(len(sys.argv)>1):
	try:
		port=int(sys.argv[1])
	except:
		print("error: input parametr must been a number")

s=socket.socket()
s.connect(("localhost",port))

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