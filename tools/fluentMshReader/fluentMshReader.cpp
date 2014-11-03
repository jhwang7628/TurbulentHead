#include <fstream> 
#include <sstream> 
#include <string> 
#include <vector>
#include <algorithm>
#include <stdio.h>
#include "fluentMshReader.h" 

using namespace std; 

static const double PI=3.14159265359;

int main(int argc, char** argv)
{
   if (argc < 2)
   {
       cerr << "Error: Please specify msh name as input argument. Program exiting.." << endl;
       exit(1);
   }

   string mshName(argv[1]); 


   Mesh *mesh = new Mesh(mshName); 
   mesh->isOutputN1 = true;

   mesh->readMesh();
   ExtractSurface(mesh);

   string objname(string(mshName)+".obj");
   mesh->extractedSurface->printOBJ(objname);


   return 0;
}

// read mesh from *.msh file //
void Mesh::readMesh () 
{ 
    meshSurface = new surface();

    const char* filename = mshName.c_str();

    string filename_out(string(filename)+".obj");

    //FILE * fobj; 
    //fobj = fopen(filename_out.c_str(),"w");

    ifstream inhead(filename, ios::in); 
    string line; 

    while(getline(inhead,line)) 
    {
        if (line.substr(0,6) == "(10 (1")
        {
            break; 
        }
    }

    // reading in the vertex position of the entire sim domain
    cout << "reading vertex position" << endl;
    getline(inhead, line);  // get rid of the "( " line
    while(getline(inhead, line)) 
    {
        if (line.substr(0,1) == ")")
        {
            break; 
        } 

        vert newVert; 

        newVert.isOnSurface=false;
        newVert.isnan = false;

        istringstream iss(line); 
        double xBuff, yBuff, zBuff; 

        iss >> xBuff >> yBuff >> zBuff; 

        newVert.position.x = xBuff; newVert.position.y = yBuff; newVert.position.z = zBuff;

        meshSurface->vertlist.push_back(newVert); 

        //fobj << "v " << newVert.position << endl;
        //fprintf(fobj, "v %.9E %.9E %.9E\n", newVert.position.x,newVert.position.y,newVert.position.z);
        //fobj << "v " << meshSurface->vertlist[meshSurface->vertlist.size()-1].position << endl;

    }

    cout << meshSurface << endl;
    cout << "mesh filename: " << filename << endl;
    cout << "number of vertices read in: " << meshSurface->vertlist.size() << endl;

    //ofstream offah("havetofindthisbug.txt"); 

    // face for the head is at thread 13-13.
    while(getline(inhead,line)) 
    {
        if (line.substr(0,6) == "(13 (d")
        {
            break; 
        }
    }


    // reading in the index list for each triangle on the surface HumanHead-2 (ID:13)
    cout << "reading face composition" << endl;
    int tri_count=0, push_back_count=0, polygon_count =0; 
    getline(inhead, line);  // get rid of the "( " line
    while(getline(inhead, line)) 
    {
        if (line.substr(0,1) == ")")
        {
            break; 
        } 
        polygon_count ++; 
        istringstream iss(line); 


        int polygonEdge;
        iss >> hex >> polygonEdge; 
        int Ntri =  polygonEdge - 2; 
        vector<int> intBuff(polygonEdge);
        for (int i=0; i<polygonEdge; i++) 
        {
            iss >> hex >> intBuff[i]; 
        }

        //offah << polygonEdge << endl;

        for (int i=0; i<Ntri; i++)
        {
            tri newTri;
            tri_count++;
            
            // convert it to zero-based...
            int i0 = intBuff[0  ]-1, 
                i1 = intBuff[i+1]-1, 
                i2 = intBuff[i+2]-1; 

            newTri.index.x = i0;
            newTri.index.y = i1; 
            newTri.index.z = i2;

            Vector3<double> v0 = (meshSurface->vertlist)[i0].position;
            Vector3<double> v1 = (meshSurface->vertlist)[i1].position;
            Vector3<double> v2 = (meshSurface->vertlist)[i2].position;
            
            Vector3<double> v01 = v1 - v0; 
            Vector3<double> v02 = v2 - v0; 
            Vector3<double> v12 = v2 - v1;

            //cout << "v1 = " << v1 << endl;
            //cout << "v01 = " << v01 << endl;


            double cos_t0 =  v01.dot(v02)/v01.norm()/v02.norm();
            double cos_t1 = -v01.dot(v12)/v01.norm()/v12.norm();
            double cos_t2 =  v02.dot(v12)/v02.norm()/v12.norm();

            newTri.angle.x = acos(cos_t0); 
            newTri.angle.y = acos(cos_t1); 
            newTri.angle.z = acos(cos_t2);


            int count =0 ;
            if ( cos_t0<0 )
            {
               newTri.isObtuse = true;
               newTri.ObtuseIndex = i0;
               count ++; 
            }
            else if ( cos_t1<0 ) 
            {
               newTri.isObtuse = true;
               newTri.ObtuseIndex = i1;
               count ++; 
            }
            else if ( cos_t2<0 ) 
            {
               newTri.isObtuse = true;
               newTri.ObtuseIndex = i2;
               count ++; 
            }

            if (count >1)
            {
               cerr << "something is wrong. there are two obtuse angles for a single triangle. " << endl;
               exit(1); 
            }

            // NOTE: in-place cross product applied here!
            Vector3<double> triNormal = v01.cross(v02);

            newTri.area = 0.5*triNormal.norm();
            // add the area-weighted triangle normal to the vertex 
            meshSurface->vertlist[i0].normal.add(triNormal); 
            meshSurface->vertlist[i1].normal.add(triNormal); 
            meshSurface->vertlist[i2].normal.add(triNormal); 

            //cout << "isOnSurface of this vertex "  << meshSurface->vertlist[i0].isOnSurface << endl;
            meshSurface->vertlist[i0].isOnSurface = true; 
            meshSurface->vertlist[i1].isOnSurface = true; 
            meshSurface->vertlist[i2].isOnSurface = true; 

            //cout << "i0 = " << meshSurface->vertlist[i0].position << endl;
            //cout << "i1 = " << meshSurface->vertlist[i1].position << endl;
            //cout << "i2 = " << meshSurface->vertlist[i2].position << endl;

            // push back the triangle into the list
            //cout << "newTri.angle = " << newTri.angle << endl;

            neighbor newNeighbor;
            newNeighbor.neighborTri = newTri;
            meshSurface->trilist.push_back(newTri); 
            meshSurface->vertlist[i0].N1neighbors.push_back(newNeighbor); 
            meshSurface->vertlist[i1].N1neighbors.push_back(newNeighbor); 
            meshSurface->vertlist[i2].N1neighbors.push_back(newNeighbor); 


            //fobj << "f " << (newTri.index+1) << endl;
            //fprintf(fobj, "f %d %d %d\n", newTri.index.x+1, newTri.index.y+1, newTri.index.z+1);
            //fobj << "f " << meshSurface->trilist[meshSurface->trilist.size()-1].index << endl;
            push_back_count += 3;


            //newTri.printIndex();
        }

    }

    for (unsigned int i=0; i<meshSurface->vertlist.size(); i++) 
    {
        if (meshSurface->vertlist[i].isOnSurface)
        {
            meshSurface->vertlist[i].normal.normalize();
            //cout << "vert " << i << " normal: ----------------------------------- " << meshSurface->vertlist[i].normal << endl;a
        }
    }

    //offah.close();
    //offah.close();
    cout << "number of polygons read in: " << polygon_count << endl;
    cout << "number of triangles read in: " << meshSurface->trilist.size() << endl;
    int count_onsurface = 0;
    for (unsigned long i=0; i<meshSurface->vertlist.size(); i++) 
    {
       if (meshSurface->vertlist[i].isOnSurface) 
       {
          count_onsurface ++; 
       }
    }

    cout << "count_onsurface vertices = " << count_onsurface << endl;
        


}

