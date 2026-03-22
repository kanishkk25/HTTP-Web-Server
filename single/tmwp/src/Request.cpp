#include<iostream>
#include<tmwp>
#include<utility>
#include<map>
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