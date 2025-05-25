#include <iostream>
#include <cstring>
#include <dirent.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <sstream>
#include <vector>
#include <fstream>
#include <unistd.h>
#include <serializer.h>


using namespace std;



string* tempuser;
string* tempPass;
bool userBool = false;
bool passBool = false; 
bool loginBool = false;
int main(void)
{
	string username = "ryan";
	string password = "icecream";


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

	cout << "server: Waiting for COMMAND: " << endl;
	
	//cout << "server: COMMAND: User received: " << endl;






	recv(cs,buffer,BUFFER_SIZE,0);


	//cout << "THE BUFFER IS: " << buffer << endl;
  string originalString = buffer;
    

  istringstream iss(originalString);
    
	string firstWord;

	string secondWord;
  
	


 
  iss >> firstWord;
	//cout << firstWord << "first word" << endl;
	 //try moving
	//cout << secondWord << " Second word " << endl;


	//cout << "COMMAND " << firstWord << " received" << endl;
  


	if(firstWord == "user")
	{
		iss >> secondWord;
		cout << "server: COMMAND: " << firstWord << " received. Second Word is:" << secondWord << endl;
		tempuser = &secondWord;
		if(*tempuser == username){
			userBool = true;
			//cout << "it is == to username! " << endl;
			
		}
			
	}

	else if(firstWord == "password")
	{
		iss >> secondWord;
		cout << "server: COMMAND: " << firstWord << " received. Second Word is:" << secondWord << endl;
		tempPass = &secondWord;
		if(*tempPass == password){
			passBool = true;
			//cout << "it is == to password! " << endl;
		}
			
	}

	  if(userBool == true && passBool == true)
	  {
	  	loginBool = true;
	 		send(cs,true);
	  }
	  else
	  {
	 		send(cs,false);
	  }
	

}while(loginBool == false); //ends login!

cout << "closed out of while loop. " << endl;

do
{

	cout << "server: Waiting for COMMAND " << endl;
	

	recv(cs,buffer,BUFFER_SIZE,0);


	//cout << "THE BUFFER IS: " << buffer << endl;
  string originalString = buffer;

  istringstream iss(originalString);
    
	string firstWord;

	string secondWord;
  
  iss >> firstWord;

	if(firstWord == "help")
	{
		//iss >> secondWord;
		cout << "server: COMMAND: " << firstWord << " received. " << endl;
		string help = "\n HELP: lists ftp commands with explanations \n USER: specifies a user for authentication \n PASSWORD: specifies a user's password for authentication \n DIR: lists the files in the server's current directory \n CD <path>: changes the server's current directory to the new path. The path can be relative or absolute \n LCD <path>: changes the client's current directory to the new path. The path can be relative or absolute \n DEL <filename>: deletes the server file specified by <filename> in the current directory \n GET <filename>: gets the server file specified by <filename> in the current directory and places the file in the client directory using the same <filename> \n PUT <filename>: puts the client file specified by <filename> in the current directory and places the file in the server directory using the same <filename>";
		send(cs,help);
			
		}




	else if(firstWord == "dir")
	{
		string dirList;
		cout << "command DIR received " << endl;
    // help from internet here
    char currentDirectory[PATH_MAX];
    if (getcwd(currentDirectory, sizeof(currentDirectory)) == nullptr) {
        cerr << "Error getting current working directory." << endl;
        return 1;
    }

		DIR* dir = opendir(currentDirectory);
		struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {

        // Print the name of the file or subdirectory
				if(strcmp(entry->d_name + strlen(entry->d_name) - 4, ".cpp") != 0)
				{
        	//cout << entry->d_name << endl;
					dirList = dirList + entry->d_name + "\n";
				}
    }

		send(cs,dirList);

    // Close the directory
    closedir(dir);

	}

	else if(firstWord == "cd")
	{
		iss >> secondWord;
		cout << " the second word is: " << secondWord <<endl;
		const char* newDirectory = secondWord.c_str();
		//cout << newDirectory << " <- this is the new directory" << endl;
		if (chdir(newDirectory) == 0) 
		{
      cout << "Current directory changed to: " << newDirectory << endl;
			string success = "success! changed directory to: " + secondWord;
			send(cs,success);
		}
		else{
			string failure = "Failure. cannot change directory to: " + secondWord;
			send(cs,failure);
		}
	}

	else if(firstWord == "lcd")
	{
		cout << "lcd" << endl;
	}


	else if(firstWord == "del")
	{
		iss >> secondWord;
		char currentDirectory[PATH_MAX];
		cout << getcwd(currentDirectory, sizeof(currentDirectory)) << endl;
		const char* fileToDelete = secondWord.c_str();
		string getWorkingDirectory = getcwd(currentDirectory, sizeof(currentDirectory));
		string getFullPathAndDirectory = getWorkingDirectory + "/" + fileToDelete;
		if(remove(getFullPathAndDirectory.c_str()) == 0)
		{
			cout << " removing file: " << fileToDelete << endl;
			string success = "success! you have successfully removed: " + secondWord;
			send(cs,success);
		}
		else{
			cout << " cannot find/remove file: " << endl;
			string failure = "DEL failed! cannot find/remove file: " + secondWord;
			send(cs,failure);
		}

	}



	else if(firstWord == "get")
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
		//ofstream destinationFile("/home/codio/workspace/client/belle.jpg", ios::binary);
		

			//send file in socket and have client recv file with ofstream and then place it in the clients current working directory
	}


	else if(firstWord == "put")
	{
		iss >> secondWord;
		cout << "sending COMMAND get over. " << secondWord << endl;


		size_t dataSize = 0;
    recv(cs, &dataSize, sizeof(dataSize), 0);
		cout << dataSize << endl;
		vector<char> bytesVector;
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



	else
	{
		string fail = " Cannot find COMMAND "; 
		send(cs,fail);
	}





}while(loginBool == true);





	close(cs);
	close(s);

	return 1;
}