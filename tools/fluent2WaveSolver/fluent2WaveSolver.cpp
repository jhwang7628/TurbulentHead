#include <fstream> 
#include <iostream> 
#include <sstream> 
#include <string> 
#include <cstdlib> 
#include <stdio.h>
#include "../Math/Vector3.h" 
#include <vector>



using namespace std; 

void rotate3(double &x, double &y, double &z, vector<double> axis_i, double angle_i);

int main() { 



    string filename, filename_out;
    filename = "../curvature/computedK.txt"; 
    filename_out = "wavesolver_input";
    FILE *file_out; 

    file_out = fopen(filename_out.c_str(), "w");
   
    ifstream in(filename.c_str()); 
    if (!in) {cerr << "cannot open file " << filename << endl; exit(1); }

    fprintf(file_out, "data/head_%%s\n");
    fprintf(file_out, "0.01\n");


    vector<Vector3<double> > pos; 

    string line;
    int count = 0;
    while(getline(in, line))
    {
        count ++;
        istringstream iss(line);
        Vector3<double> tmpBuff; 
        iss >> tmpBuff.x >> tmpBuff.y >> tmpBuff.z; 
        pos.push_back(tmpBuff); 
    }

    int N = pos.size();



    fprintf(file_out, "%d\n", count);
    for (int i=0; i<N; i++)
    {
        vector<double> axis_i;
        axis_i.push_back(0.0);
        axis_i.push_back(1.0);
        axis_i.push_back(0.0);
        //cout << "=======rotation========" << endl;
        //cout << pos[i] << endl;
        rotate3(pos[i].x, pos[i].y, pos[i].z, axis_i, 3.14159265359/2.0);
        //cout << pos[i] << endl;
        fprintf(file_out, "%.9E %.9E %.9E\n", pos[i].x, pos[i].y, pos[i].z);
    }





return 0;

}




void rotate3(double &x, double &y, double &z, vector<double> axis_i, double angle_i)
{
   double norm_axis;
   vector<double> n_axis(3,0.0);
   vector<double> R(9,0);

   norm_axis = sqrt(pow(axis_i[0],2) + pow(axis_i[1],2) + pow(axis_i[2],2));
   n_axis[0] = axis_i[0] / norm_axis; 
   n_axis[1] = axis_i[1] / norm_axis; 
   n_axis[2] = axis_i[2] / norm_axis; 

   //cout << n_axis[0] << endl << n_axis[1] << endl << n_axis[2] << endl;
   //cout << angle_i << endl;

   for (int i=0; i<3; i++)
   {
      for (int j=0; j<3; j++)
      {
         R[i*3+j] = n_axis[i]*n_axis[j] - cos(angle_i)*n_axis[i]*n_axis[j]; 

         if (i==j) { R[i*3+j] += cos(angle_i); }
      }
   }

   R[1] += -n_axis[2]*sin(angle_i); 
   R[2] +=  n_axis[1]*sin(angle_i); 
   R[3] +=  n_axis[2]*sin(angle_i); 
   R[5] += -n_axis[0]*sin(angle_i);
   R[6] += -n_axis[1]*sin(angle_i);
   R[7] +=  n_axis[0]*sin(angle_i);

//   for (int i=0; i<3; i++)
//      cout << R[i*3] << " " << R[i*3+1] << " " << R[i*3+2] << endl; 
   double x_, y_, z_;

   x_ = R[0] * x + R[1] * y + R[2] * z;
   y_ = R[3] * x + R[4] * y + R[5] * z;
   z_ = R[6] * x + R[7] * y + R[8] * z;

   x = x_; y = y_; z = z_; 
}


