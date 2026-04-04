#include<tmwp>
#include<stdio.h>
#include<string.h>
#include<iostream>
#include<fstream>
using namespace std;
using namespace tmwp;

void printStudentMarks(Request &request,Response &response)
{

int rollNumber=request.getInt("rl");

int p,c,e,h,m;
p=0;
c=0;
m=0;
e=0;
h=0;

if(rollNumber==101)
{
p=96;
c=90;
m=98;
e=80;
h=91;
}
request.setInt("physics",p);
request.setInt("chemistry",c);
request.setInt("maths",m);
request.setInt("english",e);
request.setInt("hindi",h);
request.forward((char *)"marksheet.sct");
}

int main()
{
TMServer tmServer(7070);

tmServer.onRequest("/studentMarks",printStudentMarks);

tmServer.start();
return 0;
}