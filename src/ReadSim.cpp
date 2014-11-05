#include <iostream> 
#include <fstream> 
#include <sstream> 
#include <string> 
#include "ReadSim.h" 
#include "Vector3.h" 
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <stdio.h>
#include <algorithm>
#include <cstdlib>

using namespace std;


void ReadSim(vector<vertexData> & data) {

    // each vertex is an object
    //vector<vertexData> data;

    /* Read lists from directory */
    //string dir = string("../testData");
    string dir;
    string dir_out;
    cout << "Where are the surfacepressure data? input directory path: " << endl;
    getline(cin, dir);
    cout << "Select output directory:" << endl;
    getline(cin, dir_out);
    vector<string> filenames = vector<string>();

    getdir(dir,filenames);

    //unsigned int Nts = filenames.size();
    //unsigned int file_start = 500;
    //Nts = 100;
    //Nts = filenames.size()-file_start; 

    //file_start = 0;
    //Nts = filenames.size();


    string file_start_string; 
    cout << "input file_start: " << endl;
    getline(cin,file_start_string);
    unsigned int file_start = atoi(file_start_string.c_str()); 
    unsigned int Nts = filenames.size();

    if (file_start + Nts > filenames.size())
    {
        Nts = filenames.size() - file_start;
        cout << "file_start = " << file_start << endl;
        cout << "Nts = " << Nts << endl;
    }


    ifstream fin((dir+"/"+filenames[file_start]).c_str(),ios::in);
    if (fin.is_open()) 
    {
        cout << "File " << filenames[file_start] << " opened successfully. " << file_start+1 << "/" << Nts+file_start << endl;
    }
    else 
    { 
        cerr << "Cannot read file " << filenames[file_start] << ". Aborting.." << endl; exit(1); 
    }

    //ifstream curvin("../tools/curvature/computedK.txt");
    ifstream curvin("computedK.txt");

    if (!curvin) { cerr << " cannot open file computedK.txt " << endl; exit(1);  }

    /* Read time data */
    string header, line, line_curv;
    getline(fin,header);
    unsigned int NCell = 0;
    while (getline(fin,line))
    { 
        getline(curvin, line_curv); 
        vertexData currentData;
        istringstream ssin(line); 
        istringstream ssin_curv(line_curv); 
        int node;
        double x,y,z; 
        double k; 
        double nz,ny,nx; 
        double dpz,dpy,dpx;
        double pressure; 
        ssin >> node;
        ssin >> x >> y >> z; 
        ssin >> k; 

        double doubleBuff;

        // the recomputed curvature using the discrete implementation
        ssin_curv >> doubleBuff >> doubleBuff >> doubleBuff >> k;

        //cout << "read in curvature k: " << k << endl;
        // be careful about the order of fluent output data
        ssin >> nz >> ny >> nx;
        ssin >> dpz >> dpy >> dpx; 
        ssin >> pressure; 

        Vector3<double> vertexPos(x,y,z);
        Vector3<double> vertexNor(nx,ny,nz);
        Vector3<double> vertexCurrentGradP(dpx,dpy,dpz);
        double vertexK = k; 

        currentData.positions = vertexPos;
        currentData.normals   = vertexNor;
        currentData.curvature = vertexK; 


        // reading different time step
        currentData.pressure.push_back(pressure);
        currentData.gradP.push_back(vertexCurrentGradP);

        // put the currentData into data structure
        data.push_back(currentData);

        NCell++;

    } 

    fin.close();
    curvin.close();


    /* Read subsequent time data */
    //for (unsigned int i=1; i<5; i++) 
    for (unsigned int i=file_start+1; i<Nts+file_start; i++) 
    {
        ifstream fin((dir+"/"+filenames[i]).c_str(),ios::in);
        if (fin.is_open()) 
        {
            cout << "File " << filenames[i] << " opened successfully. " << i+1 << "/" << Nts+file_start << endl;
        }
        else 
        { 
            cerr << "Cannot read file " << filenames[i] << ". Aborting.." << endl; exit(1); 
        }

        getline(fin,header);
        int count = 0; 
        while (getline(fin,line))
        { 
            istringstream ssin(line); 
            int node;
            double x,y,z; 
            double k; 
            double nz,ny,nx; 
            double dpz,dpy,dpx;
            double pressure; 
            ssin >> node;
            ssin >> x >> y >> z; 
            ssin >> k; 
            // be careful about the order of fluent output data
            ssin >> nz >> ny >> nx;
            ssin >> dpz >> dpy >> dpx; 
            ssin >> pressure; 
        
            Vector3<double> vertexCurrentGradP(dpx,dpy,dpz);
        
            data[count].pressure.push_back(pressure);
            data[count].gradP.push_back(vertexCurrentGradP);
        
            count ++;
        
        } 
        fin.close();
    }

    FILE *fP, *fpos, *fs1, *fs2, *fnor, *fgradP; 
    string fs1_path = dir_out + "/source1.txt";
    string fs2_path = dir_out + "/source2.txt";
    //fP    = fopen("../out/pressure.txt" ,"w"); 
    //fgradP= fopen("../out/gradP.txt" ,"w"); 
    //fpos  = fopen("../out/positions.txt","w");
    //fnor  = fopen("../out/normals.txt"  ,"w");
    fs1   = fopen(fs1_path.c_str()  ,"w"); 
    fs2   = fopen(fs2_path.c_str()  ,"w"); 


    //cout << "NCell is " << NCell << endl; 
    //cout << "Nts is " << Nts << endl;
    for (unsigned int i=0; i<NCell; i++) 
    {
        cout << "cell " << i << endl; 
        cout << data.size() << endl;
        data[i].computeSource1(); 
        data[i].computeSource2(); 
        for (unsigned int j=0; j<Nts; j++)
        {
            //cout << "cell" << i << " source1: " << data[i].source1[j] << " " ; 
            //cout << "cell" << i << " source2: " << data[i].source2[j] ; 
            fprintf(fs1, "%.9f ", data[i].source1 [j]);
            fprintf(fs2, "%.9f ", data[i].source2 [j]);
        //    fprintf(fP , "%.9f ", data[i].pressure[j]);
        //    fprintf(fgradP, "%.9f %.9f %.9f\n", data[i].gradP[j].x
        //                                      , data[i].gradP[j].y
        //                                      , data[i].gradP[j].z);
        //    //fs1 << data[i].source1[j] << " " ; 
        //    //fs2 << data[i].source2[j] << " " ; 

        }
        ////cout << endl;
        fprintf(fs1, "\n");
        fprintf(fs2, "\n");
        //fprintf(fP , "\n");
        //fs1  << endl; 
        //fs2  << endl;
        //fpos << data[i].positions << endl; 
        //fprintf(fpos, "%.9f %.9f %.9f\n", data[i].positions.x
        //                              , data[i].positions.y
        //                              , data[i].positions.z);
        //fprintf(fnor, "%.9f %.9f %.9f\n", data[i].normals.x
        //                              , data[i].normals.y
        //                              , data[i].normals.z);
    }

    fclose(fs1);
    fclose(fs2);


    //for (unsigned int i=0; i<NCell; i++)
    //{
    //    //data[i].printPressure(); 
    //    cout << "curvature " << i << " is " << data[i].curvature << endl;
    //}



    //cout << "Print out the source 1" << endl;
    //cout << data[0].source1.size() << endl;

    //cout << data[0].source1[0] << endl
    //     << data[0].source1[1] << endl;





}


