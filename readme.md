## 利用欧拉操作扫掠构造三维实体
#### 1.数据结构
依次定义实体，半边，边，环，面，顶点等数据结构
```C++
class Solid:public Entity<Solid>{
public:
    static Solid* GetEntityPtr(){return Entity<Solid>::GetEntityPtr();}
    Face *face_list; // list of all face_list to construct this solid
    Edge *edge_list; // list of all edge_list to construct this solid
    Solid() : face_list(nullptr), edge_list(nullptr){}
};

class HalfEdge{
public:
    Edge *edge; //this halfedge belong to which edge
    Vertex *sv; //the start vertex of this halfedge
    Vertex *ev; //the end vertex of this halfedge
    Loop *lp; //pointer to the loop that this halfedge belong to
    HalfEdge *next;
    HalfEdge *pre;
    HalfEdge *pair;
    HalfEdge() : edge(nullptr), sv(nullptr), lp(nullptr), next(nullptr), pre(nullptr), pair(nullptr){}
};

class Edge{
public:
    HalfEdge *half_l; //the edge's left halfedge
    HalfEdge *half_r; //the edge's right halfedge
    Edge *next;
    Edge *pre;
    int id;
    static int numEdge;
    Edge() : half_l(nullptr), half_r(nullptr), next(nullptr), pre(nullptr){id=numEdge++;}
};

class Loop{
public :
    int id;
    static vector<Loop*> loopArray;//an Automatic maintenance  pointer array
    static int numLoop;
    HalfEdge *halfedge_list; // list of all halfeges to construct this loop
    Face *face; // the face that constructed by this loop
    Loop *next;
    Loop *pre;
    QVector3D normal;
    QVector3D getOritation();//get normal 3Dvector
    Loop() : id(0), halfedge_list(nullptr), face(nullptr), next(nullptr), pre(nullptr){loopArray.push_back(this);id=numLoop++;}
};

class Face{
public:
    int id;
    Solid *solid; // the solid which the face belong to
    Loop *outLp; // out loop of the face--construct the face
    Loop *innerLp;//inner_lp of the face--inner loop
    Face *next;
    Face *pre;
    static int numFace;
    void addInerLoop(Loop *&l);
    Face() : id(0), solid(nullptr), outLp(nullptr), innerLp(nullptr),next(nullptr),pre(nullptr){
        id=numFace++;
    }
    ~Face();
};

class Vertex{
public:
    int id;
    static vector<Vertex*> vertexArray;//an Automatic maintenance  pointer array    whole vector
    QVector3D coordinate;               //coordinate of the vertex (x, y, z)
    Vertex *next;
    Vertex *pre;
    static int  numVertex;              //static num
    Vertex(QVector3D q) : next(nullptr), pre(nullptr)
    {
        coordinate=q;
        id=numVertex++;
        vertexArray.push_back(this);
    }
};
```
定义欧拉操作如下
```C++
    void mvfs(QVector3D coordinate);
    Vertex* mev(Vertex *sv, QVector3D coordinate, Loop *loop);
    void mef(Vertex* sv, Vertex *ev, Loop *loop=nullptr);
    void kemr(Vertex *sv, Vertex *ev, Loop *loop);
    void kemr(Edge* e, Loop *loop);
    void semv(Vertex *sv, Vertex *ev,QVector3D coordinate);
    void kfmrh(Face *fa, Face *fb);
    void semv(Edge* e,QVector3D coordinate);
```
#### 2.程序架构
由于本科专业为力学，暂时还未修读图形学，短时间内尝试学习OpenGL失败后选择了借鉴他人已实现的显示框架来显示B-rep模型。
```
CadModelingProject  // 工程目录
├── CadModelingProject.pro  //QT配置文件
│── Headers                 //C++ 头文件
│   ├── BrepModel.h         //定义欧拉操作
│   ├── ds.h                //数据结构
│   ├── glwidget.h          //基于Opengl的图形操作
│   ├── mainwindow.h        //主界面设计
│   ├── matrix4d.h          //图形设计
│   └── Entity.h         //单例模式设计
│── Sources               //C++源文件
│   ├── BrepModel.cpp       //欧拉操作的具体实现
│   ├── ds.cpp              //数据结构
│   ├── glwidget.cpp        //基于Opengl的图形操作
│   ├── main.cpp            //程序入口
│   ├── mainwindow.cpp      //主界面设计
│   └── matrix4d.cpp        //图形设计
└── Forms
    └── mainwindow.ui       // ui文件
```

#### 3.实现的主要功能
(1)实现了基于B-rep的数据结构<br>
(2)实现了6个最常用的欧拉操作,以及扫琼操作 <br>
>mvfs():定义一个体、一个面、一个外环、一个点 <br>
>mev():构造一个新点，同时构造一条连接新点与一给定点的边 <br>
>mef():构造一条新边，同时构造一个新面 <br>
>kemr():消去环中的一条边，构造一个内环 <br>
>kfmrh():删除一个面，并将其定义为一个内环，进而在体中构造一个柄，或将两个物体合并一个物体 <br>
>semv():在给定边上构造一个新点，同时将边分裂为两条边 <br>
>sweep():基于B-rep及相关的欧拉操作进行扫成操作

(3)利用Qt的OpenGL库进行图形绘制，将基于扫琼操作的实体模型进行可视化 <br>
(4)支持交互操作
#### 4.开发环境和配置
```
操作系统:Windows10 x64
集成开发环境:Desktop Qt5.14.1
编译器：msvc x64 2017
```

#### 5.实体模型生成操作
```C++
int LoopNum = LoadSketch("sketch_path"); // 读取记录草图信息的文本文件
SweepSketch(QVector3D(0,0,7),LoopNum); // 对草图进行扫掠操作
```
main函数中调用如上所示函数即可实现扫掠模型的构建。其中，草图信息文本文件的规范样例如下：
```C
3 // 该草图中面和内环的数量总和（圈圈的个数，也就是内环数量+1）
4 // 第一个环输入的节点个数
-3 -3 0 // 节点坐标（笛卡尔坐标系）
7 -3 0
7 7 0
-3 7 0
4 // 第二个环输入的节点个数
2 2 0
4 2 0
4 2.5 0
2 4 0
5
-2 -2 0
0 -2 0
1 -1 0
0 0 0
-2 0 0
// 注意，节点按照先后顺序排列形成的有向边按照右手定则判定后的方向，就是最后实体中环（面）的方向
// 如果方向不统一可能会导致绘制失败
```
生成结果如下图所示