cc=gcc
cflags=-Wall -O0 -g
link=$(cc)


obj=sv_main.o sv_core.o sv_conn.o
exe=svhttp


deps=sv_config.h sv_core.h sv_conn.h
incs= -I ./

$(exe): $(obj)
	$(link)	-o $(exe) $(obj)

sv_main.o:$(deps) sv_main.c
	$(cc)	-c $(cflags) $(incs) -o sv_main.o sv_main.c

sv_core.o:$(deps) sv_core.c
	$(cc)	-c $(cflags) $(incs) -o sv_core.o sv_core.c

sv_conn.o:$(deps) sv_conn.c
	$(cc)	-c $(cflags) $(incs) -o sv_conn.o sv_conn.c

clean:
	rm -rf $(obj) $(exe)


