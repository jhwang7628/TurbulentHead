#include "DataStructure.h" 
#include <iostream> 
#include <stdio.h>
#include <cstdlib> 
#include <cmath>
#include <vector> 
#include <string>
#include "Vector3.h"

using namespace std;

void surface::sumSources() {

    std::cout << "sum the sources" << std::endl;

    if (Nts_ == 0 || NCell_ == 0) 
    {
        cerr << "the dimension of the sources have not been set. use the method setSimDim in the class surface to set the dimension before summing the sources. returning" << endl;
        return;
    }

    uint sum_method = 2;


    switch (sum_method) 
    {
        case 0:
            sumSources_direct(); 
            sum_method_ = "direct"; 
            break; 
        case 1: 
            sumSources_areaWeighted(); 
            sum_method_ = "Voronoi area weighted"; 
            break;
        case 2: 
            sumSources_areaWeighted_FreeSpaceG(); 
            sum_method_ = "Voronoi area weighted with free-space-Green's-function.";
            break; 
        case 3: 
            sumSources_areaWeighted_SphereHeadG(); 
            sum_method_ = "Voronoi area weighted with spherical-head-Green's-function.";
            break; 
    }


    std::cout << "sum the sources completed" << std::endl;


}


/* 
 * Direct sum of the sound sources 
 */
void surface::sumSources_direct() { 

    cout << "sum the sources with a direct method" << endl;

    for (uint i=0; i<Nts_; i++)
    {
        double current_s1sum=0.0; 
        double current_s2sum=0.0;
        for (uint j=0; j<NCell_; j++) 
        {

            current_s1sum += vertlist[j].source1[i];
            current_s2sum += vertlist[j].source2[i];
        }

        source1_s.push_back(current_s1sum); 
        source2_s.push_back(current_s2sum); 

        cout << "sum of source 1 at timestep " << i << " = " << current_s1sum << endl;
        cout << "sum of source 2 at timestep " << i << " = " << current_s2sum << endl;

    }

    if (writeSourcesSum_) 
        writeSourcesSum();
}


/* 
 * Sum of the sound sources weighted by the vertex Voronoi area
 */
void surface::sumSources_areaWeighted() { 

    cout << "sum the sources weighted with Voronoi area" << endl;


    for (uint i=0; i<Nts_; i++)
    {
        double current_s1sum=0.0; 
        double current_s2sum=0.0;
        for (uint j=0; j<NCell_; j++) 
        {
            current_s1sum += vertlist[j].source1[i]*vertlist[j].A_Voronoi_sum;
            current_s2sum += vertlist[j].source2[i]*vertlist[j].A_Voronoi_sum;
        }

        source1_s.push_back(current_s1sum); 
        source2_s.push_back(current_s2sum); 

        cout << "sum of source 1 at timestep " << i << " = " << current_s1sum << endl;
        cout << "sum of source 2 at timestep " << i << " = " << current_s2sum << endl;

    }

    if (writeSourcesSum_) 
        writeSourcesSum();

}


/* 
 * Sum of the sound sources weighted by the vertex Voronoi area
 *
 * input: list of listening position
 * 
 */
void surface::sumSources_areaWeighted_FreeSpaceG() { 

    cout << "sum the sources weighted with Voronoi area using free space Green's function" << endl;

    // construct the vector of listening position
    vector<Vector3<double> > ListeningPosition;

    Vector3<double> l0(-0.003837, -0.000771, 0.06873);
    Vector3<double> r0(-0.004511, -0.003068,-0.06568); 
    Vector3<double> center(0.0,0.0,0.0); 
    Vector3<double> linf(0.0,0.0,9999); 
    Vector3<double> tinf(0.0,9999,0.0); 
    ListeningPosition.push_back(l0);
    ListeningPosition.push_back(r0);
    ListeningPosition.push_back(center);
    ListeningPosition.push_back(linf);
    ListeningPosition.push_back(tinf);


    uint N = ListeningPosition.size();


    for (uint i=0; i<N; i++) 
    {
        cout << "working on listening position " << i << " at " << ListeningPosition[i] << endl;
        vector<double> current_s1sum(Nts_,0.0); 
        vector<double> current_s2sum(Nts_,0.0); 

        for (uint j=0; j<NCell_; j++) 
        {
            double r = (ListeningPosition[i] - vertlist[j].position).norm();
            uint t_shift = floor(r/340.0*5000); // quantized t_shift
            //cout << "for cell " << j << ", the time delay is " << t_shift << endl;
            for (uint k=0; k<Nts_; k++)
            {
                if (k>=t_shift) // if smaller than padded with zero
                {
                    uint kk = k-t_shift;
                    //cout << "kk = " << kk << endl;
                    current_s1sum[kk] += -1.0/4.0/3.1415926*vertlist[j].source1[kk]/r*vertlist[j].A_Voronoi_sum;
                    current_s2sum[kk] += -1.0/4.0/3.1415926*vertlist[j].source2[kk]/r*vertlist[j].A_Voronoi_sum;
                }
            }
        }

        source1_s = current_s1sum;
        source2_s = current_s2sum;


        if (writeSourcesSum_) 
            writeSourcesSum(ListeningPosition[i]);

    }



    
}


/* 
 * Sum of the sound sources weighted by the vertex Voronoi area
 */
void surface::sumSources_areaWeighted_SphereHeadG() { 

    cout << "sum the sources weighted with Voronoi area using spherical head Green's function" << endl;
    cout << "Warning: not yet implmented." << endl;

}
















