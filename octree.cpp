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
            {
              cout<<x<<endl;
                images.push_back(CImg<float>(x.c_str()));
              }
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
            ofstream save_file(oct_file,std::ios::binary| std::ios::in |std::ios::ate);
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

            cur.children[0] = allocate_node(x1,ym,z1,xm,y2,zm, cube);//AR2(MORADO)=X1,YM,Z1 -> XM,Y2,ZM
            cur.children[1] = allocate_node(xm,ym,z1,x2,y2,zm, cube);//AR3(VERDE)=XM,YM,Z1 -> X2,Y2,ZM
            cur.children[2] = allocate_node(xm,y1,z1,x2,ym,zm, cube);//ARR4(CELESTE)=XM,Y1,Z1 -> X2,YM,ZM

            cur.children[3] = allocate_node(x1,y1,zm,xm,ym,z2, cube);//AB1(AMARILLO)= X1, Y1,ZM -> XM,YM,Z2
            cur.children[4] = allocate_node(x1,ym,zm,xm,y2,z2, cube);//AB2(BLANCO)= X1,YM,ZM -> XM,Y2,Z2
            cur.children[5] = allocate_node(xm,y1,zm,x2,ym,z2, cube);//AB3(CELESTE)=XM,Y1,ZM -> X2,YM,Z2
            cur.children[6] = allocate_node(xm,ym,zm,x2,y2,z2, cube);//AB4(VERDE)=XM,YM,ZM -> X2,Y2,Z2
            cur.children[7] = allocate_node(x1,y1,z1,xm,ym,zm, cube);// AR1(AMARILLO)=X1,Y1,Z1 -> XM,YN,ZM

            write(cur);
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


        void rebuild(vector<Node> &nodos)
        {

        }

       void find_nodes(Node& cur,float A,float B,float C,float D,vector<CImg<float>>& img)
        // void find_nodes(Node& cur,float A,float B,float C,float D,CImg<float>& img)

        {
            double x1=cur.x1;
            double x2=cur.x2;
            double y1=cur.y1;
            double y2=cur.y2;
            double z1=cur.z1;
            double z2=cur.z2;

            //cout<<x1<<" "<<y1<<" "<<z1<<" "<<x2<<" "<<y2<<" "<<z2<<" "<<cur.c<<endl;

            vector<pair<Point,Point>> points;
            points.push_back(make_pair(Point{x1,y1,z1},Point{x2,y2,z2}));
            points.push_back(make_pair(Point{x2,y1,z1},Point{x1,y2,z2}));
            points.push_back(make_pair(Point{x1,y1,z2},Point{x2,y2,z1}));
            points.push_back(make_pair(Point{x2,y1,z2},Point{x1,y2,z1}));

            bool r=0;
            for(auto i:points )
            {

              auto [p1,p2]=i;
              //cout<<(A*p1.x+B*p1.y+C*p1.z+D)*(A*p2.x+B*p2.y+C*p2.z+D)<<endl;
              if((A*p1.x+B*p1.y+C*p1.z+D)*(A*p2.x+B*p2.y+C*p2.z+D)<=0)
              {
                r=1;
                break;
              }
            }


            if(!r) return;



            if(cur.c!=-1 )
            {
              //plano xy
              if(z1==z2)return;
              for(int i=x1;i<x2;i++)
              {
                for(int j=y1;j<y2;j++)
                {
                  for(int k=z1;k<z2;k++)
                  {
                    if(abs(A*i+B*j+C*k+D)<1)
                    img[0](i,j)=cur.c;
                  }

                }
              }

              //plano yz
              if(x1==x2)return;
              for(int i=x1;i<x2;i++)
              {
                for(int j=y1;j<y2;j++)
                {
                  for(int k=z1;k<z2;k++)
                  {
                    if(abs(A*i+B*j+C*k+D)<1)
                    img[1](j,img[1].height()-1-k)=cur.c;
                  }

                }
              }
              //plano xz
              if(y1==y2)return;
              for(int i=x1;i<x2;i++)
              {
                for(int j=y1;j<y2;j++)
                {
                  for(int k=z1;k<z2;k++)
                  {
                    if(abs(A*i+B*j+C*k+D)<1)
                    img[2](i,img[1].height()-1-k)=cur.c;
                  }

                }
              }


              return;
            }


            for(int i=0;i<8;i++)
            {
              auto a=read(cur.children[i]);
              find_nodes(a, A, B, C, D,img);
            }

        }



    public:
        Octree(const char* nodesfile) {
            oct_file=nodesfile;
            cout<<oct_file<<endl;
        }
        Octree(string nodesfile, const char* fnames) {
            oct_file = nodesfile;

            build(fnames);
        }
        void get(float A,float B,float C,float D)
        {
            CImg<float> b("p1.BMP");
            CImg<float>img(b.width(),b.height());
            vector<CImg<float>>imgs;

            for(int i=0;i<3;i++)
            {
                imgs.push_back(img);
            }

            root=read(0);
            //cout<<b.width()<<" "<<b.height();
            find_nodes(root,A,B,C,D,imgs);
            // imgs[0].save("Result1.BMP");
            // imgs[1].save("Result2.BMP");
            // imgs[2].save("Result3.BMP");
            imgs[0].save("xy.BMP");
            imgs[1].save("yz.BMP");
            imgs[2].save("xz.BMP");

        }
};

Node read(long dir)
{
    ifstream save_file("octree.dat");
    save_file.seekg(dir);
    Node node{};
    save_file.read((char*)(&node),sizeof(Node));
    save_file.close();
    return node;
}
void go(Node n, vector<CImg<float>>& v)
{
    if(n.c!=-1)
    {
        for(long i = n.x1; i < n.x2; ++i) {
        for(long j = n.y1; j < n.y2; ++j) {
            for(long k = n.z1; k < n.z2; ++k) {
                v[k](i,j)=n.c;
            }
        }
    }
    }
    else
    {
        for(int i=0;i<8;i++)
        go(read(n.children[i]),v);
    }


}

void read()
{
    cout<<"hola"<<endl;
  vector<CImg<float>> v;
  CImg<float> b("p1.BMP");
  CImg<float> a(b.width(),b.height());
  v.push_back(a);
  v.push_back(a);
  v.push_back(a);


  ifstream f("octree.dat");
  Node n;
  f.read((char*)(&n),sizeof(Node));
  go(n,v);
  /*
  while(!f.eof())
  {
    f.read((char*)(&n),sizeof(Node));
    cout<<n.children[0]<<" "<<n.c<<endl;


    // cout<<n.x1<<" "<<n.y1<<" "<<n.z1<<" "<<n.x2<<" "<<n.y2<<" "<<n.z2<<endl;

    if(n.c==-1) continue;
    for(long i = n.x1; i < n.x2; ++i) {
        for(long j = n.y1; j < n.y2; ++j) {
            for(long k = n.z1; k < n.z2; ++k) {
                v[k](i,j)=n.c;
            }
        }
    }
  }

  */


  v[0].save("1.BMP");
  v[1].save("2.BMP");
  v[2].save("3.BMP");
}
