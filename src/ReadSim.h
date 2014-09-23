#ifndef READSIM_H
#define READSIM_H

#ifndef LINEARALGEBRA_VECTOR_3_HPP
#include "linearalgebra/Vector3.hpp"
#endif

#include <vector> 


class fluentData {
    public: 

        Vector<Vector3> positions; 
        Vector<Vector3> normals; 
        Vector<Vector3> gradP; 
        Vector<float> pressure; 

        unsigned int NCell; 
} 



#endif
