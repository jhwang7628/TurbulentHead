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

/* 
 * Read sources and NCell_ and Nts_ from harddrive 
 */ 
void surface::ReadFromOut(const string dirout_path) { 

    cout << "read simulation data from previous pass at directory " << dirout_path << endl;


    // Read Postprocessing information
    cout << "reading postprocessing information " << endl;
    string path_post = dirout_path+"/PostInfo.txt";
    ifstream postinfo(path_post.c_str());
    if(!postinfo) 
    {
        cerr << "cannot open file PostInfo.txt" << endl;
        exit(1);
    }

    string line;
    while(getline(postinfo, line)) 
    {
        if (line.substr(0,6) == "NCell_")
        {
            istringstream iss(line); 
            string tmp, tmp2;
            uint buff; 
            iss >> tmp >> tmp2 >> buff; 
            this->NCell_ = buff;
        }
        if (line.substr(0,4) == "Nts_")
        {
            istringstream iss(line); 
            string tmp, tmp2;
            uint buff; 
            iss >> tmp >> tmp2 >> buff; 
            iss >> buff; 
            this->Nts_ = buff;
            break;
        }
    }
    postinfo.close();

    // Read Position and curvature of vertex
    cout << "reading position and curvature of vertices" << endl;
    string path_K = dirout_path + "/computedK.txt";
    ifstream ifk(path_K.c_str());
    if(!ifk) 
    {
        cerr << "cannot open file computedK.txt" << endl;
        exit(1);
    }

    string line_k;
    while(getline(ifk, line_k)) 
    {
        istringstream issk(line_k);
        double Buffx, Buffy, Buffz, Buffk; 
        issk >> Buffx >> Buffy >> Buffz >> Buffk;

        vert newVert(Buffx, Buffy, Buffz); 
        newVert.curvature = Buffk;
        vertlist.push_back(newVert);
    }
    ifk.close();

    // Read Vertex Voronoi area 
    cout << "reading vertex Voronoi area" << endl;
    string path_A = dirout_path + "/vertVoronoi.txt"; 
    ifstream ifA(path_A.c_str()); 
    if(!ifA) 
    {
        cerr << "cannot open file vertVoronoi.txt" << endl;
        exit(1);
    }

    string line_A;
    uint count = 0;
    getline(ifA,line_A);
    while(getline(ifA, line_A)) 
    {
        istringstream issA(line_A);
        double Buff, A;  
        issA >> Buff >> A;
        vertlist[count].A_Voronoi_sum = A; 
        count ++;
    }
    ifA.close();

    // Read sources
    cout << "reading vertex sound sources" << endl;
    string path_fs1 = dirout_path + "/source1.txt"; 
    string path_fs2 = dirout_path + "/source2.txt"; 
    ifstream ifs1(path_fs1.c_str()); 
    ifstream ifs2(path_fs2.c_str()); 
    if(!ifs1 || !ifs2) 
    {
        cerr << "cannot open file source1.txt or file source2.txt" << endl;
        exit(1);
    }

    string line_s1, line_s2; 
    count = 0;
    while(getline(ifs1,line_s1))
    {
        getline(ifs2,line_s2);
        istringstream iss1(line_s1); 
        istringstream iss2(line_s2);
        double doubBuff, doubBuff2; 
        vector<double> vertsource1;
        vector<double> vertsource2;
        while (iss1 >> doubBuff)
        {
            iss2 >> doubBuff2;
            vertsource1.push_back(doubBuff);
            vertsource2.push_back(doubBuff2);
        }
        vertlist[count].source1 = vertsource1; 
        vertlist[count].source2 = vertsource2; 

        count ++;
    }
    ifs1.close();
    ifs2.close();

}


void surface::ReadSimulation() {

    cout << "read simulation data" << endl;

    /* Read lists from directory */
    //string dir = string("../testData");
    //string dir;
    string dir_out;
    //cout << "Where are the surfacepressure data? input directory path: " << endl;
    //getline(cin, dir);
    //cout << "Select output directory:" << endl;
    //getline(cin, dir_out);
    dir_out = "out";
    vector<string> filenames = vector<string>();

    /* Getting the filenames in the directory that starts with "Pressure" */
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir_.c_str())) == NULL) {
        cerr << "Error(" << errno << ") opening " << dir_ << endl;
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
    //string file_start_string; 
    //cout << "input file_start: (0 for reading entire data)" << endl;
    //getline(cin,file_start_string);
    uint file_start = atoi(file_start_string_.c_str()); 
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

        ifstream fin((dir_+"/"+filenames[i]).c_str(),ios::in);
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

            //if (initializeSearch) 
            //{
            //    cout << "===================================" << iV << endl;
            //    cout << vertlist.at(iV).normal << endl;
            //    //vertlist.at(iV).normal.x = nx;
            //    vertlist.at(iV).normal.y = ny;
            //    vertlist.at(iV).normal.z = nz;
            //    vertlist.at(iV).normal.normalize();
            //    cout << vertlist.at(iV).normal << endl;
            //}
          
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

    // set NCell_ and Nts_ for this surface
    setSimDim(vertlist.size(), vertlist[0].pressure.size());

}


