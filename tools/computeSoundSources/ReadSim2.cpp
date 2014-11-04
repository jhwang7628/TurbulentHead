#include <iostream> 
#include <fstream> 
#include <sstream> 
#include <string> 
#include "ReadSim2.h" 
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


void ReadSim2(vector<vertexData> & data) {

    // each vertex is an object
    //vector<vertexData> data;

    /* Read lists from directory */
    //string dir = string("../testData");
    //string dir;
    //string dir_out;
    //cout << "Where are the surfacepressure data? input directory path: " << endl;
    //getline(cin, dir);
    //cout << "Select output directory:" << endl;
    //getline(cin, dir_out);
    //vector<string> filenames = vector<string>();

    //getdir(dir,filenames);

    //unsigned int Nts = filenames.size();
    //unsigned int file_start = 500;
    //Nts = 100;
    //Nts = filenames.size()-file_start; 

    //file_start = 0;
    //Nts = filenames.size();


    //string file_start_string; 
    //cout << "input file_start: " << endl;
    //getline(cin,file_start_string);
    //unsigned int file_start = atoi(file_start_string.c_str()); 
    //unsigned int Nts = filenames.size();

    //ifstream curvin("../tools/curvature/computedK.txt");
    ifstream curvin("computedK.txt");
    ifstream s1in("out/source1.txt");
    ifstream s2in("out/source2.txt");

    bool readAll = true;

    if (!curvin) { cerr << " cannot open file computedK.txt " << endl; exit(1);  }
    if (!s1in) { cerr << " cannot open file out/source1.txt " << endl; exit(1);  }
    if (!s2in) { cerr << " cannot open file out/source2.txt " << endl; exit(1);  }

    /* Read time data */
    string line, line_curv, line_s1, line_s2;
    unsigned int NCell = 0;
    while (getline(curvin,line_curv))
    { 
        cout << "reading cell " << NCell << endl;

        getline(s1in, line_s1);
        getline(s2in, line_s2);

        vertexData currentData;
        istringstream ssin_curv(line_curv); 
        istringstream ssin_s1(line_s1); 
        istringstream ssin_s2(line_s2); 
        double x,y,z; 
        double k; 
        double s1Buff, s2Buff;

        ssin_curv >> x >> y >> z; 
        ssin_curv >> k; 
        //cout << "curvature = " << k << endl;

        if (readAll) 
        {
            while (ssin_s1 >> s1Buff) 
            {
                ssin_s2 >> s2Buff; 
                currentData.source1.push_back(s1Buff);
                currentData.source2.push_back(s2Buff);
                //cout << s1Buff << " ";
            }
        }
        else 
        {
            for (int jj=0; jj<30; jj++)
            {
                ssin_s1 >> s1Buff; 
                ssin_s2 >> s2Buff; 
                currentData.source1.push_back(s1Buff);
                currentData.source2.push_back(s2Buff);
                //cout << s1Buff << " ";
            }
        }
        //cout << endl;


        //cout << "read in curvature k: " << k << endl;
        // be careful about the order of fluent output data
        //
        Vector3<double> vertexPos(x,y,z);
        double vertexK = k; 

        currentData.positions = vertexPos;
        currentData.curvature = vertexK; 


        // put the currentData into data structure
        data.push_back(currentData);

        NCell++;

    } 


    curvin.close();
    s1in.close();
    s2in.close();


}

