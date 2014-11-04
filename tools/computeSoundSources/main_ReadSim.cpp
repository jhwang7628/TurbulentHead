#include <iostream>
#include <vector> 
#include "Vector3.h"
#include "ReadSim.h" 

using namespace std; 

int main() {

    //vector<vertexData> data;

    //ReadSim(data); 
    //


    vector<vertexData> data2;
    ReadSim(data2);


    Data data; 

    data.readVertexData();


    return 0;


}
