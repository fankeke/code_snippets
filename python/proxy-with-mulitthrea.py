"""
Using mutlithread to impletement proxy, every conn and remote paris are handled by a thread. so it's sucks ...
besides,we don't consideration the keepalive attruibtute for evenry connection,  once  request is sent and  response  is  returnn 
,we close two sides ... 
so much work to be done to imp lemte a  real proxy
"""
#!/usr/bin/env python
import socket
import select
import sys
import thread

maxsize=8192

usage=(
	'usage:python xxx port'
	'eg:python xxx 12345'
      )

class proxy():
    def __init__(self,addr,port):
	self.listen_sock=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
	self.listen_sock.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1)
	self.listen_sock.bind((addr,port))
	self.listen_sock.listen(1024)


    def run(self):
	while 1:
	    conn,addr=self.listen_sock.accept()
	    thread.start_new_thread(self.proxy_handler,(conn,addr))
	pass

    def proxy_handler(self,conn,addr):
	def get_req_info(data):
	    first_line=data.split('\r\n')[0]
	    host=first_line.split(' ')[1]
	    url_pos=host.find('://')
	    host=host[url_pos+3:]
	    url_end=host.find('/')
	    host=host[:url_end]
	    port_pos=host.find(':')
	    if port_pos==-1:
		port=80
	    else:
	 	port=int(host[port_pos+1:-1])
		host=host[:port_pos]
	    return host,port
	    	
	def cleanup(conn,remote):
	    if conn:
	    	conn.close()
	    if remote:
		remote.close()
	    sys.exit()

	print 'connecting from',addr
	data=conn.recv(maxsize)
    	host,port=get_req_info(data)
    	print 'req:',data
    	print 'host:',host
    	
    	remote=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
    	try:
    	    remote.connect((host,port))
    	except socket.error as e:
	    print e
	    cleanup(conn,remote)

    	remote.sendall(data)
	while 1:
	    data=remote.recv(maxsize)
    	    if len(data)==0:
	    	break
	    conn.send(data)
    		
    	cleanup(conn,remote)


def main():
    args=sys.argv
    if len(args)==2:
    	port=int(args[1])
    else:
    	print usage
    	sys.exit()
    thread_proxy=proxy((''),port)
    thread_proxy.run()

if __name__=='__main__':
    main()
