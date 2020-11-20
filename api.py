#!/usr/bin/python3

import socket
import subprocess

host = ""
port = 8080

def readLine(sock):
	result = bytearray()
	b = sock.recv(1)
	while b != b'\n':
		result.append(b[0])
		b = sock.recv(1)
	result.append(b[0])
	return result.decode('utf-8')

def readHeader(sock):
	header = []
	while len(header) == 0 or header[-1].strip() != "":
		header.append(readLine(sock).strip())
	return header

c = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
c.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
c.bind((host, port))
c.listen(1)

while 1:
	csock, caddr = c.accept()
	req = readHeader(csock)
	if req[0].startswith("GET"):	
		output = 'HTTP/1.0 200 OK\nContent-type: text/html\n\n'
		csock.send(output.encode('utf-8'))
		with open("file.txt", 'r') as fptr:
			for line in fptr:
				csock.send(line.encode('utf-8'))
	elif req[0].startswith("PUT"):
		print(req)
		length = 0
		for line in req:
			if line.startswith("Content-Length:"):
				length = int(line.split(' ')[1])
		print(length)
		with open("log.txt", 'w') as fptr:
			p1 = subprocess.Popen(["./uniq", "file.txt"], stdin=subprocess.PIPE, stdout=fptr)
			n = 0
			while n < length:
				req = csock.recv(4096)
				p1.stdin.write(req)
				n += len(req)
	csock.close()

