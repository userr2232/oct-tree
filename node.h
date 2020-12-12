struct Node {
  int x1, y1, z1, x2, y2, z2;
  long dir;
  float c;

  long children[8];

  Node() = default;
  Node(int x1, int y1, int z1, int x2, int y2, int z2, float c): 
    x1{x1}, y1{y1}, z1{z1}, x2{x2}, y2{y2}, z2{z2}, c{c} {}
};