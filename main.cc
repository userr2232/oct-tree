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
                images.push_back(CImg<float>(x.c_str()));
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
            ofstream save_file(oct_file);
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

            cur.children[0] = allocate_node(x1,y1,z1,xm,ym,zm, cube);// AR1(AMARILLO)=X1,Y1,Z1 -> XM,YN,ZM
            cur.children[1] = allocate_node(x1,ym,z1,xm,y2,zm, cube);//AR2(MORADO)=X1,YM,Z1 -> XM,Y2,ZM
            cur.children[2] = allocate_node(xm,ym,z1,x2,y2,zm, cube);//AR3(VERDE)=XM,YM,Z1 -> X2,Y2,ZM
            cur.children[3] = allocate_node(xm,y1,z1,x2,ym,zm, cube);//ARR4(CELESTE)=XM,Y1,Z1 -> X2,YM,ZM

            cur.children[4] = allocate_node(x1,y1,zm,xm,ym,z2, cube);//AB1(AMARILLO)= X1, Y1,ZM -> XM,YM,Z2
            cur.children[5] = allocate_node(x1,ym,zm,xm,y2,z2, cube);//AB2(BLANCO)= X1,YM,ZM -> XM,Y2,Z2
            cur.children[6] = allocate_node(xm,y1,zm,x2,ym,z2, cube);//AB3(CELESTE)=XM,Y1,ZM -> X2,YM,Z2
            cur.children[7] = allocate_node(xm,ym,zm,x2,y2,z2, cube);//AB4(VERDE)=XM,YM,ZM -> X2,Y2,Z2
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

    public:
        Octree(const char* nodesfile) {
            read_root();
        }
        Octree(string nodesfile, const char* fnames) {
            oct_file = nodesfile;
            build(fnames);
        }
};

int main(){
    Octree oc("octree","files.txt");
    Octree oc2("octree");
    return 0;
}