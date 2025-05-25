#include <iostream>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <serializer.h>

using namespace std;

int main(void)
{
	// Create socket
	int s = socket(PF_INET, SOCK_STREAM, 0);

	if (s <= 0)
	{
		cerr << "server:main(): Socket creation failed." << endl;
		return -1;
	}

	// Do the bind a PORT to the socket
	sockaddr_in address;
  int addressSize = sizeof(sockaddr);
  address.sin_family=AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port  = htons(15000); // <---------------- port is 15000

	int status = bind(s, (struct sockaddr*) &address, addressSize);

	// Wait for client to connect using listen
	cout << "server: Waiting for client to connect..." << endl;
	listen(s,3); // up to 3 clients can connect

	// Accept returns a NEW socket for the client we will talk to
	int cs = accept(s, (struct sockaddr*) &address, (socklen_t *) &addressSize);

	cout << "server: Server and client established communication." << endl;

	// Create a buffer to put messages in
	const int BUFFER_SIZE = 1024;
	char buffer[BUFFER_SIZE];

	do
	{
		cout << "server: Enter message and press <ENTER>: ";
		cout.flush();
		cin.getline(buffer,BUFFER_SIZE);
		dumpBuffer(buffer,BUFFER_SIZE);

		// send message to client!
		send(cs,buffer,BUFFER_SIZE,0);

		// receive message from client!
		cout << "server: Waiting for client message." << endl;
		recv(cs, buffer, BUFFER_SIZE, 0);
		cout << "server: client sent ->" << buffer << "<-" << endl;
		dumpBuffer(buffer,BUFFER_SIZE);

		int data = 42;
		cout << "server: Sending int to client..." << endl;
		send(cs,data);

		double dataDouble = 3.14;
		cout << "server: Sending double to client..." << endl;
		send(cs,dataDouble);

	} while (true);

	close(cs);
	close(s);

	return 1;
}