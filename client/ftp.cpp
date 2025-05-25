#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <dirent.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include <serializer.h>
#include <vector>

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

	bool loginBool;
	string userInput;
	do
	{
	
	cout << "Client: enter COMMAND and hit enter. " << endl;
	cout.flush();
	cin.getline(buffer,BUFFER_SIZE);
	string originalString = buffer;
    

  istringstream iss(originalString);
    
	string firstWord;

	string secondWord;
  
	


 
  iss >> firstWord;

	if(firstWord == "user"){
		cout << "sending user over. " << endl;
		send(cs,buffer,BUFFER_SIZE,0);
	}
	if(firstWord == "password")
	{
		cout << "sending password over. " << endl;
		send(cs,buffer,BUFFER_SIZE,0);
	}

	


	
	recv(cs, &loginBool);

	


	if(loginBool == false){
		cout << "You MUST login to access commands. Type <help> for a list of commands and <0> to quit. " << endl;

	}
	else if(loginBool == true)
	{
		cout << " You have successfully logged in. Welcome in. " << endl;
	}


	}while(loginBool == false);







	do
	{

	cout << "Client: enter COMMAND and hit enter. " << endl;
	cout.flush();
	cin.getline(buffer,BUFFER_SIZE);
	string originalString = buffer;
    

  istringstream iss(originalString);
    
	string firstWord;

	string secondWord;
  
	


 
  iss >> firstWord;
	cout << "sending command: " << firstWord << " over." <<endl;
	
	send(cs,buffer,BUFFER_SIZE,0);


	if(firstWord == "help")
	{
		string receivedInput;
		recv(cs, &receivedInput);
		cout << receivedInput << endl;
	}


	else if(firstWord == "dir")
	{
		string receivedInput;
		recv(cs,&receivedInput);
		cout << receivedInput << endl;
	}

	else if(firstWord == "cd")
	{
		cout << "sending cd over. " << buffer << endl;
		//send(cs,buffer);
		string changeDirStatus;
		recv(cs,&changeDirStatus);
		cout << changeDirStatus << endl;
	}

	else if(firstWord == "lcd")
	{
		iss >> secondWord;
		cout << " the second word is: " << secondWord <<endl;
		const char* newDirectory = secondWord.c_str();
		// //cout << newDirectory << " <- this is the new directory" << endl;
		if (chdir(newDirectory) == 0) 
		 {
      
		 	//string success = "success! changed directory to: " + secondWord;
			cout << "Current directory changed to: " << secondWord << endl;
		 	//send(cs,success);
		 }
		else{
		 	cout << "Failure. cannot chang directory to: " << secondWord << endl;
		 	//send(cs,failure);
		 }
	}

	else if(firstWord == "del")
	{
		cout << "sending del over. " << buffer << endl;
		//send(cs,buffer);
		string deleteStatus;
		recv(cs,&deleteStatus);
		cout << deleteStatus << endl;
	}

	else if(firstWord == "get")
	{
		iss >> secondWord;
		cout << "sending COMMAND get over. " << secondWord << endl;


		size_t dataSize = 0;
    recv(cs, &dataSize, sizeof(dataSize), 0);
		cout << dataSize << endl;
	//	vector<char> bytesVector;
		vector<char> nextSetOfBytes;
		int nextSetReceived = 0;
		while(nextSetOfBytes.size() < dataSize) 
		{
	  int bytes_received = recv(cs, buffer, sizeof(buffer), 0);
    nextSetOfBytes.insert(nextSetOfBytes.end(), buffer, buffer + bytes_received);
		}

		cout << nextSetOfBytes.size() << " <-next set of bytes size is: " << endl;
		const char* fileToDelete = secondWord.c_str();
		char currentDirectory[PATH_MAX];
		string getWorkingDirectory = getcwd(currentDirectory, sizeof(currentDirectory));
		string getFullPathAndDirectory = getWorkingDirectory + "/" + fileToDelete;
		ofstream destinationFile(getFullPathAndDirectory,ios::binary);
		//help from internet here as well cause of the error. Can explain everything however
		if (destinationFile.is_open()) {
        destinationFile.write(nextSetOfBytes.data(), nextSetOfBytes.size());
				destinationFile.close();
        cout << "Data written to: " << getFullPathAndDirectory << endl;
				
    } else {
        cerr << "Failed to open the file for writing." << endl;
    }





	}



	else if(firstWord == "put")
	{
		iss >> secondWord;
		char currentDirectory[PATH_MAX];
		cout << getcwd(currentDirectory, sizeof(currentDirectory)) << endl;
		const char* fileToDelete = secondWord.c_str();
		string getWorkingDirectory = getcwd(currentDirectory, sizeof(currentDirectory));
		string getFullPathAndDirectory = getWorkingDirectory + "/" + fileToDelete;
		cout << getFullPathAndDirectory << endl;
		ifstream jpgfile(getFullPathAndDirectory,ios::binary);
		vector<char> jpgbytes((istreambuf_iterator<char>(jpgfile)),istreambuf_iterator<char>());

		
		for(int i = 0; i < jpgbytes.size(); ++i)
		{
			//cout << jpgbytes[i]; //works
		}
		size_t dataSize = jpgbytes.size();
		cout << "datasize is: " << dataSize << endl;
    //got help a little from internet here cause I was getting a weird error for like 30 min
    send(cs, &dataSize, sizeof(dataSize), 0); 
    if (dataSize > 0) {
        int numberSent = send(cs, jpgbytes.data(), dataSize, 0); 
				cout << "jpgbytes.data() is: " << numberSent << endl;
    }
	}

	else
	{
		string fail;
		recv(cs,&fail);
		cout << fail << endl;
	}
	
	


	}while(loginBool == true);

	close(cs);

	return 1;
}