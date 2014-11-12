#include "DataStructure.h" 
#include <iostream> 
#include <stdio.h>
#include <cstdlib> 

using namespace std;

void surface::sumSources() {

    std::cout << "sum the sources" << std::endl;

    if (Nts_ == 0 || NCell_ == 0) 
    {
        cerr << "the dimension of the sources have not been set. use the method setSimDim in the class surface to set the dimension before summing the sources. returning" << endl;
        return;
    }

    uint sum_method = 1;


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

        //cout << "sum of source 1 at timestep " << i << " = " << current_s1sum << endl;
        //cout << "sum of source 2 at timestep " << i << " = " << current_s2sum << endl;

    }


}


/* 
 * Sum of the sound sources weighted by the vertex Voronoi area
 */
void surface::sumSources_areaWeighted_FreeSpaceG() { 

    cout << "sum the sources weighted with Voronoi area using free space Green's function" << endl;



    
}


/* 
 * Sum of the sound sources weighted by the vertex Voronoi area
 */
void surface::sumSources_areaWeighted_SphereHeadG() { 

    cout << "sum the sources weighted with Voronoi area using spherical head Green's function" << endl;
    cout << "Warning: not yet implmented." << endl;
}
















