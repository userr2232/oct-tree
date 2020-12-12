#include "node.h"
#include "Cimg.h"
#include <vector>
#include <iostream>
#include <string>

using namespace std;

class Octree {
    private:
        int EPS = 2;
        Node* root{0};

        vector<const char*> get_file_names(const char* fnames) {
            string fname;
            vector<const char*> res;
            while(getline(cin, fname))
                res.push_back(fname.c_str());
            return res;
        }

        void build(const char* fnames) {
            vector<const char*> v = get_file_names(fnames);
            vector<CImg<unsigned char>> images;
            for(auto x : v)
                images.push_back(CImg<unsigned char>(x));
            root = new Node(0, 0, 0, images[0].width(), images[0].height(), images.size(), images[0](0,0));
            insert(root, images);
        }

        void insert(Node* cur, vector<CImg<unsigned char>>& cube) {
            if(!verificar(cur->x1, cur->y1, cur->z1, cur->x2, cur->y2, cur->z2, cube)) {
                divide(cur);
                for(int i = 0; i < 8; ++i) {
                    auto child = cur->children[i];
                    insert(child->x1, child->y1, child->z1, child->x2, child->y2, child->z2, cube);
                }
            }
        }

        void divide(Node* cur) {
            // TODO:
        }

        void verificar(int x1, int y1, int z1, int x2, int y2, int z2, vector<CImg<unsigned char>>& cube) {
            if(x2 - x1 <= 1 && y2 - y1 <= 1 && z2 - z1 <= 1)
                return true;
            unsigned char c = cube[z1](x1, x2);
            for(int i = x1; i < x2; ++i) {
                for(int j = y1; j < y2; ++j) {
                    for(int k = z1; k < z2; ++k) {
                        if(abs(c - cube[k](i,j)) > EPS) return false;
                    }
                }
            }
            return true;
        }

    public:
        Octree(const char* nodesfile) {
            load(nodesfile);
        }
        Octree(const char* nodesfile, const char* fnames) {
            build(fnames);
            save(nodesfile);
        }
};