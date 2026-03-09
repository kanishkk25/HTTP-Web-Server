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

}
void TMServer::close()
{

}
void TMServer::onRequest(const char *url,void (*ptrOnRequest)(Request &,Response &))
{

}