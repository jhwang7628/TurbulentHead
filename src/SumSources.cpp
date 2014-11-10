#include "DataStructure.h" 
#include <iostream> 

using namespace std;

void surface::sumSources() {

    std::cout << "sum the sources" << std::endl;

    if (Nts_ == 0 || NCell_ == 0) 
    {
        cerr << "the dimension of the sources have not been set. use the method setSimDim in the class surface to set the dimension before summing the sources. returning" << endl;
        return;
    }

    //sumSources_direct();
    sumSources_areaWeighted();


    std::cout << "sum the sources COMPLETED" << std::endl;


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


void surface::sumSources_areaWeighted() { 

    cout << "sum the sources weighted with Voronoi area" << endl;

    for (uint i=0; i<Nts_; i++)
    {
        double current_s1sum=0.0; 
        double current_s2sum=0.0;
        for (uint j=0; j<NCell_; j++) 
        {

            if (i==0)
            {
                vertlist[j].computeA_Voronoi_Sum(); 
                cout << "vert " << j << " has Voronoi area = " << vertlist[j].A_Voronoi_sum << endl;
            }

            current_s1sum += vertlist[j].source1[i]*vertlist[j].A_Voronoi_sum;
            current_s2sum += vertlist[j].source2[i]*vertlist[j].A_Voronoi_sum;
        }

        source1_s.push_back(current_s1sum); 
        source2_s.push_back(current_s2sum); 

        cout << "sum of source 1 at timestep " << i << " = " << current_s1sum << endl;
        cout << "sum of source 2 at timestep " << i << " = " << current_s2sum << endl;

    }


}
