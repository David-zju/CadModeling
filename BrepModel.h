# pragma once
#include "ds.h"
#include <iostream>
#include <fstream>
#include <QDebug>
#include <vector>
#include "Entity.h"
using namespace std;

class BrepModel:public Entity<BrepModel>
{
public:
    BrepModel();
    static BrepModel* Instance(){return Entity<BrepModel>::GetEntityPtr();}

    void mvfs(QVector3D coordinate);
    Vertex* mev(Vertex *sv, QVector3D coordinate, Loop *loop);
    void mef(Vertex* sv, Vertex *ev, Loop *loop=nullptr);
    void kemr(Vertex *sv, Vertex *ev, Loop *loop);
    void kemr(Edge* e, Loop *loop);
    void semv(Vertex *sv, Vertex *ev,QVector3D coordinate);
    void kfmrh(Face *fa, Face *fb);
    void semv(Edge* e,QVector3D coordinate);
    void sweep(QVector3D oritation, Loop *loop);
    void clearData();

private:
    void updateGUI();
    Edge* getEdgeBy2Vertex(const Vertex* v1,const Vertex* v2);
    HalfEdge* gethfEdgeInLoopByVertex(const Loop* loop, const Vertex* v, int rank=1);
    inline void deleteEdgeInList(Edge* edgeToDelete);
    inline void addEdge(Edge* edgeToAdd);
    inline void deleteFaceInList(Face* faceToDelete);
    inline void addFace(Face* faceToAdd);
    bool hfEdgeInLoop(HalfEdge* he,Loop* l);
};
int LoadSketch(string path);
void SweepSketch(QVector3D orientation, int LoopNum);