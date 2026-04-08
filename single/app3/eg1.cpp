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

request.setInt("rollNumber",rollNumber);

if(rollNumber==101)
{
p=96;
c=90;
m=98;
e=80;
h=91;
}
if(rollNumber==102)
{
p=60;
c=70;
m=58;
e=73;
h=80;
}
if(rollNumber==103)
{
p=70;
c=80;
m=88;
e=81;
h=94;
}
if(rollNumber==104)
{
p=76;
c=65;
m=78;
e=59;
h=80;
}
if(rollNumber==105)
{
p=66;
c=77;
m=88;
e=55;
h=60;
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