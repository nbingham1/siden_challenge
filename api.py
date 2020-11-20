#!/usr/bin/python3

import socket
import subprocess

host = ""
port = 8080

# read a line in the http header from a socket
# This specifically looks for the new-line byte
def readLine(sock):
	result = bytearray()
	b = sock.recv(1)
	while b != b'\n':
		result.append(b[0])
		b = sock.recv(1)
	result.append(b[0])
	return result.decode('utf-8')

# read the http headers into a list from a socket
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
	# wait for a request
	csock, caddr = c.accept()

	# read the headers of the request
	req = readHeader(csock)

	# this implements the API
	if req[0].startswith("GET"):
		# GET should return the uniqued file	
		output = 'HTTP/1.0 200 OK\nContent-type: text/html\n\n'
		csock.send(output.encode('utf-8'))
		# stream the file through the socket
		with open("file.txt", 'r') as fptr:
			for line in fptr:
				csock.send(line.encode('utf-8'))
	elif req[0].startswith("PUT"):
		# PUT should save the uniqued file to the disk
		# Figure out the length of the input file from the http header
		length = 0
		for line in req:
			if line.startswith("Content-Length:"):
				length = int(line.split(' ')[1])

		# read the file onto disk, piping it through our C++ uniq utility
		with open("log.txt", 'w') as fptr:
			p1 = subprocess.Popen(["./uniq", "file.txt"], stdin=subprocess.PIPE, stdout=fptr)
			n = 0
			while n < length:
				req = csock.recv(4096)
				p1.stdin.write(req)
				n += len(req)
	
	# clean up the connection	
	csock.close()

