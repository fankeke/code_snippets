summary_path=./summary
encry_path=./crypte
encry_obj=$(encry_path)/gsl_cdf.h $(encry_path)/gsl_sf_gamma.h \
	  $(encry_path)/check.c 
lib= -lpcap  -lgsl -lgslcblas -lens
obj=datalink.o flow.o  tcpip.o util.o print.o summary.o \
    encrypt.o tcpflow.h main.c
main:$(obj)
	gcc -o tcpflow	$(obj) $(lib) 
datalink.o:tcpflow.h
flow.o:tcpflow.h
tcpip.o:tcpflow.h
util.o:tcpflow.h
print.o:tcpflow.h
encrypt.o:
	gcc -c encrypt.c $(encry_obj) 
summary.o:
	gcc -c $(summary_path)/summary.c

clean:
	rm -rf *.o tcpflow 
    	
