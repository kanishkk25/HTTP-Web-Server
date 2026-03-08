#include<tmwp>
#include<iostream>
#include<map>
#include<utility>
using namespace std;
using namespace tmwp;

map<string,string> requestMap;

Request::Request()
{

}
Request::~Request()
{

}
void Request::set(string key,string value)
{

}
string Request::get(string key)
{

}

Response::Response()
{

}
Response::~Response()
{

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
TMServer::TMServer(int portNumber)
{

}
TMServer::~TMServer()
{

}
void TMServer::start()
{

}
void TMServer::onRequest(const char *url,void (*ptrOnRequest)(Request &,Response &))
{

}
