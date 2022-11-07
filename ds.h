# pragma once
#include <QWidget>
#include <stdlib.h>
#include <QVector3D>
#include <vector>
#include <iostream>
#include <QDebug>
#include "Entity.h"
using namespace std;
class Solid;
class Face;
class Loop;
class HalfEdge;
class Vertex;
class Edge;

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
