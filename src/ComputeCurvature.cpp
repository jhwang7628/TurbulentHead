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


void surface::writeK(string Kname)
{

    cout << "write curvature" << endl;

    FILE * pFile; 

    pFile = fopen(Kname.c_str(), "w");

    if (pFile == NULL) 
    {
        cerr << "Cannot open file " + Kname << ". Exiting" << endl;
        exit(1);                                                          
    }

    for (unsigned int i=0; i<vertlist.size(); i++)
    {
        fprintf(pFile, "%.9E %.9E %.9E ", vertlist[i].position.x,
                                          vertlist[i].position.y,
                                          vertlist[i].position.z);
        fprintf(pFile, "%.9f\n", vertlist[i].curvature);
    }

    /* if matching is needed... 
     *
    string line;
    int countPoint = 0;
    int countoutlier = 0;
    getline(in,line); 
    while(getline(in,line))
    //for (int j=0; j<3000; j++)
    {
        //getline(in,line);
        //out << "==========================" << endl;
        //out << countPoint << endl;
        cout << countPoint << endl;
        Vector3<double> fluentCoord; 
        int nodenumber; 
        istringstream iss(line); 
        iss >> nodenumber;
        iss >> fluentCoord.x >> fluentCoord.y >> fluentCoord.z;

        unsigned int bestInd=0; 
        for (unsigned int i=0; i<vertlist.size(); i++)
        {
                if ((vertlist[i].position - fluentCoord).normSq() < 1e-12)
                {
                    bestInd = i; 
                    fprintf(pFile, "%.9E %.9E %.9E ", fluentCoord.x,fluentCoord.y,fluentCoord.z);
                    fprintf(pFile, "%.9f\n", vertlist[i].curvature);
                }
        }
        if (bestInd == 0)
        {
            countoutlier ++; 
            cout << "vertex " << countPoint << " is an outlier. "  << endl;
        }

        //cout << "bestmatch for vertex " << countPoint << " is vertex number " << bestInd << endl;
        //out << countPoint << " " << bestInd << endl;
        countPoint++;
    }

    /cout << "outlier = " << countoutlier << endl;


    */

    fclose(pFile);




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
