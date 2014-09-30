#include <vector>

using namespace std;

class data
{
   public: 
      int indexCount;
      int vertexCount;
      int vertexNormalCount; 
      vector<float> positions; 
      vector<float> vertexNormal; 
      vector<int> NnodePerFace; 
      vector<int> index; 

      data() 
      {
         indexCount    = 0; 
         vertexCount   = 0; 
         vertexNormalCount = 0;
      }

      bool hasData()
      {
         return indexCount >= 3 && vertexCount >= 3 
             && positions.size() == indexCount
             && index.size() == indexCount;
      }


};

class rotationData 
{
   public: 
      int N; // number of rotation
      vector<float> axis; 
      vector<float> angle;

      rotationData()
      {
         N=0;
      }
};

void loadOBJ(const char* filename, data outData, rotationData rot);
void rotate3(float &x, float &y, float &z, vector<float> axis_i, float angle_i);
