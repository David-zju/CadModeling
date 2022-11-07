#include "ds.h"
#include "mainwindow.h"
int Vertex::numVertex=0;
int Edge::numEdge=0;
int Loop::numLoop=0;
int Face::numFace=0;
vector<Vertex*> Vertex::vertexArray=vector<Vertex*>();
vector<Loop*> Loop::loopArray=vector<Loop*>();

QVector3D Loop::getOritation()
{
    QVector3D temp;
    HalfEdge* it=halfedge_list;
    if(it==nullptr)
        return QVector3D(1,0,0);
    while(true){
        temp+=QVector3D::crossProduct(it->sv->coordinate-it->ev->coordinate,it->next->sv->coordinate-it->next->ev->coordinate);
        it=it->next;
        if(it==halfedge_list)
            break;
    }
    return temp.normalized();
}

void Face::addInerLoop(Loop* &l)
{

    if(innerLp!=nullptr){
        innerLp->pre->next=l;
        l->next=innerLp;
        l->pre=innerLp->pre;
        innerLp->pre=l;
        l->face=this;
    }
    else{
        innerLp=l;
        l->face=this;
        l->next=l;
        l->pre=l;
    }
}

Face::~Face(){
    this->pre->next=this->next;
    this->next->pre=this->pre;
}


