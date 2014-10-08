#include <iostream>
#include <fstream>
#include <string> 
#include <sstream>
#include <vector>
#include "loadOBJ.h"

using namespace std;

int main()
{
   const char* filename = "head_final.obj";

   data outData1; 

   loadOBJ(filename, outData1); 



   return 0;
}


void loadOBJ(const char* filename, data outData)
{
   ifstream in(filename, ios::in); 
   if (!in) { cerr << "Cannot open " << filename << endl; exit(1); }

   string line; 
   while (getline(in, line)) 
   { 
      if (line.substr(0,2) == "v ") 
      { 
         outData.vertexCount ++; 
         istringstream s(line.substr(2));
         vector<float> v(3); 
         s >> v[0]; s >> v[1]; s >> v[2]; 
         outData.positions.push_back(v[0]); 
         outData.positions.push_back(v[1]); 
         outData.positions.push_back(v[2]); 
      } 
      else if (line.substr(0,2) == "f ") 
      { 
         // can take in different number of nodes per face
         stringstream ss(line.substr(2));
         int intBuff, Ncount=0;
         while (ss >> intBuff)
         {
            outData.index.push_back(intBuff);
            cout << intBuff << " ";
            Ncount++;
         }
         outData.NnodePerFace.push_back(Ncount);
         outData.indexCount ++; 
      }
      else if (line[0] == '#') { /* ignoring this line */ }
      else { /* ignoring this line */ }
   }
}
