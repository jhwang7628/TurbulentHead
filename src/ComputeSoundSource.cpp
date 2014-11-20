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
#include <gflags/gflags.h>
#include "DataStructure.h"

using namespace std; 

static const double PI=3.14159265359;


DEFINE_string(lppath, "none", "File path for Listening Position. ");
DEFINE_string(s1path, "none", "If postprocessing was run on this case, point us to the path of the output directory (the one that contains 'source1.txt', for example). "); 

int main(int argc, char** argv)
{

    google::ParseCommandLineFlags(&argc, &argv, true);

    
    /* Mesh reader */ 
   
    if (argc < 2) 
    {
        cout << "usage: ./ComputeSoundSource mshName(in mesh/fluentInterface folder) SurfacePressure_path file_start_string(uint) listeningPostion_path(blank if none) source1.txt_path(blank if none)" << endl;
        exit(1);
    }
    

    Mesh mesh(argv[1], argv[2], argv[3]);

    if (FLAGS_s1path.compare("none") == 0) // not using s1path
    {
        mesh.readMesh();
        mesh.ExtractSurface();

        /* Fluent Simulation data reader */
        mesh.extractedSurface->ReadSimulation();

        /* Curvature calculation */ 
        mesh.extractedSurface->computeK();

        /* Acoustic sources calculation */ 
        mesh.extractedSurface->computeVertVoronoi(); // for surface intergral 
        mesh.extractedSurface->computeDipole();


        /* Write important information */
        mesh.extractedSurface->writeK();
        mesh.extractedSurface->writeVertVoronoi(); 
        mesh.extractedSurface->writeSources();
        mesh.extractedSurface->printOBJ("./out/"+string(argv[1])+".obj");
        mesh.extractedSurface->print2WaveSolver("./out/wavesolver_input");
    }
    else 
    {
        mesh.extractedSurface = new surface(mesh.dir, mesh.file_start_string);
        string outdir_path = FLAGS_s1path; 
        //string outdir_path = argv[4];
        mesh.extractedSurface->ReadFromOut(outdir_path);
    }


    mesh.extractedSurface->set_lppath(FLAGS_lppath);
    mesh.extractedSurface->sumSources();
    mesh.extractedSurface->writePostInfo();

    //mesh.extractedSurface->writeSourcesSum_ = false;
    //mesh.extractedSurface->writeSourcesSum(); 
    
    /* OpenGL rendering */
    
    
    return 0;
}
