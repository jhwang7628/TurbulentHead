#include <vector>

using namespace std;

class data
{
   public: 
      int indexCount;
      int vertexCount;
      int vertexNormalCount; 
      vector<double> positions; 
      vector<double> vertexNormal; 
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
      vector<double> axis; 
      vector<double> angle;

      rotationData()
      {
         N=0;
      }
};

void loadOBJ(const char* filename, data outData, rotationData rot);
void rotate3(double &x, double &y, double &z, vector<double> axis_i, double angle_i);
