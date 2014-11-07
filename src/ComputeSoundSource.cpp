/* Author: Jui-Hsien Wang 
 *         jw969@cornell.edu
 *
 * Date:   Nov 3rd, 2014
 *
 *
 */


#include <fstream> 
#include <sstream> 
#include <string> 
#include <vector>
#include <algorithm>
#include <stdio.h>
#include "DataStructure.h"

using namespace std; 

static const double PI=3.14159265359;

int main(int argc, char** argv)
{

    /* Mesh reader */ 
   
    string mshName;
    cout << "Input the msh name (excluding path; I will assume its in ./mesh/fluentInterface. Can't handle other situation): ";
    getline(cin, mshName);
    //string tmp ("jhwang.msh");
    string outK("computedK.txt");
    Mesh mesh(mshName);
    
    mesh.readMesh();
    mesh.ExtractSurface();
    mesh.extractedSurface->printOBJ("./out/"+mshName+".obj");
    
    /* Curvature calculation */ 
    mesh.extractedSurface->computeK();
    mesh.extractedSurface->writeK("./out/"+outK);

    /* Fluent Simulation data reader */
    mesh.extractedSurface->ReadSimulation();

    mesh.extractedSurface->setSimDim(mesh.extractedSurface->vertlist.size(),
                                     mesh.extractedSurface->vertlist[0].pressure.size());
    mesh.extractedSurface->print2WaveSolver("./out/wavesolver_input");

    /* Acoustic sources calculation */ 

    mesh.extractedSurface->computeDipole();
    mesh.extractedSurface->writeSources();

    
    /* OpenGL rendering */
    
    
    return 0;
}
