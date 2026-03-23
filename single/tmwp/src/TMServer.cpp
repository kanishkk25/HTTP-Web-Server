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
char isClientSideTechnologyResource;
char *mimeType;
int dataCount;
char **data;
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
if(extensionEquals(resource+indexOfDot+1,(char *)"jpg"))
{
mimeType=(char *)malloc(sizeof(char)*11);
if(mimeType==NULL) return NULL;
strcpy(mimeType,"image/jpeg");
}
if(extensionEquals(resource+indexOfDot+1,(char *)"jpeg"))
{
mimeType=(char *)malloc(sizeof(char)*11);
if(mimeType==NULL) return NULL;
strcpy(mimeType,"image/jpeg");
}
if(extensionEquals(resource+indexOfDot+1,(char *)"png"))
{
mimeType=(char *)malloc(sizeof(char)*10);
if(mimeType==NULL) return NULL;
strcpy(mimeType,"image/png");
}
if(extensionEquals(resource+indexOfDot+1,(char *)"ico"))
{
mimeType=(char *)malloc(sizeof(char)*13);
if(mimeType==NULL) return NULL;
strcpy(mimeType,"image/x-icon");
}
return mimeType;
}

char isClientSideTechnologyResource(char *resource)
{
int i=0;
while(resource[i]!='\0' &&  resource[i]!='.')
{
i++;
}
if(resource[i]=='\0') return 'N';
return 'Y';
}

