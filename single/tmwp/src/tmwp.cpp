#include<windows.h>
#include<stdio.h>
#include<string.h>
#include<tmwp>
#include<iostream>
#include<map>
#include<utility>
using namespace std;
using namespace tmwp;

/*
	struct : REQUEST
	Purpose : this structure will store the data request received from header 
*/
typedef struct __request
{
char *method;
char *resource;
char isClientSideTechnology;
char *mimeType;
}REQUEST;

int extensionEquals(char *left,char *right)
{
char a,b;
while(*left && *right)
{
a=*left;
b=*right;
if(a>=65 && a<=90) a=a+32;
if(b>=65 && b<=90) b=b+32;
if(a!=b) break;
left++;
right++;
}
return *left==*right;
}

char * getMIMEType(char *resource)
{
char *mimeType;
int indexOfDot;
int length=strlen(resource);
if(resource==NULL) return NULL;
if(length<4) return NULL;
for(indexOfDot=length-1;indexOfDot>0 && resource[indexOfDot]!='.';indexOfDot--);
if(indexOfDot<=0) return NULL;

if(extensionEquals(resource+indexOfDot+1,(char *)"html"))
{
mimeType=(char *)malloc(sizeof(char)*9);
if(mimeType==NULL) return NULL;
strcpy(mimeType,"text/html");
}
if(extensionEquals(resource+indexOfDot+1,(char *)"css"))
{
mimeType=(char *)malloc(sizeof(char)*9);
if(mimeType==NULL) return NULL;
strcpy(mimeType,"text/css");
}
if(extensionEquals(resource+indexOfDot+1,(char *)"js"))
{
mimeType=(char *)malloc(sizeof(char)*16);
if(mimeType==NULL) return NULL;
strcpy(mimeType,"text/javascript");
}
return mimeType;
}

char isClientSideTechnology(char *resource)
{
return 'Y';
}

REQUEST * parseRequest(const char *header)
{
REQUEST *request;
char method[11];
char resource[100001];
int i,j;

if(header==NULL) return NULL;

// extract the Method part
for(i=0;header[i]!='\0' && header[i]!=' ';i++)
{
method[i]=header[i];
}
method[i]='\0';
i=i+2;

// extract the resource part
j=0;
for(;header[i]!='\0' && header[i]!=' ';i++,j++)
{
resource[j]=header[i];
}
resource[j]='\0';
// create REQUEST structure and populate it's members
request=(REQUEST *)malloc(sizeof(REQUEST));
if(request==NULL) return NULL;

request->method=(char *)malloc(sizeof(char)*(strlen(method)+1));
if(request->method==NULL)
{
free(request);
return NULL;
}
strcpy(request->method,method);
if(resource[0]=='\0')
{
request->resource=NULL;
request->isClientSideTechnology='Y';
request->mimeType=NULL;
}
else
{
request->resource=(char *)malloc(sizeof(char)*(strlen(resource)+1));
if(request->resource==NULL)
{
free(request->method);
free(request);
return NULL;
}
strcpy(request->resource,resource);
request->isClientSideTechnology=isClientSideTechnology(request->resource);
request->mimeType=getMIMEType(request->resource);
if(request->mimeType==NULL)
{
free(request->method);
free(request->resource);
return NULL;
}
}
return request;
}

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
char header[8192+1];  // extra 1 for '\0'
char request[1024+1];  // extra 1 for '\0'
char response[1024+1]; // extra 1 for '\0'

int serverSocketDescriptor;
int clientSocketDescriptor;

int successCode;
int len;
int bytes_extracted;

struct sockaddr_in serverSocketInformation;
struct sockaddr_in clientSocketInformation;

FILE *f;
char m;
long file_size;
int i,toRead;

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
// server should listen on all IP of the machines
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
/*
infinte loop for keeping server on listening mode until user does not close connection
*/
while(1)
{
// accept a request and divert it to another descriptor (clientSocketDescriptor)
clientSocketDescriptor=accept(serverSocketDescriptor,(struct sockaddr *)&clientSocketInformation,&len);
if(clientSocketDescriptor<0)
{
printf("Unable to accept request\n");
closesocket(serverSocketDescriptor);
WSACleanup();
return;
}
bytes_extracted=recv(clientSocketDescriptor,header,sizeof(header),0);
if(bytes_extracted<0)
{
printf("Unable to accept request due to some issue\n");
closesocket(serverSocketDescriptor);
WSACleanup();
return;
}
else if(bytes_extracted==0)
{
printf("Unable to accept request, server might be off\n");
closesocket(serverSocketDescriptor);
WSACleanup();
return;
}
header[bytes_extracted]='\0';
REQUEST *request=parseRequest(header);
if(request==NULL)
{
printf("Invalid request\n");
closesocket(serverSocketDescriptor);
WSACleanup();
return;
}
if(request->isClientSideTechnology=='Y')
{
if(request->resource==NULL)
{
f=fopen("index.html","rb");
if(f==NULL)
{
f=fopen("index.htm","rb");
}
if(f==NULL)
{
strcpy(response,"<!DOCTYPE HTML>");
strcat(response,"<html lang='en'>");
strcat(response,"<head>");
strcat(response,"<meta charset='utf-8'>");
strcat(response,"<title>Not Found</title>");
strcat(response,"</head>");
strcat(response,"<body>");
strcat(response,"<h1 style='color: red'>Resource / not found</h1>");
strcat(response,"</body>");
strcat(response,"</html>");
sprintf(header,"HTTP 200 OK\nContent-Type: text/html\nContent-Length: %d\nConnection: Keep-Alive\n\n",strlen(response));
send(clientSocketDescriptor,header,strlen(header),0);
send(clientSocketDescriptor,response,strlen(response),0);
}
else
{
fseek(f,0,SEEK_END);
file_size=ftell(f);
fseek(f,0,SEEK_SET);
sprintf(header,"HTTP 200 OK\nContent-Type: text/html\nContent-Length: %d\nConnection: Keep-Alive\n\n",file_size);
send(clientSocketDescriptor,header,strlen(header),0);
i=0;
while(i<file_size)
{
toRead=file_size-i;
if(toRead>1024) toRead=1024;
fread(response,toRead,1,f);
send(clientSocketDescriptor,response,toRead,0);
i=i+1024;
} // infinite loop to read from file ends
fclose(f);
}
// request->resource==NULL part ends
}
else
{



// else part of request->resource==NULL ends
}
// resource->isClientSideTechnology=='Y' ends
}
else
{


// else part of resource->isClientSideTechnology=='Y' ends
}
} // infinite loop ends here
closesocket(serverSocketDescriptor);
WSACleanup();
}

// will be removed 
void TMServer::close()
{

}
void TMServer::onRequest(const char *url,void (*ptrOnRequest)(Request &,Response &))
{

}