// source 1: dp/dn
void vertexData::computeSource1() 
{
    unsigned int Nts = pressure.size();

    for (unsigned int i=0; i<Nts; i++) 
    {
        source1.push_back(gradP[i].dot(normals.normalize()));
    }

}

// source 2: p*curvature
void vertexData::computeSource2()
{

    unsigned int Nts = pressure.size();

    for (unsigned int i=0; i<Nts; i++) 
    {
        source2.push_back(pressure[i]*curvature);
    }
}


// implementation for readVertexData
void Data::readVertexData()
{

    ifstream fposin("../out/positions.txt",ios::binary);
    ifstream fs1in ("../out/source1.txt",ios::binary); 
    ifstream fs2in ("../out/source2.txt",ios::binary); 

    string line_pos, line_s1, line_s2; 

    unsigned int i = 0; 
    while (getline(fs1in,line_s1)) 
    {
        vertexData tmpVertexData;

        getline(fs2in,line_s2);
        getline(fposin,line_pos); 

        istringstream iss_s1 (line_s1); 
        istringstream iss_s2 (line_s2); 
        istringstream iss_pos(line_pos); 

        double s1Buff,s2Buff,posBuffx,posBuffy,posBuffz; 

        unsigned int j = 0; 
        while (iss_s1 >> s1Buff) 
        {
            iss_s2 >> s2Buff; 
            //cout << "current time step" << Nts << endl; 
            tmpVertexData.source1.push_back(s1Buff); 
            tmpVertexData.source2.push_back(s2Buff); 
            j++;
        }
        Nts = j;

        iss_pos >> posBuffx; iss_pos >> posBuffy; iss_pos >> posBuffz;
        tmpVertexData.positions = Vector3<double>(posBuffx,posBuffy,posBuffz);


        allVertexData.push_back(tmpVertexData);

        i++;
    }

    NCell = i;


}


/* Getting the filenames in the directory that starts with "Pressure" */
int getdir (string dir, vector<string> &files)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        cerr << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL) {

        string dname = string(dirp->d_name); 

        if (dname.substr(0,8) == "Pressure")
        {
            files.push_back(dname);
        }
    }

    // sort the file names
    std::sort(files.begin(), files.end());

    closedir(dp);
    return 0;
}
