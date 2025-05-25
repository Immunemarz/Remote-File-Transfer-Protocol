CC = g++ 
CPPFLAGS = -I. -g
LDFLAGS = -Wl,--no-as-needed
all: ftpserver
ftpserver: ftpserver.o serializer.o 
ftpserver.o: ftpserver.cpp serializer.h
serializer.o: serializer.cpp serializer.h
clean:
	rm ftpserver
	rm *.o 