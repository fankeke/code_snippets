----------------server.py-----------------------
#!/usr/bin/env python

import socket
import select
import sys


serv_addr=('',12346)
buf_size=8096

class chat_ser():
    def __init__(self):
	self.listen_sock=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
	self.listen_sock.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1)
	self.listen_sock.bind(serv_addr)
	self.listen_sock.listen(10)
	self.read_socks=[self.listen_sock]
	self.client_list={}
	self.count=0
	print 'listen port:12346, waitting for connect...'

    def run(self):
	try:
	    while 1:
	    	rsocks,wsocks,esocks=select.select(self.read_socks,[],[])
	    	for sock in rsocks:
		    if sock==self.listen_sock:
		   	self.client_accept()
		    else:
		    	self.handle_message(sock)
	except KeyboardInterrupt:
	    print 'ctrl-c has pressed'
	    sys.exit(1)

    def client_accept(self):
	client,addr=self.listen_sock.accept()
	client_id='#%d' % self.count
	message='welcome to chatroom,you ID is %s' % client_id
	self.client_list[client]=client_id
	self.count+=1
	client.send(message)
	message='client %s join room' % client_id
	print message
	self.read_socks+=[client]

	for _client in self.client_list:
	    message='client %s join room' % client_id
	    if _client is not client:
	    	_client.send(message)
	
    def broadcast_client(self,sock,message):
	for client in self.client_list:
	    if client is not sock:
		client.send(message)


    def handle_message(self,sock):
	message=sock.recv(buf_size)
	if not message:
	    self.client_leave(sock)
	else:
	    message=self.client_list[sock]+':'+message
	    print message
#	    print '%s:%s' %(self.client_list[sock],message)
	    self.broadcast_client(sock,message)

    def client_leave(self,sock):
	message='client %s leaved chat room' % self.client_list[sock]
	print message
	self.broadcast_client(sock,message)
	del self.client_list[sock]
	self.read_socks.remove(sock)
	sock.close()


if __name__=='__main__':
    chat_s=chat_ser()
    chat_s.run()
	    
-----------------------------client.py-----------------------------
#!/usr/bin/env python

import socket
import select
import sys,os


serv_addr=('104.224.131.4',12346)
buf_size=8096

class chat_client():
    def __init__(self):
	self.client_sock=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
	self.client_sock.connect(serv_addr)
	message=self.client_sock.recv(buf_size)
	print 'connect server success!'
	print message
	self.read_socks=[self.client_sock,sys.stdin]
	self.client_sock.setblocking(0)
    def run(self):
	try:
	    while 1:
		rsocks,_,_=select.select(self.read_socks,[],[])
		for sock in rsocks:
		    if sock==self.client_sock:
		    	message=self.client_sock.recv(buf_size)
		    	if not message:
			    print 'server is closeing....'
			    self.client_sock.close()
			    return
		    	print message
		    if sock==sys.stdin:
		    	message=raw_input()
			if not message:
			    print 'you are leaving .....'
			    self.client_sock.close()
			    return 
		    	self.client_sock.send(message)
	except KeyboardInterrupt:
	    sys.exit(0)

	    

if __name__=='__main__':
    chat_c=chat_client()
    chat_c.run()
	    
