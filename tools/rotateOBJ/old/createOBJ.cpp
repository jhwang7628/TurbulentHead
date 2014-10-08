#include <iostream> 
#include <fstream> 
#include <string> 
#include <sstream>

#define MAXBUFSIZE ((int) 1e5)

using namespace std;

int main() 
{
   ifstream vertexList, indexList;
   ofstream obj;
   string line;

   vertexList.open("vertexlist.txt");
   indexList.open("indexlist.txt");
   obj.open("HumanHead.obj");

   while (getline(vertexList, line))
   {
      float floatBuff; 
      stringstream ss(line); 

      obj << "v ";
      while (! ss.eof())
      {
         ss >> floatBuff;
         obj << floatBuff << " ";
      }
      obj << endl;
   }

   obj << endl;

   while (getline(indexList, line))
   {
      int intBuff; 
      stringstream ss(line); 

      obj << "f ";
      while (! ss.eof())
      {
         ss >> intBuff;
         obj << intBuff << " ";
      }
      obj << endl;
   }

   obj.close();

   return 0; 
}
