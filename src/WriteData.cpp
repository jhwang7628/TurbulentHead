#include <iostream> 
#include <fstream> 
#include <sstream> 
#include <string> 
#include "DataStructure.h" 
#include "Vector3.h" 
#include <vector>
#include <stdio.h>
#include <cstdlib>

void surface::writeData()
{

    std::cout << "write sources" << std::endl;
    //FILE *fP, *fpos, *fs1, *fs2, *fnor, *fgradP; a
    FILE *fs1, *fs2, *fs1_s, *fs2_s;
    FILE *fP; 
    std::string fs1_path = "out/source1.txt";
    std::string fs2_path = "out/source2.txt";
    std::string fs1_s_path = "out/source1_sum.txt";
    std::string fs2_s_path = "out/source2_sum.txt";
    std::string fp_path  = "out/pressure.txt";

    fs1   = fopen(fs1_path.c_str()  ,"w"); 
    fs2   = fopen(fs2_path.c_str()  ,"w"); 
    fs1_s = fopen(fs1_s_path.c_str()  ,"w"); 
    fs2_s = fopen(fs2_s_path.c_str()  ,"w"); 
    fP    = fopen( fp_path.c_str()  ,"w"); 

    for (uint i=0; i<this->NCell_; i++) 
    {
        for (uint j=0; j<this->Nts_; j++)
        {
            fprintf(fs1, "%.9f ", vertlist[i].source1[j]);
            fprintf(fs2, "%.9f ", vertlist[i].source2[j]);
            fprintf(fP , "%.9f ", vertlist[i].pressure[j]);
        }
        fprintf(fs1, "\n");
        fprintf(fs2, "\n");
        fprintf(fP, "\n");
    }


    
    fclose(fs1);
    fclose(fs2);
    fclose(fs1_s);
    fclose(fs2_s);
    fclose(fP);
}
