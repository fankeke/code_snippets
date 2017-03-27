exe=list
cc=gcc
link=$(cc)
cflag=-Wall -O0 -g

incs=-I ./
deps=adlist.h


obj=adlist.o list.o

$(exe):$(obj)
	$(link) -o $(exe) $(obj)

adlist.o:$(deps) adlist.c
	$(cc)	-c $(cflag) $(incs) -o adlist.o adlist.c

list.o: $(deps) list.c
	$(cc)	-c $(cflag) $(incs) -o list.o list.c
clean:
	rm -rf $(obj) $(exe)

