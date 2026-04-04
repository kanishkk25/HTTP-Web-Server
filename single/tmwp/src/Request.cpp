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

void Request::forward(char *requestResource)
{
if(strlen(requestResource)==0) return;
char header[8193];
char response[1025];
char tmp[1001];
char variable[21];
int i,k;
char m;
int flag;
FILE *f;
int isFile=0;
int dotIndexOf=-1;
for(int i=0;i<strlen(requestResource);i++)
{
if(requestResource[i]=='.')
{
isFile=1;
dotIndexOf=i;
break;
}
}
if(isFile==1 && dotIndexOf!=-1)
{
printf("Sending file contents\n");
printf("File name : %s, with extesion : %s\n",requestResource,requestResource+dotIndexOf+1);
if(strcmp(requestResource+dotIndexOf+1,"sct")==0)
{
printf("Sending sct file contents\n");
f=fopen(requestResource,"r");
if(f==NULL)
{
// send back 404 error page
}
k=0;
i=0;
flag=0;
while(1)
{
m=fgetc(f);
if(feof(f)) break;
if(m=='$')
{
flag=1;
}
else if(flag==1 && m=='{')
{
flag=2;
}
else if(flag==2 && m!='}')
{
variable[k]=m;
k++;
}
else if(flag==2 && m=='}')
{
variable[k]='\0';
k=0;
flag=0;

char number[11];
int x=this->getInt(variable);
sprintf(number,"%d",x);
if(i>0)
{
send(this->clientSocketDescriptor,response,i,0);
i=0;
}
send(this->clientSocketDescriptor,number,strlen(number),0);
}
else 
{
response[i]=m;
i++;
if(i==1024)
{
send(this->clientSocketDescriptor,response,1024,0);
i=0;
}
flag=0;
}
} // reading from file loop ends
if(i!=0)
{
send(this->clientSocketDescriptor,response,i,0);
}
fclose(f);
closesocket(this->clientSocketDescriptor);
} // if part for sct ends

else
{
printf("Sending normal file contents\n");
f=fopen(requestResource,"rb");
if(f==NULL)
{
strcpy(response,"<!DOCTYPE HTML>");
strcat(response,"<html lang='en'>");
strcat(response,"<head>");
strcat(response,"<meta charset='utf-8'>");
strcat(response,"<title>Not Found</title>");
strcat(response,"</head>");
strcat(response,"<body>");
sprintf(tmp,"<h1 style='color: red'>Resource /%s not found</h1>",requestResource);
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
}
} // is file ends
else
{
if(requestResource[0]=='/')
{
requestResource++;
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
sprintf(tmp,"<h1 style='color: red'>Resource /%s not found</h1>",requestResource);
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
sprintf(tmp,"<h1 style='color: red'>Resource /%s not found</h1>",requestResource);
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

void Request::setInt(string variable,int value)
{
char tmp[12];
map<string,string>::iterator i=requestMap.begin();
if(variable.size()==0) return;
i=requestMap.find(variable);

sprintf(tmp,"%d",value);

if(i==requestMap.end())
{
requestMap.insert(pair<string,string>(variable,tmp));
}
else
{
i->second=tmp;
}
}

int Request::getInt(string variable)
{
int value=0;
map<string,string>::iterator i=requestMap.begin();
if(variable.size()==0) return 0;
i=requestMap.find(variable);
if(i==requestMap.end())
{
return 0;
}
value=atoi(i->second.c_str());
return value;
}