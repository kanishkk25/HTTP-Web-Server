#include<tmwp>
#include<stdio.h>
#include<string.h>
#include<iostream>
#include<fstream>
using namespace std;
using namespace tmwp;

void function_one(Request &request,Response &response)
{
request.setInt("abcd",23);
request.setInt("pqr",3333);
request.forward("/two");
}
void function_two(Request &request,Response &response)
{
char tmp[11];

int x=request.getInt("abcd");
int y=request.getInt("pqr");

response.write("<!DOCTYPE HTML>");
response.write("<html lang='en'>");
response.write("<head>");
response.write("<meta charset='utf-8'>");
response.write("<title>ABCL School, Ujjain</title>");
response.write("</head>");
response.write("<body>");
response.write("<h1> Value of x : ");
sprintf(tmp,"%d",x);
response.write(tmp);
response.write("</h1>");
response.write("<br>");
response.write("<h1> Value of y : ");
sprintf(tmp,"%d",y);
response.write(tmp);
response.write("</h1>");
response.write("</body>");
response.write("</html>");

response.close();
}
int main()
{
TMServer tmServer(7070);

tmServer.onRequest("/one",function_one);
tmServer.onRequest("/two",function_two);

tmServer.start();
return 0;
}