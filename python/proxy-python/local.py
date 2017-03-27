#!/usr/bin/env python


SERVER='127.0.0.1'
REMOTE_PORT=8388
PORT=1080
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

	def send_encrypt(self,sock,data):
		sock.send(self.encrypt(data))

	def handle_tcp(self,sock,remote):
		try:
			fdset=[sock,remote]
			while True:
				r,w,e=select.select(fdset,[],[])
				if sock in r:
					if remote.send(self.encrypt(sock.recv(4096)))<=0:
						break
				if remote in r:
					if sock.send(self.decrypt(remote.recv(4096)))<=0:
						break
		finally:
			remote.close()

		
	def handle(self):
		try:
			print 'socks connection from ',self.client_address
			sock=self.connection
			sock.recv(262)
			sock.send("\x05\x00")
			data=self.rfile.read(4)
			mode=ord(data[1])

			if mode!=1:
				print 'mode!=1'
				return
				
			addrtype=ord(data[3])
			addr_to_send=data[3]
			if addrtype==1:
				addr_ip=self.rfile.read(4)
				addr=socket.inet_ntoa(addr_ip)
				addr_to_send+=addr_ip
			elif addrtype==3:
				addr_len=sock.recv(1)
				addr=self.rfile.read(ord(addr_len))
				addr_to_send+=addr_len+addr
			else:
				print 'not support addr type'
				return
			addr_port=self.rfile.read(2)
			addr_to_send+=addr_port
			port=struct.unpack('>H',addr_port)

			try:
				if mode==1:
					reply="\x05\x00\x00\x01"
					reply+=socket.inet_aton('0.0.0.0')+struct.pack(">H",2222)
					sock.send(reply)
					remote=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
					remote.connect((SERVER,REMOTE_PORT))
					self.send_encrypt(remote,addr_to_send)
					print 'Tcp connect to',addr,port[0]
				else:
					print 'command not supported'
					return
			except socket.error as e:
				print 'socket error:'+str(e)
				return
			self.handle_tcp(sock,remote)
		except socket.error as e:
			print 'socket error '+str(e)




def main():
    server=ThreadingTCPServer(('',PORT),Socks5Server)
    server.allow_reuse_address=True
    print 'Starting proxy at port %d' % PORT
    server.serve_forever()

if __name__=='__main__':
	encrypt_table=''.join(get_table(KEY))
	decrypt_table=string.maketrans(encrypt_table,string.maketrans('',''))
	main()
    
	
	
	
