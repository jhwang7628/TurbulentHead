#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H

#include <iostream> 
#include <vector> 
#include "Vector3.h" 
#include <cmath>
#include <QGLViewer/qglviewer.h>

using namespace std; 


typedef unsigned int size_v;
typedef unsigned int uint; 


class Mesh; 
class surface; 
class tri; 
class vert; 
class Particle;


void assignK(Mesh *mesh);

class Mesh 
{
    public : 
        surface* meshSurface; 
        surface* extractedSurface;
        string mshName_;
        string dir; 
        string file_start_string; 
        void readMesh();
        void readOBJ(std::string filename);
        void ExtractSurface();
        Mesh(string f, string d, string fss) 
        {
            mshName_ = f;
            dir = d; 
            file_start_string = fss; 
        }
           
};

class surface  
{ 
    private : 
        string type; 
        string dir_; 
        string file_start_string_;
        string sum_method_;
        string lppath_; // listening position path
        uint input_sum_method_;
        bool LPopened_;
        uint NCell_; 
        uint Nts_; 
        vector<double> source1_s;
        vector<double> source2_s; 
        void sumSources_direct(); 
        void sumSources_areaWeighted(); 
        void sumSources_areaWeighted_FreeSpaceG();
        void sumSources_areaWeighted_FreeSpaceG_fromFile();
        void sumSources_areaWeighted_SphereHeadG();
    public : 
        vector<tri> trilist; 
        vector<vert> vertlist;
        int Nnanvert;
        bool writeSourcesSum_; 

        double maxK;

        // ---------- helper methods ------------ //
        void setSimDim(const uint &NCell, const uint &Nts);
        void setVar();

        void findShareTriangles(const int ind_xi, const int ind_xj, 
                                const vector<tri>& triRange, 
                                vector<double>& alpha);
        int findClosest(const vert & v);

        void ReadSimulation();
        void ReadFromOut(const string outdir_path);

        void computeK(); 
        void computeMaxK(); 
        void computeDipole();
        void computeVertVoronoi();

        void sumSources();
        void writeK();
        void writePressure();
        void writeSources();
        void writeSourcesSum();
        void writeSourcesSum(const Vector3<double> LP);
        void writeVertVoronoi();
        void writePostInfo();
        void printOBJ(string objname);
        void print2WaveSolver(string filename);
        void set_lppath(string lppath)
        {
            lppath_ = lppath; 
        }


        // ---------- Constructor ------------ //
        surface(string dir, string file_start_string)
        { 
            type="mesh";
            writeSourcesSum_ = true;
            LPopened_ = false; 
            maxK = 0.0;
            NCell_ = 0;
            Nts_ = 0;
            dir_ = dir;
            file_start_string_ = file_start_string;
            sum_method_ = "not recorded";
        }
        
        // ----------- Operator overload ---------- //
        friend std::ostream &operator<<(std::ostream& os, const surface* s)
        {
            //os << "{" << V.x << ", " << V.y << ", " << V.z << "}"; 
            os << "Surface type: " << s->type; 
            return os;
        }

};


class tri 
{
    public: 
        Vector3<int> index; // the index to the vertices
        Vector3<double> angle; // angle corresponding to each vertex of the triangle
        double area; 
        bool isObtuse; // one angle is greater than 90
        int ObtuseIndex;

        void printIndex() 
        {
            cout << "index of triangle = " << "(" << index.x << ", "  << index.y << ", " << index.z << ")" << endl;
        }

        bool equals(const tri& t) const
        {
            return (index.x == t.index.x && index.y == t.index.y && index.z == t.index.z);
        }

        void setEqual(tri t)
        { 
            angle = t.angle; 
            area  = t.area; 
            isObtuse = t.isObtuse; 
            ObtuseIndex = t.ObtuseIndex; 
        }

        tri()
        {
           area = 0; 
           isObtuse = false; 
           ObtuseIndex = 0;
        };

};

class neighbor
{
    public: 
        tri neighborTri; 
        double A_Voronoi; // Voronoi area in the neighborTri;
};


class vert  
{
    public: 


        // -------------- Fields ----------------- //
          
          
        /* static fields (not changing with time) */
        Vector3<double> position; 
        Vector3<double> normal;
        vector<neighbor> N1neighbors; // stores information of the 1-ring neighbors
        // for extraction
        bool isOnSurface; 
        int correspondVert; 
        // for curvature
        double A_mixed; // see Mark Meyer's discrete curvature paper
        double A_Voronoi_sum;
        double curvature;
        bool isnan;
        Vector3<double> K_xi_wo_Amixed;