// extract surface vertices from the flow field 
void ExtractSurface(Mesh *mesh) 
{

    cout << "Extract surface" << endl;

    mesh->extractedSurface = new surface();

    int count = 0;
    for (unsigned int i=0; i<mesh->meshSurface->vertlist.size(); i++)
    {
        if (mesh->meshSurface->vertlist[i].isOnSurface) 
        {
            mesh->meshSurface->vertlist[i].correspondVert = count;
            vert newVert; 
            newVert.setEqual(mesh->meshSurface->vertlist[i]);
            mesh->extractedSurface->vertlist.push_back(newVert);
            count ++; 
        }
    }

    for (unsigned int i=0; i<mesh->meshSurface->trilist.size(); i++)
    {
        int i1, i2, i3; 
        i1 = mesh->meshSurface->trilist[i].index.x;
        i2 = mesh->meshSurface->trilist[i].index.y;
        i3 = mesh->meshSurface->trilist[i].index.z;

        tri newTri; 

        newTri.setEqual(mesh->meshSurface->trilist[i]);

        newTri.index.x = mesh->meshSurface->vertlist[i1].correspondVert;
        newTri.index.y = mesh->meshSurface->vertlist[i2].correspondVert;
        newTri.index.z = mesh->meshSurface->vertlist[i3].correspondVert;

        mesh->extractedSurface->trilist.push_back(newTri);
    }
}


void surface::printOBJ(string objname)
{

    cout << "print OBJ" << endl;

    FILE *fobj;

    fobj = fopen(objname.c_str(),"w");

    for (unsigned int i=0; i<vertlist.size(); i++)
    {
        fprintf(fobj, "v %.9E %.9E %.9E\n", vertlist[i].position.x,vertlist[i].position.y,vertlist[i].position.z);
    }
    for (unsigned int i=0; i<trilist.size(); i++)
    {
        fprintf(fobj, "f %d %d %d\n", trilist[i].index.x+1, trilist[i].index.y+1, trilist[i].index.z+1);
    }
}