REQUEST * parseRequest(const char *header)
{
REQUEST *request;
char method[11];
char resource[100001];
int i,j,si;

if(header==NULL) return NULL;

// extract the Method part
for(i=0;header[i]!='\0' && header[i]!=' ';i++)
{
method[i]=header[i];
}
method[i]='\0';
i=i+2;

int dataCount=0;
char **data=NULL;

if(strcmp("GET",method)==0)
{
// extract the resource part
for(j=0;header[i]!='\0' && header[i]!=' ';i++,j++)
{
if(header[i]=='?') break;
resource[j]=header[i];
}
resource[j]='\0';
if(header[i]=='?')
{
si=i;
while(header[i]!='\0' && header[i]!=' ')
{
if(header[i]=='&') dataCount++;
i++;
}
dataCount++;

data=(char **)malloc(sizeof(char *)*dataCount);
int *pc=(int *)malloc(sizeof(int)*dataCount);
i=si;
j=0;
while(header[i]!=' ' && header[i]!='\0')
{
if(header[i]=='&')
{
pc[j]=i;
j++;
}
i++;
}
pc[j]=i;
i=si+1;
j=0;
int howManyToPick;
while(j<dataCount)
{
howManyToPick=pc[j]-i;
data[j]=(char *)malloc(sizeof(char)*(howManyToPick+1));
strncpy(data[j],header+i,howManyToPick);
data[j][howManyToPick]='\0';
i=pc[j]+1;
j++;
}
} // if for ? encounter ends
} // if for GET method ends

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

request->dataCount=dataCount;
request->data=data;

if(resource[0]=='\0')
{
request->resource=NULL;
request->isClientSideTechnologyResource='Y';
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
request->isClientSideTechnologyResource=isClientSideTechnologyResource(request->resource);
request->mimeType=getMIMEType(request->resource);
}
return request;
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
int i,toRead,j;
char tmp[1001];
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
if(request->isClientSideTechnologyResource=='Y')
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
sprintf(header,"HTTP 200 OK\nContent-Type: text/html\nContent-Length: %d\nConnection: close\n\n",strlen(response));
send(clientSocketDescriptor,header,strlen(header),0);
send(clientSocketDescriptor,response,strlen(response),0);
}
else
{
fseek(f,0,SEEK_END);
file_size=ftell(f);
fseek(f,0,SEEK_SET);
sprintf(header,"HTTP 200 OK\nContent-Type: text/html\nContent-Length: %d\nConnection: close\n\n",file_size);
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
f=fopen(request->resource,"rb");
if(f==NULL)
{
strcpy(response,"<!DOCTYPE HTML>");
strcat(response,"<html lang='en'>");
strcat(response,"<head>");
strcat(response,"<meta charset='utf-8'>");
strcat(response,"<title>Not Found</title>");
strcat(response,"</head>");
strcat(response,"<body>");

sprintf(tmp,"<h1 style='color: red'>Resource /%s not found</h1>",request->resource);

strcat(response,tmp);
strcat(response,"</body>");
strcat(response,"</html>");
sprintf(header,"HTTP 200 OK\nContent-Type: text/html\nContent-Length: %d\nConnection: close\n\n",strlen(response));
send(clientSocketDescriptor,header,strlen(header),0);
send(clientSocketDescriptor,response,strlen(response),0);
}
else
{
fseek(f,0,SEEK_END);
file_size=ftell(f);
fseek(f,0,SEEK_SET);
sprintf(header,"HTTP 200 OK\nContent-Type: text/html\nContent-Length: %d\nConnection: close\n\n",file_size);
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
// else part of f==NULL ends
}
// else part of request->resource==NULL ends
}
// resource->isClientSideTechnologyResource=='Y' ends
}
else
{
printf("URL is : %s\n",request->resource);
map<string,void (*)(Request &,Response &)>::iterator iter=this->ptrMap.find(request->resource);
if(iter==this->ptrMap.end())
{
strcpy(response,"<!DOCTYPE HTML>");
strcat(response,"<html lang='en'>");
strcat(response,"<head>");
strcat(response,"<meta charset='utf-8'>");
strcat(response,"<title>Not Found</title>");
strcat(response,"</head>");
strcat(response,"<body>");
sprintf(tmp,"<h1 style='color:red'>Resource /%s not found</h1>",request->resource);
strcat(response,tmp);
strcat(response,"</body>");
strcat(response,"</html>");

sprintf(header,"HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: %d\nConnection: close\n\n",strlen(response));

send(clientSocketDescriptor,header,strlen(header),0);
send(clientSocketDescriptor,response,strlen(response),0);
}
else
{
void (*ptr)(Request &,Response &);
ptr=iter->second;
if(ptr==NULL)
{
printf("Yahs\n");
strcpy(response,"<!DOCTYPE HTML>");
strcat(response,"<html lang='en'>");
strcat(response,"<head>");
strcat(response,"<meta charset='utf-8'>");
strcat(response,"<title>Not Found</title>");
strcat(response,"</head>");
strcat(response,"<body>");
sprintf(tmp,"<h1 style='color:red'>Resource /%s not found</h1>",request->resource);
strcat(response,tmp);
strcat(response,"</body>");
strcat(response,"</html>");

sprintf(header,"HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: %d\nConnection: close\n\n",strlen(response));

send(clientSocketDescriptor,header,strlen(header),0);
send(clientSocketDescriptor,response,strlen(response),0);
}
else
{
Response res;
Request req;
char key[1001];
char value[1001];

for(i=0;i<request->dataCount;i++)
{
j=0;
while(request->data[i][j]!='\0')
{
if(request->data[i][j]=='=')
{
key[j]='\0';
req.set(key,request->data[i]+j+1);
break;
}
else
{
key[j]=request->data[i][j];
}
j++;
}
}
// free request->data;
if(request->data!=NULL)
{
for(i=0;i<request->dataCount;i++)
{
free(request->data[i]);
}
free(request->data);
}
res.setDescriptor(clientSocketDescriptor);
res.sendHeader();
ptr(req,res);
}
}
// else part of resource->isClientSideTechnologyResource=='Y' ends
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
map<string,void (*)(Request &,Response &)>::iterator i=ptrMap.begin();
i=ptrMap.find(url);
if(i!=ptrMap.end()) return;
if(url[0]=='/') url=url+1;
ptrMap.insert(pair<string,void (*)(Request &,Response &)>(url,ptrOnRequest));
}