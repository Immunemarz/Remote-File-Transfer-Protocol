#include<serializer.h>

using namespace std;

void dumpBuffer(char* buffer, int size)
{
	for (int i=0; i<size; i++)
	{
		cout << "[" << (int)buffer[i] << "]";
	}
	cout << endl;
	cout << "-------------------------------" << endl;
}

void send(int socket, int data)
{
	send(socket,&data,sizeof(int),0);
}

void recv(int socket, int* data)
{
	recv(socket, data, sizeof(int), 0);
}

void send(int socket, double data)
{
	send(socket,&data,sizeof(double),0);
}

void recv(int socket, double* data)
{
	recv(socket, data, sizeof(double), 0);
}

void send(int socket, bool data)
{
	send(socket,&data,sizeof(bool),0);
}

void recv(int socket, bool* data)
{
	recv(socket, data, sizeof(bool), 0);
}

void send(int socket, char* data)
{
	int size = strlen(data)+1;
	send(socket,size); // send size of c-string FIRST
	send(socket,data,size,0);
}

void recv(int socket, char* data)
{
	int size=0;
	recv(socket,&size); // get size of c-string FIRST
	recv(socket, data, size, 0);
}

void send(int socket, string data)
{
	char * cstr = new char [data.length()+1];
	strcpy (cstr, data.c_str());
	send(socket,cstr);
}

void recv(int socket, string* data)
{
	int size=0;
	recv(socket,&size); // get size of c++ string FIRST
	char * cstr = new char [size];
	recv(socket, cstr, size, 0);
	*data = cstr;
}
