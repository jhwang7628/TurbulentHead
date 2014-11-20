#include <iostream>
#include <fstream>
#include <string> 
#include <sstream>
#include "loadOBJ.h"

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
         glm::vec3 v; 
         s >> v.x; s >> v.y; s >> v.z; 
         outData.positions.push_back(v); 
      } 
      else if (line.substr(0,2) == "f ") 
      { 
         outData.indexCount ++; 
         istringstream s(line.substr(2)); 
         int a,b,c; 
         s >> a; s >> b; s >> c; 
         outData.index.push_back(a); outData.index.push_back(b); outData.index.push_back(c);
      }
      else if (line[0] == '#') { /* ignoring this line */ }
      else { /* ignoring this line */ }
   }
}
