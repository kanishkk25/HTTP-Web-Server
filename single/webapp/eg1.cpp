#include<tmwp>
#include<stdio.h>
#include<string.h>
#include<iostream>
#include<fstream>
using namespace std;
using namespace tmwp;
void function_one(Request &request,Response &response)
{
cout<<"One got called"<<endl;
request.forward("xyz.html");
}
void function_two(Request &request,Response &response)
{
request.forward("/three");
}
void function_three(Request &request,Response &response)
{
response.write("<!DOCTYPE HTML>");
response.write("<html lanh='en'>");
response.write("<head>");
response.write("<meta charset='utf-8'>");
response.write("<title>My Website</title>");
response.write("</head>");
response.write("<body>");
response.write("<h1>Cool</h1>");
response.write("</body>");
response.write("</html>");
response.close();
}

int main()
{
TMServer tmServer(7070);

tmServer.onRequest("/one",function_one);
tmServer.onRequest("/two",function_two);
tmServer.onRequest("/three",function_three);

tmServer.start();
return 0;
}