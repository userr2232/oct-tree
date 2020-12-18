#include "CImg.h"
#include <vector>
#include <utility>
using namespace std;
using namespace cimg_library;
//#define cimg_use_jpeg 1

struct Point
{
  double x,y,z;
};

struct Node {
  short x1, y1, z1, x2, y2, z2;
  long dir;
  short c;

  long children[8];

  Node() = default;
  Node(short x1, short y1, short z1, short x2, short y2, short z2, short c):
    x1{x1}, y1{y1}, z1{z1}, x2{x2}, y2{y2}, z2{z2}, c{c} {};
};
