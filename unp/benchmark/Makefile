cc=gcc
link=$(cc)
cflags=-Wall -O0 -g

deps=ben_core.h ben_config.h ben_socket.h
incs=-I ./

obj=ben_core.o ben_socket.o 
exe=benmark

$(exe):$(obj)
	$(link) -o $(exe) $(obj)

ben_core.o:$(deps) ben_core.c
	$(cc)	-c $(cflags) $(incs) -o ben_core.o ben_core.c

ben_socket.o:$(deps) ben_socket.c
	$(cc)	-c $(cflags) $(incs) -o ben_socket.o ben_socket.c

clean:
	rm -rf $(exe) $(obj)
