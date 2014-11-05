#include <iostream> 
#include <fstream> 
#include <sstream> 
#include <string> 
#include "DataStructure.h" 
#include "Vector3.h" 
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <stdio.h>
#include <algorithm>
#include <cstdlib>

using namespace std;


void surface::ReadSimulation() {

    cout << "read simulation data" << endl;

    /* Read lists from directory */
    //string dir = string("../testData");
    string dir;
    string dir_out;
    cout << "Where are the surfacepressure data? input directory path: " << endl;
    getline(cin, dir);
    //cout << "Select output directory:" << endl;
    //getline(cin, dir_out);
    dir_out = "out";
    vector<string> filenames = vector<string>();

    /* Getting the filenames in the directory that starts with "Pressure" */
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        cerr << "Error(" << errno << ") opening " << dir << endl;
        exit(1);
    }

    while ((dirp = readdir(dp)) != NULL) {

        string dname = string(dirp->d_name); 

        if (dname.substr(0,8) == "Pressure")
        {
            filenames.push_back(dname);
        }
    }

    // sort the file names
    std::sort(filenames.begin(), filenames.end());

    closedir(dp);

    
   
    /* prompt file reading range */
    string file_start_string; 
    cout << "input file_start: (0 for reading entire data)" << endl;
    getline(cin,file_start_string);
    uint file_start = atoi(file_start_string.c_str()); 
    uint Nts = filenames.size();

    if (file_start + Nts > filenames.size())
    {
        Nts = filenames.size() - file_start;
        cout << "file_start = " << file_start << endl;
        cout << "Nts = " << Nts << endl;
    }


    vector<int> SearchTable;
    bool initializeSearch = true;
    ofstream FST("out/DEBUG_SearchTable.txt");

    /* Read subsequent time data */
    for (uint i=file_start; i<Nts+file_start; i++) 
    {

        ifstream fin((dir+"/"+filenames[i]).c_str(),ios::in);
        if (fin.is_open()) 
        {
            cout << "File " << filenames[i] << " opened successfully. " 
                 << i+1 << "/" << Nts+file_start << endl;
        }
        else 
        { 
            cerr << "Cannot read file " << filenames[i] << ". Aborting.." << endl; exit(1); 
        }

        if (initializeSearch) 
        {
            cout << "Constructing Search Table (matching fluent output data to fluent input mesh)" 
                 << endl; 
            FST << "Exported_nodes_index" << " " << 
                   "Stored_nodes_index"   << endl;
        }

        // get the header line 
        string header, line;
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
            double pres; 
            ssin >> node;
            ssin >> x >> y >> z; 
            ssin >> k; 
            // be careful about the order of fluent output data
            ssin >> nz >> ny >> nx;
            ssin >> dpz >> dpy >> dpx; 
            ssin >> pres; 

            vert newVert(x,y,z); 

            int iV;

            if (initializeSearch) 
            {
                iV = findClosest(newVert); 
                SearchTable.push_back(iV); 
                FST << count << " " << iV << endl;
            }
            else
            {
                iV = SearchTable[count]; 
            }

            if (initializeSearch) 
            {
                vertlist.at(iV).normal.x = nx;
                vertlist.at(iV).normal.y = ny;
                vertlist.at(iV).normal.z = nz;
            }
          
            vertlist.at(iV).pressure.push_back(pres); 
            Vector3<double> currentGradP(dpx,dpy,dpz);
            vertlist[count].gradP.push_back(currentGradP);

            count ++;
            
        } 
        if (initializeSearch) 
        {
            cout << "Search Table construction complete" 
                 << endl; 
        }
        initializeSearch = false; 
        fin.close();
    }

    FST.close();

}


