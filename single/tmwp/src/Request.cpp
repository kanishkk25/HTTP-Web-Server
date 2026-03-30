#include<iostream>
#include<tmwp>
#include<utility>
#include<map>
#include<string.h>
#include<windows.h>
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
map<string,string>::iterator i=requestMap.begin();
if(key.size()==0) return;
i=requestMap.find(key);
if(i!=requestMap.end()) return;
requestMap.insert(pair<string,string>(key,value));
}
string Request::get(string key)
{
map<string,string>::iterator i=requestMap.begin();
if(key.size()==0) return string("");
i=requestMap.find(key);
if(i!=requestMap.end()) return i->second;
return string("");
}

void Request::setDescriptor(int clientSocketDescriptor)
{
this->clientSocketDescriptor=clientSocketDescriptor;
}
void Request::setTMServer(TMServer *server)
{
this->server=server;
}
void Request::setResponse(Response *response)
{
this->response=response;
}

void Request::forward(string requestResource)
{
if(requestResource.size()==0) return;
char header[8193];
char response[1025];
char tmp[1001];
FILE *f;
int isFile=0;
for(int i=0;i<requestResource.size();i++)
{
if(requestResource[i]=='.') isFile=1;
}
if(isFile==1)
{
f=fopen(requestResource.c_str(),"rb");
if(f==NULL)
{
strcpy(response,"<!DOCTYPE HTML>");
strcat(response,"<html lang='en'>");
strcat(response,"<head>");
strcat(response,"<meta charset='utf-8'>");
strcat(response,"<title>Not Found</title>");
strcat(response,"</head>");
strcat(response,"<body>");
sprintf(tmp,"<h1 style='color: red'>Resource /%s not found</h1>",requestResource.c_str());
strcat(response,tmp);
strcat(response,"</body>");
strcat(response,"</html>");
send(clientSocketDescriptor,response,strlen(response),0);
}
else
{
fseek(f,0,SEEK_END);
int file_size=ftell(f);
fseek(f,0,SEEK_SET);
int i=0;
int toRead;
while(i<file_size)
{
toRead=file_size-i;
if(toRead>1024) toRead=1024;
fread(response,toRead,1,f);
if(feof(f))
{
send(this->clientSocketDescriptor,response,toRead,0);
break;
}
else
{
send(this->clientSocketDescriptor,response,toRead,0);
}
i=i+1024;
}
fclose(f);
closesocket(this->clientSocketDescriptor);
/* header has already been sent in TMServer through sendHeader method of Response class therefore we did not send any header here although we closed the connection
*/
}
} // is file ends
else
{
if(requestResource[0]=='/')
{
requestResource.erase(0,1);
}
map<string,void (*)(Request &,Response &)>::iterator i=server->ptrMap.find(requestResource);
if(i==server->ptrMap.end())
{
strcpy(response,"<!DOCTYPE HTML>");
strcat(response,"<html lang='en'>");
strcat(response,"<head>");
strcat(response,"<meta charset='utf-8'>");
strcat(response,"<title>Not Found</title>");
strcat(response,"</head>");
strcat(response,"<body>");
sprintf(tmp,"<h1 style='color: red'>Resource /%s not found</h1>",requestResource.c_str());
strcat(response,tmp);
strcat(response,"</body>");
strcat(response,"</html>");
send(clientSocketDescriptor,response,strlen(response),0);
closesocket(this->clientSocketDescriptor);
return;
}
void (*ptr)(Request &,Response &)=i->second;
if(ptr==NULL)
{
strcpy(response,"<!DOCTYPE HTML>");
strcat(response,"<html lang='en'>");
strcat(response,"<head>");
strcat(response,"<meta charset='utf-8'>");
strcat(response,"<title>Not Found</title>");
strcat(response,"</head>");
strcat(response,"<body>");
sprintf(tmp,"<h1 style='color: red'>Resource /%s not found</h1>",requestResource.c_str());
strcat(response,tmp);
strcat(response,"</body>");
strcat(response,"</html>");
send(clientSocketDescriptor,response,strlen(response),0);
closesocket(this->clientSocketDescriptor);
return;
}
else
{
ptr(*this,*(this->response));
}
} // functions
}