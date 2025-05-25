CC = g++
CPPFLAGS = -I. -I../server -g
all: ftp
ftp: ftp.o ../server/serializer.o 
ftp.o: ftp.cpp ../server/serializer.h
../server/serializer.o: ../server/serializer.cpp ../server/serializer.h
clean:
	rm ftp
	rm *.o 