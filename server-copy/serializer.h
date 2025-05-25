#include <iostream>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

using namespace std;

void dumpBuffer(char* buffer, int size);
void send(int socket, int data);
void recv(int socket, int* data);
void send(int socket, double data);
void recv(int socket, double* data);
void send(int socket, char* data);
void recv(int socket, char* data);
void send(int socket, string data);
void recv(int socket, string* data);