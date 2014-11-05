#include <iostream>
#include "DataStructure.h" 

int surface::findClosest(const vert & v)
{
    double minDistance = 99999;
    double EPS = 1E-6;
    int vertIndex = 999999999;
    bool modified = false;
    for (uint i=0; i<vertlist.size(); i++) 
    {
        double currentDistance = v.EuclideanDistance(vertlist[i]);
        if (currentDistance < minDistance && currentDistance < EPS) 
        {
            modified = true;
            minDistance = currentDistance; 
            vertIndex = i;
        }
    }
    if (!modified)
    {
        cerr << "Warning: Not able to find matching vertex." << endl;
        cerr << "Target vertex position = " 
             << v.position << endl;
        exit(1);
    }

    return vertIndex; 
}

void surface::setSimDim(const uint & NCell, const uint & Nts) 
{
    this->NCell_ = NCell; 
    this->Nts_   = Nts; 
}
