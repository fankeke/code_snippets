info_path=./info_file
encry_path=./crypte
print_path=./print

lib= -lpcap  -lgsl -lgslcblas -lens

obj=datalink.o flow.o  tcpip.o util.o print.o summary.o \
    encrypt.o  conver.o main.c



main:$(obj)
	gcc -o tcpflow	$(obj) $(lib) 

datalink.o:tcpflow.h
flow.o:tcpflow.h
tcpip.o:tcpflow.h
util.o:tcpflow.h
encrypt.o:
	gcc -c -o encrypt.o  $(encry_path)/check.c
summary.o:
	gcc -c $(info_path)/summary.c
conver.o:
	gcc -c $(info_path)/conver.c
print.o:
	gcc -c $(print_path)/print.c

clean:
	rm -rf *.o tcpflow 
    	
