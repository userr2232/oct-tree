#include "node.h"
#include "CImg.h"
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <utility>

using namespace std;
using namespace cimg_library;
//#define cimg_use_jpeg 1
// g++ -std=c++17  main.cc -I /opt/X11/include -L /opt/X11/lib -lX11 -ljpeg -lpng -lz -pthread
//g++ -std=c++17 main.cc -lX11 -lpthread

vector<CImg<float>>v_img;

class Octree {
    private:
        string oct_file;
        long EPS = 2;
        Node root;

        vector<string> get_file_names(const char* fnames) {
            string fname;
            vector<string> res;
            ifstream file(fnames);
            while(getline(file, fname)){
                res.push_back(fname);
            }
            return res;
        }

        void build(const char* fnames) {
            vector<string> v = get_file_names(fnames);
            vector<CImg<float>> images;
            for(auto x : v)
            {
              cout<<x<<endl;
                images.push_back(CImg<float>(x.c_str()));
              }
            allocate_node(0, 0, 0, images[0].width(), images[0].height(), images.size(), images);
            read_root();
            insert(root, images);
        }

        Node read(long dir)
        {
            ifstream save_file(oct_file);
            save_file.seekg(dir);
            Node node{};
            save_file.read((char*)(&node),sizeof(Node));
            save_file.close();
            return node;
        }

        void insert(Node cur, vector<CImg<float>>& cube) {
            if(!verificar(cur.x1, cur.y1, cur.z1, cur.x2, cur.y2, cur.z2, cube).first) {
                divide(cur,cube);
                for(long i = 0; i < 8; ++i) {
                    auto child = read(cur.children[i]);
                    insert(child,cube);
                }
            }
        }

        long allocate_node(long x1, long y1, long z1, long x2, long y2, long z2,vector<CImg<float>>& cube)
        {
            ofstream save_file(oct_file, ios::app | ios::binary);
            Node a(x1,y1,z1,x2,y2,z2,-1);
            a.c=verificar(x1,y1,z1,x2,y2,z2,cube).second;
            long pos=save_file.tellp();
            a.dir=pos;
            save_file.write((char*)(&a),sizeof(Node));
            save_file.close();
            return pos;
        }

        void write(Node cur)
        {
            ofstream save_file(oct_file,ios::app);
            save_file.seekp(cur.dir);
            save_file.write((char*)(&cur),sizeof(Node));
            save_file.close();
        }

        void divide(Node& cur,vector<CImg<float>>& cube) {
            int x1=cur.x1;
            int x2=cur.x2;
            int y1=cur.y1;
            int y2=cur.y2;
            int z1=cur.z1;
            int z2=cur.z2;

            long xm=(cur.x1+cur.x2)/2;
            long ym=(cur.y1+cur.y2)/2;
            long zm=(cur.z1+cur.z2)/2;

            cur.children[1] = allocate_node(x1,ym,z1,xm,y2,zm, cube);//AR2(MORADO)=X1,YM,Z1 -> XM,Y2,ZM
            cur.children[2] = allocate_node(xm,ym,z1,x2,y2,zm, cube);//AR3(VERDE)=XM,YM,Z1 -> X2,Y2,ZM
            cur.children[3] = allocate_node(xm,y1,z1,x2,ym,zm, cube);//ARR4(CELESTE)=XM,Y1,Z1 -> X2,YM,ZM

            cur.children[4] = allocate_node(x1,y1,zm,xm,ym,z2, cube);//AB1(AMARILLO)= X1, Y1,ZM -> XM,YM,Z2
            cur.children[5] = allocate_node(x1,ym,zm,xm,y2,z2, cube);//AB2(BLANCO)= X1,YM,ZM -> XM,Y2,Z2
            cur.children[6] = allocate_node(xm,y1,zm,x2,ym,z2, cube);//AB3(CELESTE)=XM,Y1,ZM -> X2,YM,Z2
            cur.children[7] = allocate_node(xm,ym,zm,x2,y2,z2, cube);//AB4(VERDE)=XM,YM,ZM -> X2,Y2,Z2
            cur.children[0] = allocate_node(x1,y1,z1,xm,ym,zm, cube);// AR1(AMARILLO)=X1,Y1,Z1 -> XM,YN,ZM
            write(cur);
        }

