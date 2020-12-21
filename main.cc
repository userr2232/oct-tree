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

int main(){


// Octree oc2("octree.dat"."texto.txt"); //"texto.txt" es el archivo donde estan las direcciones de todas las imagenes que seran parte del cubo

Octree oc2("octree.dat"); //Una vez ya esta creado el octree
// oc2.get(0.5,1,0,-200,"img.BMP");


//Paralelo al plano xy
// for(int i=0;i<5;i++)
// oc2.get(0,0,1,-i*4,"paralelo_xy-"+to_string(i)+".BMP");


//Paralelo al plano xz
// oc2.get(0,1,0,-54,"paralelo_xz-1.BMP");
// oc2.get(0,1,0,-128,"paralelo_xz-2.BMP");
// oc2.get(0,1,0,-192,"paralelo_xz-3.BMP");
// oc2.get(0,1,0,-256,"paralelo_xz-4.BMP");
// oc2.get(0,1,0,-384,"paralelo_xz-5.BMP");

//Paralelo al plano yz
// oc2.get(1,0,0,-54,"paralelo_yz-1.BMP");
// oc2.get(1,0,0,-128,"paralelo_yz-2.BMP");
// oc2.get(1,0,0,-192,"paralelo_yz-3.BMP");
// oc2.get(1,0,0,-256,"paralelo_yz-4.BMP");
// oc2.get(1,0,0,-384,"paralelo_yz-5.BMP");


//Perpendicular xz
// oc2.get(-15.0/512,0,1,-15,"Perpendicular_xz-1.BMP");
// oc2.get(15.0/512,0,1,-15,"Perpendicular_xz-2.BMP");
// oc2.get(20.0/512,0,1,-20,"Perpendicular_xz-3.BMP");
// oc2.get(-20.0/512,0,1,-20,"Perpendicular_xz-4.BMP");

//Perpendicular xy
// oc2.get(-0.5,1,0,-256,"Perpendicular_xy-1.BMP");
// oc2.get(0.5,1,0,-256,"Perpendicular_xy-2.BMP");
// oc2.get(-0.7,1,0,-190,"Perpendicular_xy-3.BMP");
// oc2.get(0.7,1,0,-256,"Perpendicular_xy-4.BMP");




}
