#include <fstream> 
#include <sstream> 
#include <string> 
#include <vector>
#include <algorithm>
#include <stdio.h>
#include "DataStructure.h"

using namespace std; 

void surface::computeK()
{
    cout << "compute curvature" << endl;

   int Nvert = (this->vertlist).size();

   // compute A_mixed for each vertex
   for (int i=0; i<Nvert; i++)
   {
      vertlist[i].A_mixed = 0;

      for (unsigned int j=0; j<vertlist[i].N1neighbors.size(); j++) 
      {
         tri tmpTri = vertlist[i].N1neighbors[j].neighborTri;
         double PR2, PQ2; 
         double cotQ, cotR;
         Vector3<double> PR, PQ; 
         if (i == tmpTri.index.x)
         {
             PR = vertlist[tmpTri.index.x].position - vertlist[tmpTri.index.y].position;
             PR2 = PR.normSq();
             cotQ = 1.0/tan(tmpTri.angle.z);
             PQ = vertlist[tmpTri.index.x].position - vertlist[tmpTri.index.z].position;
             PQ2 = PQ.normSq();
             cotR = 1.0/tan(tmpTri.angle.y);
         }
         else if (i == tmpTri.index.y)
         {
             PR = vertlist[tmpTri.index.y].position - vertlist[tmpTri.index.x].position;
             PR2 = PR.normSq();
             cotQ = 1.0/tan(tmpTri.angle.z);
             PQ = vertlist[tmpTri.index.y].position - vertlist[tmpTri.index.z].position;
             PQ2 = PQ.normSq();
             cotR = 1.0/tan(tmpTri.angle.x);
         }
         else if (i == tmpTri.index.z)
         {
             PR = vertlist[tmpTri.index.z].position - vertlist[tmpTri.index.x].position;
             PR2 = PR.normSq();
             cotQ = 1.0/tan(tmpTri.angle.y);
             PQ = vertlist[tmpTri.index.z].position - vertlist[tmpTri.index.y].position;
             PQ2 = PQ.normSq();
             cotR = 1.0/tan(tmpTri.angle.x);
         }
         else
         {
             cerr << "i does not match any index in tmpTri. Abort" << endl;
             exit(1);
         }

         vertlist[i].K_xi_wo_Amixed = vertlist[i].K_xi_wo_Amixed + PR.mul(cotQ) + PQ.mul(cotR);
         vertlist[i].N1neighbors[j].A_Voronoi = 1.0/8.0*(PR2*cotQ + PQ2*cotR);

         if (!vertlist[i].N1neighbors[j].neighborTri.isObtuse) 
         {
            //cout << "is NOT Obtuse!" << endl;
            //cout << "Voronoi region added" << endl;
            vertlist[i].A_mixed += vertlist[i].N1neighbors[j].A_Voronoi; 
         }
         else 
         {
             //cout << "isObtuse! Obtuse index is: " ;
             //cout << vertlist[i].N1neighbors[j].neighborTri.ObtuseIndex << endl;
            if (vertlist[i].N1neighbors[j].neighborTri.ObtuseIndex == i)
            {
               //cout << "Triangle area / 2.0 added" << endl;
               //cout << "trilist[j].area = " << trilist[j].area << endl;
               vertlist[i].A_mixed += trilist[j].area / 2.0; 
            }
            else 
            {
               //cout << "Triangle area / 4.0 added" << endl;
               //cout << "trilist[j].area = " << trilist[j].area << endl;
               vertlist[i].A_mixed += trilist[j].area / 4.0; 
            }
         }
      }


      Vector3<double> K_xi = vertlist[i].K_xi_wo_Amixed.mul(1.0/vertlist[i].A_mixed);
      vertlist[i].curvature = 1.0/4.0*K_xi.dot(vertlist[i].normal); 

      if (vertlist[i].curvature != vertlist[i].curvature)
      {
          Nnanvert++;
          vertlist[i].isnan=true;
          vertlist[i].curvature = 0.0;
      }
   }





}

// Compute the maximum Curvature in the vertex list
void surface::computeMaxK() 
{

    cout << "compute maximum curvature" << endl;

    for (size_v i=0; i<vertlist.size(); i++) 
    {
        if (abs(vertlist[i].curvature) > maxK)
        {
            maxK = abs(vertlist[i].curvature); 
        }
    }


}
