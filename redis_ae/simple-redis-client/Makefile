cflag= -Wall -g -O0
cc=gcc
link=$(cc)

exe=redis-cli
obj=redis-cli.o anet.o sds.o

deps=  utils.h
incs=-I ./

all:$(obj)
	$(link)	-o  $(exe) $(obj)

redis-cli.o:$(deps) redis-cli.c
	$(cc)	-c $(cflag) $(incs) -o redis-cli.o redis-cli.c
anet.o:$(deps) anet.c
	$(cc)	-c $(cflag) $(incs) -o anet.o anet.c
sds.o: $(deps) sds.c
	$(cc)	-c $(cflag) $(incs) -o sds.o sds.c

clean:
	rm -rf $(obj) $(exe)

