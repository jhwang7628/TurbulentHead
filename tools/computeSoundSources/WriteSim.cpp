#include "WriteSim.h" 
#include "ReadSim.h" 

#include <vector>
//#include <fstream>
//#include <stdio.h>

//using namespace std; 

void WriteSim(vector<vertexData> & data) {

    //unsigned int NCell = data.size();
    //unsigned int Nts = data[0].pressure.size();

    //FILE *fP, *fpos, *fs1, *fs2, *fnor, *fgradP; 
    //fP    = fopen("../out/pressure.txt" ,"w"); 
    //fgradP= fopen("../out/gradP.txt"    ,"w"); 
    //fpos  = fopen("../out/positions.txt","w");
    //fnor  = fopen("../out/normals.txt"  ,"w");
    //fs1   = fopen("../out/source1.txt"  ,"w"); 
    //fs2   = fopen("../out/source2.txt"  ,"w"); 


    //for (unsigned int i=0; i<NCell; i++) 
    //{ 

    //    for (unsigned int j=0; j<Nts; j++) 
    //    {
    //        fprintf(fP , "%.9f ", data[i].pressure[j]);
    //        fprintf(fgradP, "%.9f %.9f %.9f\n", data[i].gradP[j].x
    //                                          , data[i].gradP[j].y
    //                                          , data[i].gradP[j].z);
    //        fprintf(fs1, "%.9f ", data[i].source1 [j]);
    //        fprintf(fs2, "%.9f ", data[i].source2 [j]);
    //    }


    //    fprintf(fpos, "%.9f %.9f %.9f\n", data[i].positions.x
    //                                  , data[i].positions.y
    //                                  , data[i].positions.z);
    //    fprintf(fnor, "%.9f %.9f %.9f\n", data[i].normals.x
    //                                  , data[i].normals.y
    //                                  , data[i].normals.z);
    //}

}

