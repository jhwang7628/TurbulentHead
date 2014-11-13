#include <iostream> 
#include <fstream> 
#include <sstream> 
#include <string> 
#include "DataStructure.h" 
#include "Vector3.h" 
#include <vector>
#include <stdio.h>
#include <cstdlib>

void surface::writeK()
{

    cout << "write curvature" << endl;

    string Kname("out/computedK.txt");
    FILE * pFile; 

    pFile = fopen(Kname.c_str(), "w");

    if (pFile == NULL) 
    {
        cerr << "Cannot open file " + Kname << ". Exiting" << endl;
        exit(1);                                                          
    }

    for (unsigned int i=0; i<vertlist.size(); i++)
    {
        fprintf(pFile, "%.9E %.9E %.9E ", vertlist[i].position.x,
                                          vertlist[i].position.y,
                                          vertlist[i].position.z);
        fprintf(pFile, "%.9f\n", vertlist[i].curvature);
    }

    /* if matching is needed... 
     *
    string line;
    int countPoint = 0;
    int countoutlier = 0;
    getline(in,line); 
    while(getline(in,line))
    //for (int j=0; j<3000; j++)
    {
        //getline(in,line);
        //out << "==========================" << endl;
        //out << countPoint << endl;
        cout << countPoint << endl;
        Vector3<double> fluentCoord; 
        int nodenumber; 
        istringstream iss(line); 
        iss >> nodenumber;
        iss >> fluentCoord.x >> fluentCoord.y >> fluentCoord.z;

        unsigned int bestInd=0; 
        for (unsigned int i=0; i<vertlist.size(); i++)
        {
                if ((vertlist[i].position - fluentCoord).normSq() < 1e-12)
                {
                    bestInd = i; 
                    fprintf(pFile, "%.9E %.9E %.9E ", fluentCoord.x,fluentCoord.y,fluentCoord.z);
                    fprintf(pFile, "%.9f\n", vertlist[i].curvature);
                }
        }
        if (bestInd == 0)
        {
            countoutlier ++; 
            cout << "vertex " << countPoint << " is an outlier. "  << endl;
        }

        //cout << "bestmatch for vertex " << countPoint << " is vertex number " << bestInd << endl;
        //out << countPoint << " " << bestInd << endl;
        countPoint++;
    }

    /cout << "outlier = " << countoutlier << endl;


    */

    fclose(pFile);




}

void surface::writePressure()
{
    std::cout << "write pressure" << std::endl;
    //FILE *fP, *fpos, *fs1, *fs2, *fnor, *fgradP; a
    FILE *fP; 
    std::string fp_path  = "out/pressure.txt";

    fP    = fopen( fp_path.c_str()  ,"w"); 

    for (uint i=0; i<this->NCell_; i++) 
    {
        for (uint j=0; j<this->Nts_; j++)
        {
            fprintf(fP , "%.9f ", vertlist[i].pressure[j]);
        }
        fprintf(fP,  "\n");
    }

    fclose(fP);

}

void surface::writeSources()
{

    std::cout << "write sources" << std::endl;
    //FILE *fP, *fpos, *fs1, *fs2, *fnor, *fgradP; a
    FILE *fs1, *fs2;
    std::string fs1_path = "out/source1.txt";
    std::string fs2_path = "out/source2.txt";

    fs1   = fopen(fs1_path.c_str()  ,"w"); 
    fs2   = fopen(fs2_path.c_str()  ,"w"); 

    for (uint i=0; i<this->NCell_; i++) 
    {
        for (uint j=0; j<this->Nts_; j++)
        {
            fprintf(fs1, "%.9f ", vertlist[i].source1[j]);
            fprintf(fs2, "%.9f ", vertlist[i].source2[j]);
        }
        fprintf(fs1, "\n");
        fprintf(fs2, "\n");
    }

    
    fclose(fs1);
    fclose(fs2);
}

