#!/usr/bin/env python


""""
一个简单的中转件，没有解析DNS的功能，所以需要指定中转的目的ip和端口,而且没有考虑阻塞问题，属于比较粗暴的收发方式
下一步，加入DNS解析功能,这样proxy可以根据域名来指定服务器
"""

import socket
import select
import sys

proxy_addr=('192.168.1.106',80)

class proxy():
    def __init__(self,addr):
	self.listen_sock=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
	self.listen_sock.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1)
	self.listen_sock.bind(addr)
	self.listen_sock.listen(10)
	print 'listen in:',addr
	self.read_socks=[self.listen_sock]
	self.routes={}

    def run(self):
	while 1:
	    rsocks,_,_=select.select(self.read_socks,[],[])
	    for sock in rsocks:
		if sock==self.listen_sock:
		    self.accept_client()
		else:
		    data=sock.recv(8096)
		    if not data:
			self.disconnect(sock)
		    else:
			self.routes[sock].send(data)

    def accept_client(self):
	client,addr=self.listen_sock.accept()
	print addr,'conncted'
	upstream_sock=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
	upstream_sock.connect(proxy_addr)
	self.read_socks+=[client,upstream_sock]
	self.routes[client]=upstream_sock
	self.routes[upstream_sock]=client

    def disconnect(self,sock):
	self.read_socks.remove(sock)
	del self.routes[sock]
	sock.close()


if __name__=='__main__':
    pro=proxy(('',12345))
    try:
    	pro.run()
    except KeyboardInterrupt:
    	sys.exit(1)






