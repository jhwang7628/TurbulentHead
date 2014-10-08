#include <iostream>
#include <fstream>
#include <string> 
#include <sstream>
#include <vector>
#include <cmath>
#include "rotateOBJ.h"

#define pi 3.14159265359


using namespace std;

/* Argument in: 
 * [0] binary name
 * [1] # rotations (N)
 * [2+4*i,2+3+4*i] rotation i: axis-x axis-y axis-z angle(deg)
 *    i = 0...N-1
 *
 * Rotation 0 will be performed first, and then Rotation 1, and then ...
 * v = (R_N ... R_1 * R_0) * x
 *
 */
int main(int argc, char* argv[])
{

   rotationData rot;

   if (argc == 1)
   {
       cout << "no rotation is applied" << endl;
   }
   else
   {
        if ((argc-2)%4 != 0 || (argc-2)/4 != (double) atof(argv[1]))
        {
           cout << (argc-2)%4 << endl;
           cout << (argc-2)/4 << endl;
           cout << (double) atof(argv[1]) << endl;
           cout << "Number of input argument is incorrect!" << argc << endl;
           exit(1);
        }
        
        rot.N = (argc-2)/4;

        for (int i=0; i<rot.N; i++) 
        {
           rot.axis.push_back((double) atof(argv[i*4+2]));
           rot.axis.push_back((double) atof(argv[i*4+3]));
           rot.axis.push_back((double) atof(argv[i*4+4]));
           rot.angle.push_back((double) atof(argv[i*4+5])/180*pi);
           cout << "Rotation axis #" << i << " = ["   << rot.axis[i*3]
                << " "                                << rot.axis[i*3+1] 
                << " "                                << rot.axis[i*3+2] << "]" << endl;
           cout << "Rotation angle #" << i << " = " << rot.angle[i] << "(rad)" << endl;
        }
   }



   //const char* filename = "HumanHead-2.obj";
   string filename;
   cout << "Input the obj file that is being rotated: " << endl;
   getline(cin,filename);

   data outData1; 


   loadOBJ(filename.c_str(), outData1, rot); 

   return 0;
}


void loadOBJ(const char* filename, data outData, rotationData rot)
{
   ifstream  in(filename, ios::in); 
   FILE * out;
   //ofstream out("newOBJ.obj", ios::out); 
   out = fopen("newOBJ.obj", "w");

   //ofstream debug("debug.txt");
   if (!in) { cerr << "Cannot open " << filename << endl; exit(1); }

   string line; 
   while (getline(in, line)) 
   { 
      if (line.substr(0,2) == "v ") 
      { 
         outData.vertexCount ++; 
         //cout << outData.vertexCount << endl;
         istringstream s(line.substr(2));
         vector<double> v(3);
         s >> v[0]; s >> v[1]; s >> v[2]; 
//         cout << "New Vertex ===" << endl;
//         cout << v[0] << " " << v[1] << " " << v[2] << endl;

         if (rot.N != 0) 
         {
            for (int i=0; i<rot.N; i++)
            {
               vector<double> axis_i(3,0);
               axis_i[0] = rot.axis[i*3];
               axis_i[1] = rot.axis[i*3+1];
               axis_i[2] = rot.axis[i*3+2];
               rotate3(v[0], v[1], v[2], axis_i, rot.angle[i]);
            }
         }
       
         fprintf(out, "v %.9f %.9f %.9f \n", v[0], v[1], v[2]);

         //out << "v " << v[0] << " " << v[1] << " " << v[2] << endl;

         outData.positions.push_back(v[0]); 
         outData.positions.push_back(v[1]); 
         outData.positions.push_back(v[2]); 
      } 
      else if (line.substr(0,2) == "f ") 
      { 
         // can take in different number of nodes per face
         stringstream ss(line.substr(2));
         string token, token2;
         int intBuff, Ncount=0;
         fprintf(out, "f ");
         //out << "f ";
         int count2=0;
         //cout << line << endl;
         while (ss >> intBuff) 
         { 
             count2++;
             fprintf(out, "%d ", intBuff);
             //out << intBuff << " ";
         }
         //debug << count2 << endl;

         // while (getline(ss,token,'/'))
         // {
         //     cout << token << endl;
         //     istringstream ss_token(token); 
         //     while (getline(ss_token,token2,' '))
         //     {
         //         count2++;
         //         istringstream ss_token2(token2); 
         //         ss_token2 >> intBuff;
         //         // works only when texture is not output, otherwise change it
         //         // to count2%3==1
         //         if (count2%2==1)
         //         {
         //            cout << intBuff << " ";
         //            out  << intBuff << " ";
         //         }
         //     }
         // }
         // cout << endl;
         //out  << endl;
         fprintf(out, "\n");
      }
      if (line.substr(0,3) == "vn ") 
      { 
         outData.vertexNormalCount ++; 
         cout << outData.vertexNormalCount << endl;
         istringstream s(line.substr(2));
         vector<double> v(3);
         s >> v[0]; s >> v[1]; s >> v[2]; 
//         cout << "New Vertex ===" << endl;
//         cout << v[0] << " " << v[1] << " " << v[2] << endl;

         //out << "vn " << v[0] << " " << v[1] << " " << v[2] << endl;
         fprintf(out, "vn %.9f %.9f %.9f\n", v[0], v[1], v[2]);

         outData.vertexNormal.push_back(v[0]); 
         outData.vertexNormal.push_back(v[1]); 
         outData.vertexNormal.push_back(v[2]); 
      } 

      else if (line[0] == '#') { /* ignoring this line */ }
      else { /* ignoring this line */ }
   }


   fclose(out);
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


