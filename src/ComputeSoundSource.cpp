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
   
    if (argc < 2) 
    {
        cout << "usage: ./ComputeSoundSource mshName(in mesh/fluentInterface folder) SurfacePressure_path file_start_string(uint)" << endl;
        exit(1);
    }
    

    Mesh mesh(argv[1], argv[2], argv[3]);
    
    mesh.readMesh();
    mesh.ExtractSurface();

    /* Fluent Simulation data reader */
    mesh.extractedSurface->ReadSimulation();
    
    /* Curvature calculation */ 
    mesh.extractedSurface->computeK();

    /* Acoustic sources calculation */ 
    mesh.extractedSurface->computeVertVoronoi(); // for surface intergral 
    mesh.extractedSurface->computeDipole();
    mesh.extractedSurface->sumSources();

    mesh.extractedSurface->writeK();
    mesh.extractedSurface->writeVertVoronoi(); 
    mesh.extractedSurface->writePostInfo();
    //mesh.extractedSurface->writeSources();
    mesh.extractedSurface->writeSourcesSum(); 
    mesh.extractedSurface->printOBJ("./out/"+string(argv[1])+".obj");
    mesh.extractedSurface->print2WaveSolver("./out/wavesolver_input");





    
    /* OpenGL rendering */
    
    
    return 0;
}
