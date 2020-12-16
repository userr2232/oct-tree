#include "CImg.h"
#include <vector>
#include <utility>
using namespace std;
using namespace cimg_library;
//#define cimg_use_jpeg 1

struct Point
{
  int x,y,z;
}

struct Node {
  int x1, y1, z1, x2, y2, z2;
  long dir;
  float c;

  long children[8];

  Node() = default;
  Node(int x1, int y1, int z1, int x2, int y2, int z2, float c):
    x1{x1}, y1{y1}, z1{z1}, x2{x2}, y2{y2}, z2{z2}, c{c} {};
  void find_nodes(int A,int B,int C,int D,CImg<float>& img)
  {
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




  //   double v1=A*x1+B*y1+C*z1+D;
  //   double v2=A*x2+B*y2+C*z2+D;
  //
  //   if(v1*v2>0) return;
  //
  //   if(c!=-1)
  //   {
  //     for(long i = x1; i < x2; ++i) {
  //         for(long j = y1; j < y2; ++j) {
  //
  //             }
  //         }
  //     }
  //   }
  //
  //
  }
};
