

#include <fstream>
using namespace std;
class Octree {
    private:
        fstream save_file;
        int EPS = 2;

        void insert(Node* cur, vector<CImg<unsigned char>>& cube) {
            if(!verificar(cur->x1, cur->y1, cur->z1, cur->x2, cur->y2, cur->z2, cube)) {
                divide(cur);
                for(int i = 0; i < 8; ++i) {
                    auto child = cur->children[i];
                    insert(child->x1, child->y1, child->z1, child->x2, child->y2, child->z2, cube);
                }
            }
        }
};