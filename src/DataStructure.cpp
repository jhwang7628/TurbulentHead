#include <iostream>
#include <fstream> 
#include "DataStructure.h" 

int surface::findClosest(const vert & v)
{
    double minDistance = 99999;
    double EPS = 1E-6;
    int vertIndex = 999999999;
    bool modified = false;
    int count_vert = 0;
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
        count_vert ++; 
        cerr << "Warning: Not able to find matching vertex." << endl;
        cerr << "Target vertex position = " 
             << v.position << endl;
        cerr << "Recording the convergence in out/DEBUG_non-matched-vertex-list.txt" << endl;
        ofstream outf("out/DEBUG_non-matched-vertex-list.txt", ios::app);
        outf << "============== Vertex " << count_vert << "th discovered ================== "<< endl;
        outf << "Target vertex detailed information: " << endl;
        outf << "----------------------------------- " << endl;
        outf << "Position = " << v.position << endl;
        outf << "======================================================= " << endl;
        outf << "Iteration Updated_vertex Current_Distance " << endl;
        int count = 0; 
        double currentDistance;
        for (uint i=0; i<vertlist.size(); i++) 
        {
            currentDistance = v.EuclideanDistance(vertlist[i]);
            if (currentDistance < minDistance && currentDistance) 
            {
                count ++; 
                modified = true;
                minDistance = currentDistance; 
                vertIndex = i;
                //cout << "Current Distance = " << minDistance << endl;
                //cout << "Updated at vertex " << i << endl; 
                outf << count << " " << i << " " << minDistance << endl;
            }
        }
        cerr << "For this vertex, distance " << currentDistance << " is used. " << endl;
        outf << "For this vertex, distance " << currentDistance << " is used. " << endl;
        //if (true)
        //{
        //    cerr << "Error: Not able to find matching vertex. Exiting." << endl;
        //    exit(1);
        //}
        outf.close();
    }

    return vertIndex; 
}

void surface::setSimDim(const uint & NCell, const uint & Nts) 
{
    this->NCell_ = NCell; 
    this->Nts_   = Nts; 
}
