#include <vector>

using namespace std;

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

void loadFILE(string filename, rotationData rot);
void rotate3(double &x, double &y, double &z, vector<double> axis_i, double angle_i);
