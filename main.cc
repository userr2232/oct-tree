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


Octree oc2("octree.dat");
oc2.get(0.5,1,0,-200);

}