        /* time-varying fields */
        vector<double> pressure; 
        vector<Vector3<double> > gradP; 
        vector<double> source1; // acoustic analogy source from p_n
        vector<double> source2; // acoustic analogy source from p*k

        // -------------- Constructors ---------------- //
        vert(double xin, double yin, double zin)
        {
            position.x = xin; 
            position.y = yin; 
            position.z = zin; 
        }

        vert() {}


            
        // -------------- Helpers Methods -------------- //
          
    
        void setEqual(vert v)
        { 
            position = v.position; 
            normal   = v.normal; 
            A_mixed = v.A_mixed; 
        }

        double EuclideanDistance(const vert & v) const
        {
            Vector3<double> PosDiff = position - v.position;
            return PosDiff.norm();
        }
        void printDetailInfo(bool splitingLine_flag) const
        {
            if (splitingLine_flag)
                cout << " ============ vertex information ============= " << endl;

            //if (isOnSurface) { cout << "This vertex is on the surface. " << endl; } 
            //else { cout << "This vertex is not on the surface. " << endl; } 

            cout << "Position = " << position << endl;
            cout << "Normal = " << normal << endl; 
            cout << "Curvature = " << curvature << endl; 

            if (splitingLine_flag)
                cout << "----- N1 neighbors information -----" << endl; 
            cout << "It has " << N1neighbors.size() << " N1 neighboring triangles" << endl; 
            for (size_v jj = 0; jj<N1neighbors.size(); jj++)
            {
                if (N1neighbors[jj].neighborTri.isObtuse)
                    cout << "Triangle # " << jj << " is obtuse" << endl;
                else
                    cout << "Triangle # " << jj << " is not obtuse" << endl;

                cout << "Triangle # " << jj << " area = "  << N1neighbors[jj].neighborTri.area << endl;
                cout << "Triangle # " << jj << " index = " << N1neighbors[jj].neighborTri.index << endl;
                cout << "Triangle # " << jj << " angle = " << N1neighbors[jj].neighborTri.angle << endl;
            }
        }
          
        // print pressure
        void printPressure() const
        {

            std::cout << "Pressure (t): " << std::endl;
            std::cout << "------------- " << std::endl;
            int p_size = this->pressure.size();
            for (int i=0; i<p_size; i++) 
            {
                std::cout << pressure[i] << std::endl; 
            }
        }

        void printPressure(std::ostream& os) const
        {

            os << "Pressure (t): " << std::endl;
            os << "------------- " << std::endl;
            int p_size = this->pressure.size();
            for (int i=0; i<p_size; i++) 
            {
                os << pressure[i] << std::endl; 
            }
        }

        // print pressure gradient
        void printGradP() const
        {

            std::cout << "Grad(Pressure) (t): " << std::endl;
            std::cout << "------------------- " << std::endl;
            int p_size = this->pressure.size();
            for (int i=0; i<p_size; i++) 
            {
                std::cout << gradP[i] << std::endl; 
            }
        }

        // print pressure gradient
        void printGradP(std::ostream& os) const
        {

            os << "Grad(Pressure) (t): " << std::endl;
            os << "------------------- " << std::endl;
            int p_size = this->pressure.size();
            for (int i=0; i<p_size; i++) 
            {
                os << gradP[i] << std::endl; 
            }
        }

        void computeA_Voronoi_Sum()
        {
            A_Voronoi_sum = 0.0;
            for (uint i=0; i<N1neighbors.size(); i++) 
            {
                if (isfinite(N1neighbors[i].A_Voronoi)) 
                    A_Voronoi_sum += N1neighbors[i].A_Voronoi; 
            }
        }

        void computeSource1();
        void computeSource2();

        // -------------- Operator Overload ------------------ //
        
        friend std::ostream &operator<<(std::ostream& os, const vert& V)
        {
            os << std::endl;

//            os << "=== Vertex final fields ===" << std::endl;
            os << "Vertex Position : " << V.position << std::endl; 
            os << "Vertex Normals  : " << V.normal   << std::endl; 
            os << "Vertex Curvature: " << V.curvature << std::endl;

//            os << "=== Vertex time-varying fields ===" << std::endl;
            V.printPressure(os); 
            V.printGradP(os);
            
            return os;

        }
};


#endif

