#ifndef READSIM_H
#define READSIM_H

#ifndef VECTOR3_H 
#include "Vector3.h"
#endif

#include <vector> 


class vertexData;
class Data; 

class vertexData {
    public: 

        // -------------- Fields ----------------- //

        // static fields (not changing with time) 
        Vector3<double> positions; 
        Vector3<double> normals; 
        double curvature;

        // time-varying fields
        std::vector<double> pressure; 
        std::vector<Vector3<double> > gradP; 
        std::vector<double> source1; // acoustic analogy source from p_n
        std::vector<double> source2; // acoustic analogy source from p*k

        vertexData(){}


        // -------------- Helpers Methods -------------- //
          
          
        // print pressure
        void printPressure() const
        {

            std::cout << "Pressure (t): " << std::endl;
            std::cout << "------------- " << std::endl;
            for (int i=0; i<this->pressure.size(); i++) 
            {
                std::cout << pressure[i] << std::endl; 
            }
        }

        void printPressure(std::ostream& os) const
        {

            os << "Pressure (t): " << std::endl;
            os << "------------- " << std::endl;
            for (int i=0; i<this->pressure.size(); i++) 
            {
                os << pressure[i] << std::endl; 
            }
        }

        // print pressure gradient
        void printGradP() const
        {

            std::cout << "Grad(Pressure) (t): " << std::endl;
            std::cout << "------------------- " << std::endl;
            for (int i=0; i<this->pressure.size(); i++) 
            {
                std::cout << gradP[i] << std::endl; 
            }
        }

        // print pressure gradient
        void printGradP(std::ostream& os) const
        {

            os << "Grad(Pressure) (t): " << std::endl;
            os << "------------------- " << std::endl;
            for (int i=0; i<this->pressure.size(); i++) 
            {
                os << gradP[i] << std::endl; 
            }
        }

        double computeSource1();

        double computeSource2();

        // -------------- Operator Overload ------------------ //
        
        friend std::ostream &operator<<(std::ostream& os, const vertexData& V)
        {
            os << std::endl;

//            os << "=== Vertex final fields ===" << std::endl;
            os << "Vertex Position : " << V.positions << std::endl; 
            os << "Vertex Normals  : " << V.normals   << std::endl; 
            os << "Vertex Curvature: " << V.curvature << std::endl;

//            os << "=== Vertex time-varying fields ===" << std::endl;
            V.printPressure(os); 
            V.printGradP(os);
            
            return os;

        }

};

// Data for each configuration
class Data { 
    private: 
        const char* SimSource;
        unsigned int NCell; 

    public: 
//        std::vector<vertexData > vertexdata; 
//
//        void setVertexData(vertexData * vD) 
//        {
//            this->vertexdata.push_back(*vD); 
//        }
//
//        vertexData getVertexData(int ind) 
//        {
//            return vertexdata[ind];
//        }

        const char* getSimSource() 
        {
            return SimSource;
        }

        unsigned int getNCell() 
        { 
            return NCell; 
        }

        void setSimSource(const char* targetSimSource) 
        { 
            SimSource = targetSimSource; 
        }

        void setNCell(unsigned int NC) 
        { 
            NCell = NC; 
        }

        // constructor
        Data()
        {
            setSimSource("ANSYS Fluent");
            NCell = 0;
        }


};









#endif
