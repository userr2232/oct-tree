#include "node.h"
#include "CImg.h"
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <utility>
#include "octree.cpp"

using namespace std;
using namespace cimg_library;
//#define cimg_use_jpeg 1
// g++ -std=c++17  main.cc -I /opt/X11/include -L /opt/X11/lib -lX11 -ljpeg -lpng -lz -pthread
//g++ -std=c++17 main.cc -lX11 -lpthread

vector<CImg<float>>v_img;

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
        // cout<<A*i+B*j+C*k<<endl;
        if(abs(A*i+B*j+C*k+D<=0.5))
       // if(abs(A*i+B*j+C*k+D==0))

        {
          a(i,j)=v_img[k](i,j);
        }
      }
    }
  }
  a.save("resultado.BMP");

}


int main(){
//  v_img.push_back(CImg<float>("p1.BMP"));
//  v_img.push_back(CImg<float>("p2.BMP"));
//  v_img.push_back(CImg<float>("p3.BMP"));


    // Octree oc("octree.dat","files.txt");





    Octree oc2("octree.dat");
    //oc2.get(4.0/512,0,1,-3);
    oc2.get(0,0,1,0);
    
    // ifstream f("texto.txt");
    // while(!f.eof())
    // {
    //   getline(f,s);
    //   if(s.size()==0)break;
    //   v_img.push_back(CImg<float>(string("1/"+s).c_str()));
    // };
    //
    //

    //read(4.0/512,0,1,-3);

    //read(0,0,1,-1);

    //
    //read();
    // return 0;
}
