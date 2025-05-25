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
	int cs = socket(PF_INET, SOCK_STREAM, 0);

	if (cs <= 0)
	{
		cerr << "client:main(): Socket creation failed." << endl;
		return -1;
	}

	// Connect to server by specifying ip address and port
	sockaddr_in address;
  int addressSize = sizeof(sockaddr);
  address.sin_family=AF_INET;
  address.sin_port  = htons(15000); // <---------------- port is 15000
	inet_pton(AF_INET,"127.0.0.1",&address.sin_addr);

	// Connect
	int status = connect(cs, (struct sockaddr*) &address, addressSize);

	cout << "client: client and server established communication." << endl;

	// Create a buffer to put messages in
	const int BUFFER_SIZE = 1024;
	char buffer[BUFFER_SIZE];

	do
	{
		// receive message from server!
		cout << "client: Waiting for server message." << endl;
		recv(cs, buffer, BUFFER_SIZE, 0);
		cout << "client: server sent ->" << buffer << "<-" << endl;

		cout << "client: Enter message and press <ENTER>: ";
		cout.flush();
		cin.getline(buffer,BUFFER_SIZE);

		// send message to server!
		send(cs,buffer,BUFFER_SIZE,0);

		int data = 0;
		cout << "client: Waiting for int from server." << endl;
		recv(cs, &data);
		
		cout << "client: int received is: " << data << endl;
		double dataDouble = 0;

		cout << "client: Waiting for double from server." << endl;
		recv(cs, &dataDouble);
		cout << "client: double received is: " << dataDouble << endl;

	} while (true);

	close(cs);

	return 1;
}