void surface::writeSourcesSum()
{
    std::cout << "write sources sum" << std::endl;

    FILE *fs1_s, *fs2_s;

    std::string fs1_s_path = "out/source1_sum.txt";
    std::string fs2_s_path = "out/source2_sum.txt";

    fs1_s = fopen(fs1_s_path.c_str()  ,"w"); 
    fs2_s = fopen(fs2_s_path.c_str()  ,"w"); 
    for (uint j=0; j<this->Nts_; j++) 
    {
        fprintf(fs1_s, "%.9f ", source1_s[j]);
        fprintf(fs2_s, "%.9f ", source2_s[j]);
    }

    fclose(fs1_s);
    fclose(fs2_s);

}

void surface::writeSourcesSum(const Vector3<double> ListeningPosition)
{
    std::cout << "write sources sum with listening position" << std::endl;
    std::cout << "make sure there is no existing source*_sum.txt files. will cause problems. " << std::endl;

    FILE *fs1_s, *fs2_s, *fLP_s; 

    std::string fs1_s_path = "out/source1_sum.txt";
    std::string fs2_s_path = "out/source2_sum.txt";
    std::string fLP_path = "out/ListeningPosition.txt";

    if (!LPopened_) 
    {
        fs1_s = fopen(fs1_s_path.c_str(), "w"); 
        fs2_s = fopen(fs2_s_path.c_str(), "w"); 
        fLP_s = fopen(fLP_path.c_str()  , "w"); 
        LPopened_ = true;
    }
    else 
    {
        fs1_s = fopen(fs1_s_path.c_str(), "a"); 
        fs2_s = fopen(fs2_s_path.c_str(), "a"); 
        fLP_s = fopen(fLP_path.c_str()  , "a"); 
    }

    fprintf(fLP_s, "%.9f %.9f %.9f", ListeningPosition.x, ListeningPosition.y, ListeningPosition.z);
    for (uint j=0; j<this->Nts_; j++) 
    {
        fprintf(fs1_s, "%.9f ", source1_s[j]);
        fprintf(fs2_s, "%.9f ", source2_s[j]);
    }
    fprintf(fs1_s, "\n");
    fprintf(fs2_s, "\n");
    fprintf(fLP_s, "\n");

    fclose(fs1_s);
    fclose(fs2_s);
    fclose(fLP_s);

}

void surface::writeVertVoronoi()
{

    std::cout << "write Voronoi_sum " << std::endl;

    FILE *fV; 

    string fV_path = "out/vertVoronoi.txt"; 

    fV = fopen(fV_path.c_str(), "w"); 

    if (!fV) 
    {
        cerr << "Cannot open file " << fV_path << ". Return." << endl;
        return;
    }

        for (uint j=0; j<NCell_; j++) 
        {
            vertlist[j].computeA_Voronoi_Sum(); 
            //cout << "vert " << j << " has Voronoi area = " << vertlist[j].A_Voronoi_sum << endl;
            fprintf(fV, "vert %i has Voronoi area = %.9f \n", j, vertlist[j].A_Voronoi_sum);
        }


    fclose(fV);

}

void surface::writePostInfo() 
{
    FILE *f; 

    string f_path = "out/PostInfo.txt"; 

    f = fopen(f_path.c_str(), "w");

    if (!f) 
    {
        cerr << "Cannot open file " << f_path << ". Return. " << endl;
        return; 
    }

    fprintf(f, "Data Reading: \n"); 
    fprintf(f, "------------- \n"); 
    fprintf(f, "SurfacePressure directory: %s\n", dir_.c_str()); 
    fprintf(f, "Read Surface Pressure skip %s files\n", file_start_string_.c_str()); 
    fprintf(f, "\n\n");
    fprintf(f, "Postprocessing: \n"); 
    fprintf(f, "-------------- \n"); 
    fprintf(f, "Sum Method: %s\n", sum_method_.c_str());

}
