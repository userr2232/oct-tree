struct Node {
  int x1, y1, z1, x2, y2, z2;
  long dir;
  float c;

  long children[8];

  Node() = default;
  Node(int x1, int y1, int z1, int x2, int y2, int z2, float c): 
    x1{x1}, y1{y1}, z1{z1}, x2{x2}, y2{y2}, z2{z2}, c{c} {};
  void find_nodes(int A,int B,int C,int D,vector<Node> &nodos)
  {
    double v1=A*x1+B*y1+C*z1+D;
    double v2=A*x2+B*y2+C*z2+D;

    if(v1*v2>0) return;

    if(c!=-1)
    {
      nodos.push_back(*this);
    }

  }
};