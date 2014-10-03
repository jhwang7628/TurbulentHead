#include <fstream> 
#include <sstream> 
#include <string> 
#include <vector>
#include <algorithm>
#include <qapplication.h>
#include "curvature.h" 


using namespace qglviewer;
using namespace std; 

static const double PI=3.14159265359;

int main(int argc, char** argv)
{
   QApplication application(argc,argv);


   Viewer viewer;

   viewer.setWindowTitle("Pressure Contour on the head");

   viewer.show();


  return application.exec();
  //return 0;
}

void Mesh::readOBJ (string filename)
{
    meshSurface = new surface();

    filename = "../../testCase/spherePatch.obj";

    cout << "reading mesh " << filename << endl;

    ifstream in(filename.c_str(), ios::in);
    if (!in) { cerr << "Cannot open " << filename << endl; exit(1); }


    string line;
    while (getline(in, line)) 
    { 
       if (line.substr(0,2) == "v ") 
       { 
          vert newVert;
          newVert.isOnSurface=false;
          istringstream iss(line.substr(2)); 
          double xBuff, yBuff, zBuff; 

          iss >> xBuff >> yBuff >> zBuff; 

          newVert.position.x = xBuff; 
          newVert.position.y = yBuff; 
          newVert.position.z = zBuff;

          meshSurface->vertlist.push_back(newVert); 

          //fobj << "v " << newVert.position << endl;

       } 
       else if (line.substr(0,2) == "f ") 
       { 
            tri newTri;
            istringstream iss(line.substr(2)); 

            int i0, i1, i2; 

            iss >> i0 >> i1 >> i2; 

            // turn it into zero-based
            i0--; i1--; i2--;
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

            meshSurface->vertlist[i0].isOnSurface = true; 
            meshSurface->vertlist[i1].isOnSurface = true; 
            meshSurface->vertlist[i2].isOnSurface = true; 

            //cout << "isOnSurface of vertex "  << i0 << " is on surface? " << meshSurface->vertlist[i0].isOnSurface << endl;
            //cout << "isOnSurface of vertex "  << i1 << " is on surface? " << meshSurface->vertlist[i1].isOnSurface << endl;
            //cout << "isOnSurface of vertex "  << i2 << " is on surface? " << meshSurface->vertlist[i2].isOnSurface << endl;

            //cout << "i0 = " << meshSurface->vertlist[i0].position << endl;
            //cout << "i1 = " << meshSurface->vertlist[i1].position << endl;
            //cout << "i2 = " << meshSurface->vertlist[i2].position << endl;

            // push back the triangle into the list
            //cout << "newTri.angle = " << newTri.angle << endl;

            meshSurface->trilist.push_back(newTri); 
            meshSurface->vertlist[i0].N1neighbors.push_back(newTri); 
            meshSurface->vertlist[i1].N1neighbors.push_back(newTri); 
            meshSurface->vertlist[i2].N1neighbors.push_back(newTri); 

            newTri.printIndex();

       }
       else if (line[0] == '#') { /* ignoring this line */ }
       else { /* ignoring this line */ }

       cout << "vertlist size = " << meshSurface->vertlist.size() << endl;
    }


    for (int i=0; i<meshSurface->vertlist.size(); i++) 
    {
        meshSurface->vertlist[i].normal.normalize();
        cout << "vert " << i << " normal: ----------------------------------- " << meshSurface->vertlist[i].normal << endl;
    }


    



}