        pair<bool,long> verificar(long x1, long y1, long z1, long x2, long y2, long z2, vector<CImg<float>>& cube) {
            long color=0;
            if(x2 - x1 <= 1 && y2 - y1 <= 1 && z2 - z1 <= 1)
                return {true,cube[z1](x1,y1)};
            float c = cube[z1](x1, x2);
            for(long i = x1; i < x2; ++i) {
                for(long j = y1; j < y2; ++j) {
                    for(long k = z1; k < z2; ++k) {
                        //color+=cube[k](i,j);
                        if(abs(c - cube[k](i,j)) > EPS) return {false,-1};
                    }
                }
            }
            return {true,c/*/((x2-x1)*(y2-y1)*(z2-z1))*/};
        }

        void read_root() {
            root = read(0);
        }


        void rebuild(vector<Node> &nodos)
        {

        }

        void find_nodes(Node& cur,int A,int B,int C,int D,CImg<float>& img)
        {
            int x1=cur.x1;
            int x2=cur.x2;
            int y1=cur.y1;
            int y2=cur.y2;
            int z1=cur.z1;
            int z2=cur.z2;

            if(cur.c==-1)
            {
              
            }

            vector<pair<Point,Point>> points;
            points.push_back(make_pair{{x1,y1,z1},{x2,y2,z2}});
            points.push_back(make_pair{{x2,y1,z1},{x1,y2,z2}});
            points.push_back(make_pair{{x1,y1,z2},{x2,y2,z1}});
            points.push_back(make_pair{{x2,y1,z2},{x1,y2,z1}});

            bool r=0;
            for(auto i:points )
            {
              auto [p1,p2]=i;
              if((A*p1.x+B*p1.y+C*p1.y+D)*(A*p2.x+B*p2.y+C*p2.y+D)<=0)
              {
                r=1
                break;
              }
            }

            if(!r) return;

            for(int i=0;i<8;i++)
            {
              auto a=read(cur.children[i]);
              find_nodes(a, A, B, C, D,img);
            }

        }



    public:
        Octree(const char* nodesfile) {
            read_root();
        }
        Octree(string nodesfile, const char* fnames) {
            oct_file = nodesfile;
            build(fnames);
        }
        void get(int A,int B,int C,int D)
        {
            vector<Node> nodos;
            // root.find_nodes(A,B,C,D,nodos);
            rebuild(nodos);
        }
};

void read()
{
  vector<CImg<float>> v;
  CImg<float> b("p1.BMP");
  CImg<float> a(b.width(),b.height());
  v.push_back(a);
  v.push_back(a);
  v.push_back(a);


  ifstream f("octree");
  Node n;
  while(!f.eof())
  {
    f.read((char*)(&n),sizeof(Node));
    if(n.c==-1) continue;
    for(long i = n.x1; i < n.x2; ++i) {
        for(long j = n.y1; j < n.y2; ++j) {
            for(long k = n.z1; k < n.z2; ++k) {
                v[k](i,j)=n.c;
            }
        }
    }
  }


  v[0].save("1.BMP");
  v[1].save("2.BMP");
  v[2].save("3.BMP");
}


void read(double A, double B, double C, double D)
{
  CImg<float> b("p1.BMP");
  CImg<float> a(b.width(),b.height());
  for(int i=0;i<a.width();i++)
  {
    for(int j=0;j<a.height();j++)
    {
      for(int k=0;k<v_img.size();k++)
      {
        if(abs(A*i+B*j+C*k+D)<=0.5)
        {
          a(i,j)=v_img[k](i,j);
        }
      }
    }
  }
  a.save("resultado.BMP");

}


int main(){
  v_img.push_back(CImg<float>("p1.BMP"));
  v_img.push_back(CImg<float>("p2.BMP"));
  v_img.push_back(CImg<float>("p3.BMP"));


    // Octree oc("octree","files.txt");
    //Octree oc2("octree");
    //oc2.get(0,0,1,0);
    // ifstream f("texto.txt");
    // string s;
    // while(!f.eof())
    // {
    //   getline(f,s);
    //   if(s.size()==0)break;
    //   v_img.push_back(CImg<float>(string("1/"+s).c_str()));
    // };
    //
    //


    read(4.0/512,0,1,-3);
    //
    // return 0;
}
