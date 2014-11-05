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
    
    string tmp ("jhwang.msh");
    string tmp2("computedK.txt");
    Mesh mesh(tmp);
    
    mesh.readMesh();
    mesh.ExtractSurface();
    mesh.extractedSurface->printOBJ("./out/"+tmp+".obj");
    
    /* Curvature calculation */ 
    mesh.extractedSurface->computeK();
    mesh.extractedSurface->writeK("./out/"+tmp2);

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
