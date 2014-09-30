#include <vector>

using namespace std;

class data
{
   public: 
      unsigned long int indexCount;
      unsigned long int vertexCount;
      vector<float> positions; 
      vector<int> NnodePerFace; 
      vector<int> index; 

      data() 
      {
         indexCount    = 0; 
         vertexCount   = 0; 
      }

      bool hasData()
      {
         return indexCount >= 3 && vertexCount >= 3 
             && positions.size() == indexCount
             && index.size() == indexCount;
      }


};

void loadOBJ(const char* filenname, data outData);