// data parser //
void Mesh::readMesh () 
{ 
    meshSurface = new surface();

    const char* filename = "../jhwang.msh";

    ofstream fobj("HumanHead-2.obj"); 


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
    getline(inhead, line);  // get rid of the "( " line
    while(getline(inhead, line)) 
    {
        if (line.substr(0,1) == ")")
        {
            break; 
        } 

        vert newVert; 

        newVert.isOnSurface=false;

        istringstream iss(line); 
        double xBuff, yBuff, zBuff; 

        iss >> xBuff >> yBuff >> zBuff; 

        newVert.position.x = xBuff; newVert.position.y = yBuff; newVert.position.z = zBuff;

        meshSurface->vertlist.push_back(newVert); 

        fobj << "v " << newVert.position << endl;
        //fobj << "v " << meshSurface->vertlist[meshSurface->vertlist.size()-1].position << endl;

    }

    cout << meshSurface << endl;
    cout << "mesh filename: " << filename << endl;
    cout << "number of vertices read in: " << meshSurface->vertlist.size() << endl;

    //ofstream offah("havetofindthisbug.txt"); 

    while(getline(inhead,line)) 
    {
        if (line.substr(0,6) == "(13 (d")
        {
            break; 
        }
    }


    // reading in the index list for each triangle on the surface HumanHead-2 (ID:13)
    int stop_count =0, tri_count=0, push_back_count=0, polygon_count =0; 
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
            newTri.area = 0.5*triNormal.norm();

            // push back the triangle into the list
            //cout << "newTri.angle = " << newTri.angle << endl;

            meshSurface->trilist.push_back(newTri); 
            meshSurface->vertlist[i0].N1neighbors.push_back(newTri); 
            meshSurface->vertlist[i1].N1neighbors.push_back(newTri); 
            meshSurface->vertlist[i2].N1neighbors.push_back(newTri); 


            fobj << "f " << (newTri.index+1) << endl;
            //fobj << "f " << meshSurface->trilist[meshSurface->trilist.size()-1].index << endl;
            push_back_count += 3;


            //newTri.printIndex();
        }

    }

    for (int i=0; i<meshSurface->vertlist.size(); i++) 
    {
        meshSurface->vertlist[i].normal.normalize();
        cout << "vert " << i << " normal: ----------------------------------- " << meshSurface->vertlist[i].normal << endl;
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

// return alpha for this pair of indices ind_xi and ind_xj
void surface::findShareTriangles(const int ind_xi, const int ind_xj, const vector<tri>& triRange, vector<double>& alpha)
{
   //cout << "-------------------- for (" <<  ind_xi << ", " << ind_xj << ") ----------------------" << endl;

   int N = triRange.size(); 

   for (int i=0; i<N; i++)
   {
       //cout << "triangle " << &triRange[i];
       // use concept from chmod: 1 2 4 represents x, y, z
       int count_match_ind = 0, index_match = 0;
       if (triRange[i].index.x == ind_xi) {count_match_ind++;index_match += 1;}
       if (triRange[i].index.x == ind_xj) {count_match_ind++;index_match += 1;}
       if (triRange[i].index.y == ind_xi) {count_match_ind++;index_match += 2;}
       if (triRange[i].index.y == ind_xj) {count_match_ind++;index_match += 2;}
       if (triRange[i].index.z == ind_xi) {count_match_ind++;index_match += 4;}
       if (triRange[i].index.z == ind_xj) {count_match_ind++;index_match += 4;}

       // found the triangle!
       if (count_match_ind == 2)
       {
          //cout << " is the matched triangle. " << endl;
          //cout << "found the triangle" << endl; 
          double Tri_alpha;

          //cout << "the angle for this triangle is " << triRange[i].angle << endl;

          if (index_match == 3) // the other index is z
          {
              //cout << "alpha at z " << endl; 
              Tri_alpha = triRange[i].angle.z; 
          }
          else if (index_match == 6) // the other index is x
          {
              //cout << "alpha at x " << endl; 
              Tri_alpha = triRange[i].angle.x; 
          }
          else if (index_match == 5) // the other index is y
          {
              //cout << "alpha at y " << endl; 
              Tri_alpha = triRange[i].angle.y; 
          }
          else 
          {
              cout << "ind_xi = " << ind_xi << "; ind_xj = " << ind_xj << endl;
              cout << index_match << endl;
              cerr << "something is wrong. cannot find alpha for this triangle " << endl;
              exit(1);
          }

          alpha.push_back(Tri_alpha);

          //cout << " The triangle that contains this edge (" << ind_xi << " and " << ind_xj << " is " << &triRange[i] << endl;
       }
       //else
       //{
       //    cout << " is NOT the matched triangle. " << endl;
       //}

   }

   //cout << "size of alpha is " << alpha.size() << endl;

}



void surface::computeK()
{

   int Ntri = (this->trilist).size();
   int Nvert = (this->vertlist).size();

   // mark the N1 neighbors for each vertex
   //for (int i=0; i<Ntri; i++)
   //{

   //   cout << "filling N1 neighbors for triangle " << i+1 << "/" << Ntri << endl;
   //   int i0 = trilist[i].index.x; 
   //   int i1 = trilist[i].index.y; 
   //   int i2 = trilist[i].index.z; 
   //   vertlist[i0].N1neighbors.push_back(trilist[i]);
   //   vertlist[i1].N1neighbors.push_back(trilist[i]);
   //   vertlist[i2].N1neighbors.push_back(trilist[i]);
   //}

   // compute A_voronoi for each vertex
   ofstream outfile("A_Voronoi.txt");
   ofstream outfile2("K_xi.txt");
   for (int i=0; i<Nvert; i++)
   {
      if (vertlist[i].isOnSurface) 
      {
          //cout << "not on surface " << endl;
      

          // how many triangles does this vertex possess
          int N_N1neighbors = vertlist[i].N1neighbors.size();

          vertlist[i].A_Voronoi = 0;

          // list of all neighboring vertices
          vector<int> neighborVert;
          vector<int>::iterator it; 

          cout << "vertex " << i << " has " << N_N1neighbors << " neighbor triangles. " << endl;
          for (int j=0; j<N_N1neighbors; j++)
          {
             //cout << "angles of the N1-neighbors are " << vertlist[i].N1neighbors[j].angle << endl;
             neighborVert.push_back(vertlist[i].N1neighbors[j].index.x);
             neighborVert.push_back(vertlist[i].N1neighbors[j].index.y);
             neighborVert.push_back(vertlist[i].N1neighbors[j].index.z);
          }
          if (N_N1neighbors != 0)
          {
              cout << "======= before sort ======" << endl;
             for ( int k=0; k<neighborVert.size(); k++) 
             {
                   cout << "vertex " << i << " has neighbor vertex " << neighborVert[k] << endl;
             }
             sort(neighborVert.begin(), neighborVert.end()); 
              cout << "======= after sort ======" << endl;
             for ( int k=0; k<neighborVert.size(); k++) 
             {
                   cout << "vertex " << i << " has neighbor vertex " << neighborVert[k] << endl;
             }
             it = unique(neighborVert.begin(), neighborVert.end()); 
             neighborVert.resize( distance(neighborVert.begin(),it) );
              cout << "======= after unique ======" << endl;
             for ( int k=0; k<neighborVert.size(); k++) 
             {
                   cout << "vertex " << i << " has neighbor vertex " << neighborVert[k] << endl;
             }
          }

          cout << "======= outside the loop ======" << endl;

          for ( int k=0; k<neighborVert.size(); k++) 
          {
                cout << "vertex " << i << " has neighbor vertex " << neighborVert[k] << endl;
          }
          cout << "vertex " << i << " has " << neighborVert.size() << " neighbor vertices. " << endl;

          // scan through each neighboring vertex to compute voronoi area
          double sum_N1AV = 0;
          Vector3<double> sum_N1NK;
          for (unsigned long j=0; j<neighborVert.size(); j++) 
          {
             if (neighborVert[j]!=i)
             {
                int ind_xi = i, ind_xj = neighborVert[j]; 

                // find alpha and beta for each neighbor
                vector<double> Alpha; 

                findShareTriangles(ind_xi, ind_xj, vertlist[i].N1neighbors, Alpha);
                //cout << "size of Alpha is " << Alpha.size() << endl;
                //cout << "Retrieved Alpha is " << Alpha[0] << " and " << Alpha[1] << endl;
                //if (Alpha[0] < 0 || Alpha[0] > PI || Alpha[1] < 0 || Alpha[1] > PI) 
                //{
                //    cerr << "something is wrong. alpha is greater than 180 or negative. " << endl; 
                //    exit(1); 
                //}

                double xij2 = (vertlist[ind_xi].position - vertlist[ind_xj].position).normSq();
                // the vector distance 
                Vector3<double> xij_current = (vertlist[ind_xi].position - vertlist[ind_xj].position);
                //if (xij2<0) 
                //{
                //    cerr << "something is wrong. xij2 is negative. " << endl; 
                //    exit(1); 
                //}
                //cout << "alpha = " << Alpha[0] << ", beta = " << Alpha[1] << endl;
                //double tmp = (1.0/tan(Alpha[0]) + 1.0/tan(Alpha[1]))*xij2;
                double cot_alpha_sum = 1.0/tan(Alpha[0]) + 1.0/tan(Alpha[1]);
                cout << "For (i,j) = " << ind_xi << ", " << ind_xj << endl;
                cout << "Alpha 0: " << Alpha[0]*180/PI << "; Alpha 1: " << Alpha[1]*180/PI << endl;
                if (Alpha[0] < 1e-8 || Alpha[1] < 1e-8)
                {
                   cout << "vert " << i << " has zero alphas " << endl;
                   cout << "alpha 0 " << Alpha[0] << endl;
                   cout << "alpha 1 " << Alpha[1] << endl;
                   //exit(1);
                }
                //cout << "tmp = " << tmp <<  endl;
                //if (tmp>0) 
                //{
                cout << "sum_N1NK = " << sum_N1NK << endl;
                cout << "xij_current = " << xij_current << endl;
                sum_N1AV += cot_alpha_sum * xij2;
                sum_N1NK.add(xij_current*cot_alpha_sum);
                    //cerr << "something is wrong. tmp is negative. " << endl; 
                    //exit(1); 
                //}
             }
          }
          vertlist[i].A_Voronoi = 1.0/8.0*sum_N1AV;
          vertlist[i].K_xi_wo_Amixed = sum_N1NK;
          //if (vertlist[i].A_Voronoi<0) 
          //{
          //    cerr << "something is wrong. cell Voronoi area is negative. " << endl; 
          //    //exit(1); 
          //}
          //cout << "the Voronoi area for vertex " << i << " is " << vertlist[i].A_Voronoi << endl;
          outfile << vertlist[i].A_Voronoi << endl;
          outfile2<< vertlist[i].K_xi_wo_Amixed << endl;



      }

   }



   outfile.close();
   outfile2.close();


   ofstream fK("computedK.txt"); 
   ofstream fA("A_mixed.txt");

   // Compute A_mixed area and curvature
   for (int i=0; i<Nvert; i++) 
   {
       cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
       cout <<" come onnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn " << i << endl;
      if (vertlist[i].isOnSurface)
      {
       cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
       cout <<" come onnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn " << i << endl;
       cout << "How many neighbor points? " << vertlist[i].N1neighbors.size() << endl;

         //cout << "==========vertex " << i << endl;

          vertlist[i].A_mixed = 0;

          int N_N1neighbors = vertlist[i].N1neighbors.size();
          
          for (int j=0; j<N_N1neighbors; j++) 
          {
             //cout << "(i,j) = " << i << ", " << j << endl;
             if (!vertlist[i].N1neighbors[j].isObtuse) 
             {
                 cout << "isNOTObtuse!" << endl;
                cout << "Voronoi region added" << endl;
                vertlist[i].A_mixed += vertlist[i].A_Voronoi; 
             }
             else 
             {
                 cout << "isObtuse!" << endl;
                 cout << vertlist[i].N1neighbors[j].ObtuseIndex << endl;
                if (vertlist[i].N1neighbors[j].ObtuseIndex == i)
                {
                   cout << "Triangle area / 2.0 added" << endl;
                   //cout << "trilist[j].area = " << trilist[j].area << endl;
                   vertlist[i].A_mixed += trilist[j].area / 2.0; 
                }
                else 
                {
                   cout << "Triangle area / 4.0 added" << endl;
                   //cout << "trilist[j].area = " << trilist[j].area << endl;
                   vertlist[i].A_mixed += trilist[j].area / 4.0; 
                }
             }
          }

          Vector3<double> K_xi = vertlist[i].K_xi_wo_Amixed.mul(1.0/vertlist[i].A_mixed);
          //K_xi.mul(1.0/2.0/vertlist[i].A_mixed);

          cout << K_xi << endl;
          cout << vertlist[i].normal << endl;
          vertlist[i].curvature = 1.0/4.0*K_xi.dot(vertlist[i].normal); 
          fK << vertlist[i].curvature << endl;
          fA << vertlist[i].A_mixed << endl;
      }

      
   }

   fA.close();
   fK.close();


}

void Viewer::init()
{
   restoreStateFromFile(); 
   glDisable(GL_LIGHTING);

   glPointSize(5.0); 
   setGridIsDrawn(false); 

   // const char* filename;
   string filename;
   cout << "Input the OBJ file to read: " << endl;
   getline(cin, filename);

   mesh->readOBJ(filename);
   //mesh->readMesh();
   mesh->meshSurface->computeK();
   //cout << " YEAHYEAHYEAH " << endl;
}

// // Debug draw 
// void Viewer::draw()
// {
//    int i = 42965;
//    //int i = 116841;
//    glBegin(GL_POINTS);
//    qglviewer::Vec vertPos = Vec(
//          mesh->meshSurface->vertlist[i].position.x,
//          mesh->meshSurface->vertlist[i].position.y,
//          mesh->meshSurface->vertlist[i].position.z);
//    glColor3f(1.0f,0.0f,0.0f); 
//    glVertex3fv(vertPos); 
//    glEnd(); 
// 
//    glBegin(GL_TRIANGLES);
// 	glEdgeFlag(TRUE);
//    for (unsigned long j=0; j<mesh->meshSurface->vertlist[i].N1neighbors.size(); j++)
//    {
//           glColor3f(1.0f, 1.0f, 1.0f); 
//           qglviewer::Vec vertPos = Vec(
//           mesh->meshSurface->vertlist[mesh->meshSurface->trilist[j].index.x].position.x,
//           mesh->meshSurface->vertlist[mesh->meshSurface->trilist[j].index.x].position.y,
//           mesh->meshSurface->vertlist[mesh->meshSurface->trilist[j].index.x].position.z);
//           glVertex3fv(vertPos); 
// 
//           qglviewer::Vec vertPos2 = Vec(
//           mesh->meshSurface->vertlist[mesh->meshSurface->trilist[j].index.y].position.x,
//           mesh->meshSurface->vertlist[mesh->meshSurface->trilist[j].index.y].position.y,
//           mesh->meshSurface->vertlist[mesh->meshSurface->trilist[j].index.y].position.z);
//           glVertex3fv(vertPos2); 
// 
//           qglviewer::Vec vertPos3 = Vec(
//           mesh->meshSurface->vertlist[mesh->meshSurface->trilist[j].index.z].position.x,
//           mesh->meshSurface->vertlist[mesh->meshSurface->trilist[j].index.z].position.y,
//           mesh->meshSurface->vertlist[mesh->meshSurface->trilist[j].index.z].position.z);
//           glVertex3fv(vertPos3); 
//    }
//    glEnd();
// 
//    glBegin(GL_POINTS);
// 	glEdgeFlag(TRUE);
//    for (unsigned long j=0; j<mesh->meshSurface->vertlist[i].N1neighbors.size(); j++)
//    {
//           glColor3f(0.0f, 1.0f, 0.0f); 
//           qglviewer::Vec vertPos = Vec(
//           mesh->meshSurface->vertlist[mesh->meshSurface->trilist[j].index.x].position.x,
//           mesh->meshSurface->vertlist[mesh->meshSurface->trilist[j].index.x].position.y,
//           mesh->meshSurface->vertlist[mesh->meshSurface->trilist[j].index.x].position.z);
//           glVertex3fv(vertPos); 
// 
//           qglviewer::Vec vertPos2 = Vec(
//           mesh->meshSurface->vertlist[mesh->meshSurface->trilist[j].index.y].position.x,
//           mesh->meshSurface->vertlist[mesh->meshSurface->trilist[j].index.y].position.y,
//           mesh->meshSurface->vertlist[mesh->meshSurface->trilist[j].index.y].position.z);
//           glVertex3fv(vertPos2); 
// 
//           qglviewer::Vec vertPos3 = Vec(
//           mesh->meshSurface->vertlist[mesh->meshSurface->trilist[j].index.z].position.x,
//           mesh->meshSurface->vertlist[mesh->meshSurface->trilist[j].index.z].position.y,
//           mesh->meshSurface->vertlist[mesh->meshSurface->trilist[j].index.z].position.z);
//           glVertex3fv(vertPos3); 
//    }
//    glEnd();
// }
 void Viewer::draw()
 {
 
    //double max_curv = 0;
    //for (unsigned long i=0; i<mesh->meshSurface->vertlist.size(); i++)
    //{
    //   if (mesh->meshSurface->vertlist[i].isOnSurface)
    //   {
    //        if (abs(mesh->meshSurface->vertlist[i].curvature) > max_curv)
    //        {
    //           max_curv = abs(mesh->meshSurface->vertlist[i].curvature);
    //           cout << "max_curv is updated in vert" << i << " to be " <<  max_curv << endl;
    //        }
    //   }
    //}

    //double max_curv = mesh->meshSurface->vertlist[2151].curvature; 
    double max_curv = 1000; 

    //cout << max_curv << endl;


 
    glBegin(GL_POINTS);
 
    int count_onsurface = 0;
    for (unsigned long i=0; i<(mesh->meshSurface)->vertlist.size(); i++) 
    {

       if ((mesh->meshSurface->vertlist)[i].isOnSurface)
       {
           count_onsurface ++;
          //cout << "i=" << i << endl;
          double ratio = abs(mesh->meshSurface->vertlist[i].curvature/max_curv);
 
          //cout << "max_curv = " << max_curv << endl;
          //cout << "ratio of " << i << " = " << ratio << endl;
 
          int N = (mesh->meshSurface)->vertlist.size();
          glColor3f(ratio, 0.0, 0.0); 
          //glColor3f((double) i/N, (double) i/N, (double) i/N); 
          //glColor3f(0.0f, 0.0f, 0.0f);
          qglviewer::Vec vertPos = Vec(
                mesh->meshSurface->vertlist[i].position.x,
                mesh->meshSurface->vertlist[i].position.y,
                mesh->meshSurface->vertlist[i].position.z);
          glVertex3fv(vertPos); 
       }
    }

    //cout << "ocunt_onsurface = " << count_onsurface << endl;
    //cout << "vert 0 is on surface?" << mesh->meshSurface->vertlist[0].isOnSurface << endl;
    //cout << "vert 1 is on surface?" << mesh->meshSurface->vertlist[1].isOnSurface << endl;
 
    glEnd(); 

    cout << "vert 12 position: " << mesh->meshSurface->vertlist[12].position << endl;
    cout << "vert 12 N1neighbors size: " << mesh->meshSurface->vertlist[12].N1neighbors.size() << endl;

    for (int ii=0; ii<mesh->meshSurface->vertlist.size(); ii++) 
    {
        cout << "vert " << ii << "'s normals are: " << 
            mesh->meshSurface->vertlist[ii].normal << endl;
        //cout << "vert " << ii << "'s Voronoi are is: " << 
        //    mesh->meshSurface->vertlist[ii].A_Voronoi << endl;
    }

    for (int ii=0; ii<mesh->meshSurface->vertlist[12].N1neighbors.size(); ii++)
    {
        cout << "vert 12's " << ii << "-th neighbor has angle: " << 
            mesh->meshSurface->vertlist[12].N1neighbors[ii].angle/3.1415926*180.0 << endl;
        cout << "vert 12's " << ii << "-th neighbor is obtuse?: " << 
            mesh->meshSurface->vertlist[12].N1neighbors[ii].isObtuse << endl;
        //cout << "vert 12's " << ii << "-th neighbor has the index: " << endl;
        //mesh->meshSurface->vertlist[12].N1neighbors[ii].printIndex(); 
    }

 
 
//    glBegin(GL_TRIANGLES);
//    for (unsigned long i=0; i<(mesh->meshSurface)->trilist.size(); i++) 
//    {
//       //if ((mesh->meshSurface->vertlist)[i].isOnSurface)
//      //{
//          glColor3f(1.0f, 1.0f, 1.0f); 
//          qglviewer::Vec vertPos = Vec(
//                mesh->meshSurface->vertlist[mesh->meshSurface->trilist[i].index.x].position.x,
//                mesh->meshSurface->vertlist[mesh->meshSurface->trilist[i].index.x].position.y,
//                mesh->meshSurface->vertlist[mesh->meshSurface->trilist[i].index.x].position.z);
//          glVertex3fv(vertPos); 
//          qglviewer::Vec vertPos2 = Vec(
//                mesh->meshSurface->vertlist[mesh->meshSurface->trilist[i].index.y].position.x,
//                mesh->meshSurface->vertlist[mesh->meshSurface->trilist[i].index.y].position.y,
//                mesh->meshSurface->vertlist[mesh->meshSurface->trilist[i].index.y].position.z);
//          glVertex3fv(vertPos2); 
//          qglviewer::Vec vertPos3 = Vec(
//                mesh->meshSurface->vertlist[mesh->meshSurface->trilist[i].index.z].position.x,
//                mesh->meshSurface->vertlist[mesh->meshSurface->trilist[i].index.z].position.y,
//                mesh->meshSurface->vertlist[mesh->meshSurface->trilist[i].index.z].position.z);
//          glVertex3fv(vertPos3); 
//       //}
//    }
// 
//    glEnd(); 
 
    cout << "see drawing" << endl;
}




   


