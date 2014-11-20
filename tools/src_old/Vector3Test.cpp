#include <iostream> 
#include <vector>

#include "Vector3.h"

using namespace std;

int main() { 


    Vector3<double> va(1.2,1.1,2.2),vb(1.8,2.2,3.11);


    cout << va << endl << vb << endl; 
    cout << "add: " << va.add(vb) << endl;
    cout << "add: " << va.add(1.0,1.0,1.0) << endl;

    cout << va << endl << vb << endl; 
    Vector3<double> T = va+vb; 
    cout << "not-inplace add: " << T << endl;
    cout << "is va the same as tmpVec3? " << (&T == &va) << endl;
    T = va + 1.0;
    cout << "not-inplace add: " << T << endl;
    cout << "is va the same as tmpVec3? " << (&T == &va) << endl;

    cout << va << endl << vb << endl; 
    cout << "sub: " << va.sub(vb) << endl;
    cout << "sub: " << va.sub(1.0,1.0,1.0) << endl;

    cout << va << endl << vb << endl; 
    cout << "mul: " << va.mul(vb) << endl;
    //cout << "mul: " << va.mul(2.0,3.0,4.0) << endl;

    cout << va << endl << vb << endl; 
    cout << "div: " << va.div(vb) << endl;
    cout << "div: " << va.div(2.0,3.0,4.0) << endl;

    cout << va << endl << vb << endl; 
    cout << "dot: " << va.dot(vb) << endl;

    cout << va << endl << vb << endl; 
    cout << "cross: " << va.cross(vb) << endl;

    cout << va << endl; 
    cout << "normSq: " << va.normSq() << endl;
    
    cout << va << endl; 
    cout << "norm: " << va.norm() << endl;

    cout << va << endl; 
    cout << "normalization: " << va.normalize() << endl;

    Vector3<double> vc;
    vc.push_back(0.0);vc.push_back(1.0);vc.push_back(2.0);
    cout << &vc << endl;
    cout << &vc+1 << endl;
    cout << *(&vc) << endl;
    cout << *(&vc+1) << endl;


    vector<Vector3<double> > test; 


    cout << va << endl << vb << endl; 
    test.push_back(va);
    test.push_back(vb); 

    cout << test[0] << endl << test[1] << endl; 






    return 0;




}
