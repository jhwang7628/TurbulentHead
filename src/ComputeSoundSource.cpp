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



DEFINE_string(listening_position, "none", "File path for Listening Position. ");
DEFINE_string(previous_run_dir, "none", "If postprocessing was run on this case, point us to the path of the output directory (the one that contains 'source1.txt', for example). "); 
DEFINE_string(module, "none", "run only modules of the code. supported options: writeOBJ");
DEFINE_int32(sum_method, 2, "define sum method: 0->direct, 1->area-weighted, 2->area-weighted and free space G, 3->area-weighted and spherical head G. (Default: 2)");


int main(int argc, char** argv)
{

    google::ParseCommandLineFlags(&argc, &argv, true);

    
    /* Mesh reader */ 
   
    if (argc < 2) 
    {
        cout << "usage: ./ComputeSoundSource mshName(location: mesh/fluentInterface) SurfacePressure_path file_start_string(uint) [--listening_position LISTENING_POSITION] [--previous_run_dir PREVIOUS_RUN_DIR] [--sum_method SUM_METHOD]. [--module MODULE] For options see --help" << endl;
        exit(1);
    }
    

    Mesh mesh(argv[1], argv[2], argv[3]);

    if (FLAGS_module.compare("writeOBJ") == 1)
    {
        mesh.readMesh(); 
        mesh.ExtractSurface();
        mesh.extractedSurface->printOBJ("./out/"+string(argv[1])+".obj");
        cout << "module 'writeOBJ' is turned on, finished run and exiting." << endl;
        exit(1); 
    }

    if (FLAGS_previous_run_dir.compare("none") == 0) // not using s1path
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
        string outdir_path = FLAGS_previous_run_dir; 
        //string outdir_path = argv[4];
        mesh.extractedSurface->ReadFromOut(outdir_path);
    }


    mesh.extractedSurface->set_lppath(FLAGS_listening_position);
    mesh.extractedSurface->sumSources();
    mesh.extractedSurface->writePostInfo();

    //mesh.extractedSurface->writeSourcesSum_ = false;
    //mesh.extractedSurface->writeSourcesSum(); 
    
    /* OpenGL rendering */
    
    
    return 0;
}
