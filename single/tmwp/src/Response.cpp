#include<iostream>
#include<windows.h>
#include<tmwp>
#include<string.h>
#include<utility>
#include<map>
using namespace std;
using namespace tmwp;

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
this->clientSocketDescriptor=clientSocketDescriptor;
}
void Response::sendHeader()
{
char header[8192+1];
sprintf(header,"HTTP/1.1 200 OK\nContent-Type: text/html\nConnection: close\n\n");
send(this->clientSocketDescriptor,header,strlen(header),0);
}
void Response::write(string html)
{
send(this->clientSocketDescriptor,html.c_str(),html.size(),0);
}
void Response::close()
{
closesocket(this->clientSocketDescriptor);
}