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
#include <algorithm>

using namespace std;

int getdir (string dir, vector<string> &files);

int main() {

    // each vertex is an object
    vector<vertexData> data;


    /* Read lists from directory */
    string dir = string("../testData");
    vector<string> filenames = vector<string>();

    getdir(dir,filenames);

    unsigned int NTimeSteps = filenames.size();


    ifstream fin((dir+"/"+filenames[0]).c_str(),ios::in);
    if (fin.is_open()) 
    {
        cout << "File " << filenames[0] << " opened successfully. " << 1 << "/" << NTimeSteps << endl;
    }
    else 
    { 
        cerr << "Cannot read file " << filenames[0] << ". Aborting.." << endl; exit(1); 
    }

    /* Read time data */
    string header, line;
    getline(fin,header);
    unsigned int NCell = 0;
    while (getline(fin,line))
    { 
        vertexData currentData;
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


    /* Read subsequent time data */
    //for (unsigned int i=1; i<5; i++) 
    for (unsigned int i=1; i<NTimeSteps; i++) 
    {
        ifstream fin((dir+"/"+filenames[i]).c_str(),ios::in);
        if (fin.is_open()) 
        {
            cout << "File " << filenames[i] << " opened successfully. " << i+1 << "/" << NTimeSteps << endl;
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



    for (unsigned int i=0; i<NCell; i++) 
    {
        data[i].computeSource1(); 
        data[i].computeSource2(); 
    }


    return 0;




}

double vertexData::computeSource1() 
{
    unsigned int Nts = pressure.size();

    for (int i=0; i<Nts; i++) 
    {
        cout << "acoustic analogy 1: " << i << endl;
        source1.push_back(gradP[i].dot(normals.normalize()));
    }
}

double vertexData::computeSource2()
{

    unsigned int Nts = pressure.size();

    for (int i=0; i<Nts; i++) 
    {
        cout << "acoustic analogy 2: " << i << endl;
        source2.push_back(pressure[i]*curvature);
    }
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
