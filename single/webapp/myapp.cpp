#include<tmwp>
#include<stdio.h>
#include<string.h>
#include<iostream>
#include<fstream>
using namespace std;
using namespace tmwp;
class Student
{
public:
int rollNumber;
char name[21];
};
void addStudent(Request &request,Response &response)
{
/*
no need to check uniqueness of the roll number
but be sure that you are not adding duplicate roll number
code to add data in to file
put data in object, open file in append and binary mode
write object 
close file
*/
// using response.write send back html that says, student added
// add link (ok) to send request for index.html
// in the end don't forget to call close method

Student student;
ofstream file("student.data",ios::app | ios::binary);

student.rollNumber=atoi((request.get("rl")).c_str());
strcpy(student.name,(request.get("nm")).c_str());
file.write((char *)&student,sizeof(Student));
cout<<"Student added"<<endl;
file.close();

response.write("<!DOCTPYE HTML>");
response.write("<html lang='en>");
response.write("<head>");
response.write("<meta charset='utf-8'>");
response.write("<title>ABC School, Ujjain</title>");
response.write("</head>");
response.write("<body>");
response.write("Student (Add Module)<br>");
response.write("<h3>Student Added</h3>");
response.write("<form method='get' action='index.html'>");
response.write("<button type='submit'>OK</button>");
response.write("</from>");
response.write("</body>");
response.write("</html>");

response.close();
}
void getAllStudents(Request &request,Response &response)
{
/*
code to open file
create html with data form file and send it using response.write
*/
// in the end don't forget to call close method 

Student student;
ifstream file("student.data",ios::in | ios::binary);
int count=0;
char tmp[1001];
response.write("<!DOCTPYE HTML>");
response.write("<html lang='en>");
response.write("<head>");
response.write("<meta charset='utf-8'>");
response.write("<title>ABC School, Ujjain</title>");
response.write("</head>");
response.write("<body>");

response.write("<table border='1'>");
response.write("<tr>");
response.write("<td>S.No.</td><td>Roll number</td><td>Name</td>");
response.write("</tr>");

while(1)
{
file.read((char *)&student,sizeof(Student));
if(file.fail()) break;
count++;

sprintf(tmp,"<td>%d</td><td>%d</td><td>%s</td>",count,student.rollNumber,student.name);

response.write("<tr>");
response.write(tmp);
response.write("</tr>");
}
response.write("</table>");

if(count==0) response.write("<h1 style='color:red'>No Students has been added</h1>");
file.close();
response.write("<br>");
response.write("<a href='index.html'>OK</a>");
response.write("</body>");
response.write("</html>");
response.close();

}
void sendStudentEditForm(Request &request,Response &response)
{
// extract roll number from request (rl)
// file handling code to search roll number and send html
char tmp[1001];
char name[51];
string rl=request.get("rl");
int rollNumber=atoi(rl.c_str());
Student student;
int found=0;
ifstream file("student.data",ios::in | ios::binary);
while(1)
{
file.read((char *)&student,sizeof(Student));
if(file.fail()) break;
if(student.rollNumber==rollNumber)
{
found=1;
break;
}
}
if(found==1) strcpy(name,student.name);
else strcpy(name,"None");
file.close();

response.write("<!DOCTYPE HTML>");
response.write("<html lang='en'>");
response.write("<head>");
response.write("<meta charset='utf-8'>");
response.write("<title>ABC School, Ujjain</title>");
response.write("</head>");
response.write("<body>");
response.write("Student (Edit Module)");
response.write("<form method='get' action='updateStudent'>");
sprintf(tmp,"Roll number : %d",rollNumber);
response.write(tmp);
sprintf(tmp,"<input type='hidden' id='rl' name='rl' value='%d'><br>",rollNumber);
response.write(tmp);
sprintf(tmp,"Name <input type='text' id='nm' name='nm' value='%s'><br>",name);
response.write(tmp);
response.write("<button type='submit'>Update</button>");
response.write("</from>");
response.write("<br>");
response.write("</body>");
response.write("</html>");
response.close();
/*
note : 
Roll number : 101	in place of 101 value of rl should be placed
value='101'         	in place of 101 value of rl should be placed
value='Sameer'		in place of Sameer the name against 101 should be placed
*/
}
void updateStudentData(Request &request,Response &response)
{

// code to extract data againt rl and nm 
// file handling code to search update roll number and update name

int rollNumber=atoi(request.get("rl").c_str());
string name=request.get("nm");
Student student;
int found=0;
int foundAt;
fstream file("student.data",ios::out | ios::in | ios::binary);
while(1)
{
foundAt=file.tellp();
file.read((char *)&student,sizeof(Student));
if(file.fail()) break;
if(student.rollNumber==rollNumber)
{
found=1;
break;
}
}
if(found==1)
{
file.seekp(foundAt);
student.rollNumber=rollNumber;
strcpy(student.name,name.c_str());
file.write((char *)&student,sizeof(Student));
}
file.close();
response.write("<!DOCTYPE HTML>");
response.write("<html lang='en'>");
response.write("<head>");
response.write("<meta charset='utf-8'>");
response.write("<title>ABC School, Ujjain</title>");
response.write("</head>");
response.write("<body>");
response.write("Student (Edit Module)<br>");
response.write("<h3>Student updated</h3>");
response.write("<form method='get' action='index.html'>");
response.write("<button type='submit'>OK</button>");
response.write("</from>");
response.write("</body>");
response.write("</html>");
response.close();
}
void sendStudentDeleteConfirmationForm(Request &request,Response &response)
{
// extract roll number from request (rl)
// file handling code to search roll number and send html
char tmp[1001];
char name[51];
string rl=request.get("rl");
int rollNumber=atoi(rl.c_str());
Student student;
int found=0;
ifstream file("student.data",ios::in | ios::binary);
while(1)
{
file.read((char *)&student,sizeof(Student));
if(file.fail()) break;
if(student.rollNumber==rollNumber)
{
found=1;
break;
}
}
if(found==1) strcpy(name,student.name);
else strcpy(name,"None");
file.close();

response.write("<!DOCTYPE HTML>");
response.write("<html lang='en'>");
response.write("<head>");
response.write("<meta charset='utf-8'>");
response.write("<title>ABC School, Ujjain</title>");
response.write("</head>");
response.write("<body>");
response.write("Student (Delete Module)");
response.write("<form method='get' action='deleteStudent'>");
sprintf(tmp,"Roll number : %d",rollNumber);
response.write(tmp);
sprintf(tmp,"<input type='hidden' id='rl' name='rl' value='%d'><br>",rollNumber);
response.write(tmp);
sprintf(tmp,"Name : %s<br>",name);
response.write(tmp);
response.write("<button type='submit'>Delete</button>");
response.write("</from>");
response.write("<br>");
response.write("</body>");
response.write("</html>");
response.close();
/*
note : 
Roll number : 101	in place of 101 value of rl should be placed
value='101'         	in place of 101 value of rl should be placed
value='Sameer'		in place of Sameer the name against 101 should be placed
*/
}
void deleteStudentData(Request &request,Response &response)
{

// code to extract data againt rl 
// file handling code to search delete data

int rollNumber=atoi(request.get("rl").c_str());
Student student;
int found=0;
ifstream file("student.data",ios::in | ios::binary);
while(1)
{
file.read((char *)&student,sizeof(Student));
if(file.fail()) break;
if(student.rollNumber==rollNumber)
{
printf("Same same\n");
found=1;
break;
}
}
file.close();
if(found==1)
{
file.open("student.data",ios::in | ios::binary);
ofstream tmpFile("tmp.tmp",ios::out | ios::binary);
while(1)
{
file.read((char *)&student,sizeof(Student));
if(file.fail()) break;
if(student.rollNumber!=rollNumber)
{
tmpFile.write((char *)&student,sizeof(Student));
}
}
file.close();
tmpFile.close();

ofstream dataFile("student.data",ios::out | ios::binary);
ifstream tmp("tmp.tmp",ios::in | ios::binary);
while(1)
{
tmp.read((char *)&student,sizeof(Student));
if(tmp.fail()) break;
dataFile.write((char *)&student,sizeof(Student));
}
dataFile.close();
tmp.close();
tmpFile.open("tmp.tmp",ios::out);
tmpFile.close();
}
response.write("<!DOCTYPE HTML>");
response.write("<html lang='en'>");
response.write("<head>");
response.write("<meta charset='utf-8'>");
response.write("<title>ABC School, Ujjain</title>");
response.write("</head>");
response.write("<body>");
response.write("Student (Delete Module)<br>");
response.write("<h3>Student Deleted</h3>");
response.write("<form method='get' action='index.html'>");
response.write("<button type='submit'>OK</button>");
response.write("</from>");
response.write("</body>");
response.write("</html>");
response.close();
}
int main()
{
TMServer tmServer(7070);
tmServer.onRequest("/addStudent",addStudent);
tmServer.onRequest("/getStudents",getAllStudents);
tmServer.onRequest("/editStudent",sendStudentEditForm);
tmServer.onRequest("/updateStudent",updateStudentData);
tmServer.onRequest("/confirmDeleteStudent",sendStudentDeleteConfirmationForm);
tmServer.onRequest("/deleteStudent",deleteStudentData);
tmServer.start();
return 0;
}