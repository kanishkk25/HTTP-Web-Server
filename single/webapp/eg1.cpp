#include<tmwp>
#include<iostream>
using namespace std;
using namespace tmwp;
int main()
{
TMServer tmServer(6060);
tmServer.start();
return 0;
}