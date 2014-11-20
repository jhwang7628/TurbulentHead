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

using namespace std; 

// read sorted data
void HD_ReadSim(vector<vertexData> & data){

    string file_p       = "../out/pressure.txt"; 
    string file_pos     = "../out/positions.txt"; 
    string file_nor     = "../out/normals.txt"; 
    string file_gradp   = "../out/gradP.txt"; 
    string file_source1 = "../out/source1.txt"; 
    string file_source2 = "../out/source2.txt"; 

    ifstream fP      (file_p      .c_str()); 
    ifstream fPos    (file_pos    .c_str()); 
    ifstream fNor    (file_nor    .c_str()); 
    ifstream fGradP  (file_gradp  .c_str()); 
    ifstream fSource1(file_source1.c_str()); 
    ifstream fSource2(file_source2.c_str()); 

    string lineP      ; 
    string linePos    ; 
    string lineNor    ; 
    string lineGradP  ; 
    string lineSource1; 
    string lineSource2; 


    while (getline(fP,lineP))
    {
        getline(fPos    ,linePos    );
        getline(fNor    ,lineNor    );
        getline(fGradP  ,lineGradP  );
        getline(fSource1,lineSource1);
        getline(fSource2,lineSource2);





    }





}
