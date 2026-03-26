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

void Request::forward(string requestResource)
{
if(requestResource.size()==0) return;
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
//send back 404 error page
}
else
{
fseek(f,0,SEEK_END);
int file_size=ftell(f);
fseek(f,0,SEEK_SET);
char header[8193];
char response[1025];

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
/* header has already been sent in TMServer through sendHeader method of Response class therefore we did not sent any header here though we close the connection
*/
}
} // is file ends
else
{

} // functions
}