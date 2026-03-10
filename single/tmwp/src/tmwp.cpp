#include<windows.h>
#include<stdio.h>
#include<tmwp>
#include<iostream>
#include<map>
#include<utility>
using namespace std;
using namespace tmwp;
Request::Request()
{
// do nothing for now
}
Request::~Request()
{
// do nothing for now
}
void Request::set(string key,string value)
{

}
string Request::get(string key)
{

}

Response::Response()
{
// do nothgin for now
}
Response::~Response()
{
// do nothgin for now
}
void Response::setDescriptor(int clientSocketDescriptor)
{

}
void Response::sendHeader()
{

}
void Response::write(string html)
{

}
void Response::close()
{

}

// Implementation of TMServer methods
TMServer::TMServer(int portNumber)
{
this->portNumber=portNumber;
}
TMServer::~TMServer()
{
// do nothing for now
}
void TMServer::start()
{

int serverSocketDescriptor;
int clientSocketDescriptor;

int successCode;
int len;

struct sockaddr_in serverSocketInformation;
struct sockaddr_in clientSocketInformation;


// set up configurations for Windows platform
WSADATA wsaData;
WORD ver=MAKEWORD(1,1);
WSAStartup(ver,&wsaData);


// creates a socket descriptor
serverSocketDescriptor=socket(AF_INET,SOCK_STREAM,0);
if(serverSocketDescriptor<0)
{
printf("Unable to create server socket\n");
WSACleanup();
return;
}

// initialzes serverSocketInformation structure 
serverSocketInformation.sin_family=AF_INET;
serverSocketInformation.sin_port=htons(this->portNumber);
// we want to listen on all IP of the machines
serverSocketInformation.sin_addr.s_addr=htonl(INADDR_ANY);

// bind the socket descriptor to IP + Port
successCode=bind(serverSocketDescriptor,(struct sockaddr *)&serverSocketInformation,sizeof(struct sockaddr));
if(successCode<0)
{
printf("Unable to bind socket descriptor to port number %d\n",this->portNumber);
closesocket(serverSocketDescriptor);
WSACleanup();
return;
}

// Put the server on listening mode
listen(serverSocketDescriptor,10);
printf("TMServer is ready to accept request on port : %d\n",this->portNumber);

len=sizeof(struct sockaddr);
// accept a request and divert it to another descriptor (clientSocketDescriptor)
clientSocketDescriptor=accept(serverSocketDescriptor,(struct sockaddr *)&clientSocketInformation,&len);
if(clientSocketDescriptor<0)
{
printf("Unable to accept request\n");
closesocket(serverSocketDescriptor);
WSACleanup();
return;
}



WSACleanup();
}
void TMServer::close()
{

}
void TMServer::onRequest(const char *url,void (*ptrOnRequest)(Request &,Response &))
{

}