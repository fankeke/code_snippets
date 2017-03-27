cc=gcc
link=$(cc)
cflags=-Wall -O0 -g

incs=-I ./
deps=

obj=redis.o \
    adlist.o \
    ae.o \
    anet.o  \
    ae_epoll.o \
    networking.o

exe=redis

all:$(exe)

$(exe):$(obj)
	$(link) -o $(exe) $(obj)

redis.o:$(deps) redis.h redis.c
	$(cc)	-c $(cflags) $(incs) -o redis.o redis.c

ae.o:$(deps) ae.h ae.c 
	$(cc)	-c $(cflags) $(incs) -o ae.o ae.c 

anet.o:$(deps) anet.h anet.c
	$(cc)	-c $(cflags) $(incs) -o anet.o anet.c

adlist.o:$(deps) adlist.h adlist.c
	$(cc)	-c $(cflags) $(incs) -o adlist.o adlist.c

ae_epoll.o:$(deps) ae_epoll.h ae_epoll.c
	$(cc)	-c $(cflags) $(incs) -o ae_epoll.o ae_epoll.c

networking.o:$(deps) networking.h networking.c
	$(cc)	-c $(cflags) $(incs) -o networking.o networking.c

clean:
	rm -rf $(obj) $(exe)


