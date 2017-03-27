#!/usr/bin/env python


PORT=8388
KEY="barfoo!"


import socket
import select
import string
import struct 
import hashlib
import threading
import time
import SocketServer



def get_table(key):
	m=hashlib.md5()
	m.update(key)
	s=m.digest()
	(a,b)=struct.unpack('<QQ',s)
	table=[c for c in string.maketrans('','')]
	for i in xrange(1,1024):
		table.sort(lambda x,y:int(a%(ord(x)+i)-a%(ord(y)+i)))
	return table







class ThreadingTCPServer(SocketServer.ThreadingMixIn,SocketServer.TCPServer):
	pass


class Socks5Server(SocketServer.StreamRequestHandler):
	def encrypt(self,data):
		return data.translate(encrypt_table)

	def decrypt(self,data):
		return data.translate(decrypt_table)

	def handle_tcp(self,sock,remote):
		try:
			fdset=[sock,remote]
			while True:
				r,w,e=select.select(fdset,[],[])
				if sock in r:
					if remote.send(self.decrypt(sock.recv(4096)))<=0:
						break
				if remote in r:
					if sock.send(self.encrypt(remote.recv(4096)))<=0:
						break
		finally:
			remote.close()


	def send_encrpyt(self,sock,data):
		sock.send(self.encrypt(data))
	
	def handle(self):
		try:
			print 'socks connection from ', self.client_address
			sock=self.connection
			addrtype=ord(self.decrypt(sock.recv(1)))
			if addrtype==1:
				addr=socket.inet_ntoa(self.decrypt(self.rfile.read(4)))
			elif addrtype==3:
				addr=self.decrypt(self.rfile.read(ord(self.decrypt(sock.recv(1)))))
			else:
				print 'server:not support'
				return
			port=struct.unpack('>H',self.decrypt(self.rfile.read(2)))

			try:
				print 'Tcp connecint to:',addr,port[0]
				remote=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
				remote.connect((addr,port[0]))
				lcoal=remote.getsockname()
			except socket.error:
				return

			self.handle_tcp(sock,remote)
		except socket.error as e:
			print 'socket error'



def main():
    server=ThreadingTCPServer(('',PORT),Socks5Server)
    server.allow_reuse_address=True
    print 'Starting server at port %d' % PORT
    server.serve_forever()

if __name__=='__main__':
    encrypt_table=''.join(get_table(KEY))
    decrypt_table=string.maketrans(encrypt_table,string.maketrans('',''))
    main()
    
	
	
	
