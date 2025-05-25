CC = g++
CPPFLAGS = -I. -g
all: ftpserver
ftpserver: ftpserver.o serializer.o 
ftpserver.o: ftpserver.cpp serializer.h
serializer.o: serializer.cpp serializer.h
clean:
	rm ftpserver
	rm *.o 