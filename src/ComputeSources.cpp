#include <iostream> 
#include "DataStructure.h"
#include "Vector3.h"

// compute the dipole sources
void surface::computeDipole() 
{
    cout << "computing dipole sources" << endl;
    for (uint i=0; i<this->NCell_; i++) 
    {
        for (uint j=0; j<this->Nts_; j++) 
        {
            // source 1: dp/dn
            vertlist[i].source1.push_back(vertlist[i].gradP[j].dot(vertlist[i].normal.normalize()));
            // source 2: p*curvature
            vertlist[i].source2.push_back(vertlist[i].pressure[j]*vertlist[i].curvature);
        }
    }
}



// compute the quadrupole sources
