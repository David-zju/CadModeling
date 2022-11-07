#include "BrepModel.h"
#include "mainwindow.h"

BrepModel::BrepModel()
{
}

void BrepModel::mvfs(QVector3D coordinate){
    Vertex* v=new Vertex(coordinate);
    Solid::GetEntityPtr()->face_list=new Face();
    Face* f=Solid::GetEntityPtr()->face_list;
    Loop* l=new Loop();
    v->pre=v;
    v->next=v;
    f->outLp=l;
    f->next=f;
    f->pre=f;
    l->face=f;
    f->solid=Solid::GetEntityPtr();
    Solid::GetEntityPtr()->face_list=f;
    GUI->print("mvfs  coordinate: "+QString::number(coordinate.x())+" "+QString::number(coordinate.y())+" "+QString::number(coordinate.z()));
    updateGUI();
}

Vertex *BrepModel::mev(Vertex *sv, QVector3D coordinate, Loop *loop){
    //make point
    Vertex* ev=new Vertex(coordinate);

    //make edge
    Edge* newEdge=new Edge();
    HalfEdge* hfEdgeLeft=new HalfEdge();
    HalfEdge* hfEdgeRight=new HalfEdge();

    //initial pointer
    newEdge->half_l=hfEdgeLeft;
    newEdge->half_r=hfEdgeRight;
    hfEdgeLeft->edge=newEdge;
    hfEdgeLeft->pair=hfEdgeRight;
    hfEdgeRight->pair=hfEdgeLeft;
    hfEdgeLeft->ev=ev;
    hfEdgeLeft->sv=sv;
    hfEdgeRight->sv=ev;
    hfEdgeRight->ev=sv;

    ev->next=sv->next;
    ev->pre=sv;
    sv->next->pre=ev;
    sv->next=ev;

    //loop operation
    if(loop->halfedge_list){
        HalfEdge* currentHalfEdge=gethfEdgeInLoopByVertex(loop,sv);
        currentHalfEdge->next->pre=hfEdgeRight;
        hfEdgeLeft->next=hfEdgeRight;
        hfEdgeRight->pre=hfEdgeLeft;
        hfEdgeRight->next=currentHalfEdge->next;
        hfEdgeLeft->pre=currentHalfEdge;
        currentHalfEdge->next=hfEdgeLeft;
    }
    else{
        loop->halfedge_list=hfEdgeLeft;
        hfEdgeLeft->next=hfEdgeRight;
        hfEdgeLeft->pre=hfEdgeRight;
        hfEdgeRight->next=hfEdgeLeft;
        hfEdgeRight->pre=hfEdgeLeft;
    }

    //edge operation
    if(Solid::GetEntityPtr()->edge_list!=nullptr){
        addEdge(newEdge);
    }
    else{
        Solid::GetEntityPtr()->edge_list=newEdge;
        newEdge->pre=newEdge;
        newEdge->next=newEdge;
    }  
    GUI->print("mev start vertex: "+QString::number(sv->id) +" new vertex coordinate: "+QString::number(coordinate.x())+" "+QString::number(coordinate.y())+" "+QString::number(coordinate.z())+" loop:"+QString::number(loop->id));
    updateGUI();
    return ev;
    //vertex operation

}

void BrepModel::mef(Vertex *sv, Vertex *ev,Loop* loop){
    bool findLoop=false;
    if(loop==nullptr)
        findLoop=true;
    else if(gethfEdgeInLoopByVertex(loop,sv)==nullptr||gethfEdgeInLoopByVertex(loop,ev)==nullptr){
        loop=nullptr;
        findLoop=true;
    }
    if(findLoop){
        for(int i=0;i<Loop::loopArray.size();i++){
            if(gethfEdgeInLoopByVertex(Loop::loopArray[i],sv)&&gethfEdgeInLoopByVertex(Loop::loopArray[i],ev)){
                loop=Loop::loopArray[i];
                break;
            }
        }
    }

    if(loop==nullptr){
        GUI->print("error occur in finding loop",InfoType::error);
        return;
    }
    HalfEdge* startHfEdge=gethfEdgeInLoopByVertex(loop,sv);
    HalfEdge* endHfEdge=gethfEdgeInLoopByVertex(loop,ev);
    if(hfEdgeInLoop(endHfEdge->pair,loop)){
        endHfEdge=endHfEdge->pair->pre;
    }

    //new element
    Loop *newLoop=new Loop();
    Face* newFace=new Face();
    Edge* newEdge=new Edge();
    HalfEdge* newHfEdLeft=new HalfEdge();
    HalfEdge* newHfEdRight=new HalfEdge();


    //build ds
    newHfEdLeft->sv=sv;
    newHfEdLeft->ev=ev;
    newHfEdRight->ev=sv;
    newHfEdRight->sv=ev;
    newEdge->half_l=newHfEdLeft;
    newEdge->half_r=newHfEdRight;
    newHfEdLeft->pair=newHfEdRight;
    newHfEdRight->pair=newHfEdLeft;
    newHfEdLeft->edge=newEdge;
    newHfEdRight->edge=newEdge;


    newFace->outLp=newLoop;
    newLoop->face=newFace;
    newFace->solid=Solid::GetEntityPtr();

    //build half edge
    newHfEdLeft->next=endHfEdge->next;
    newHfEdLeft->pre=startHfEdge;
    newHfEdRight->next=startHfEdge->next;
    newHfEdRight->pre=endHfEdge;
    /**/
    endHfEdge->next->pre=newHfEdLeft;
    endHfEdge->next=newHfEdRight;
    startHfEdge->next->pre=newHfEdRight;
    startHfEdge->next=newHfEdLeft;

    newLoop->halfedge_list=newHfEdRight;
    loop->halfedge_list=newHfEdLeft;

    //add face
    addFace(newFace);

    //add edge
    addEdge(newEdge);
    GUI->print("mef edge:"+QString::number(sv->id)+"---"+QString::number(ev->id)+" loop: "+QString::number(loop->id));
    updateGUI();

}

void BrepModel::kemr(Vertex *sv, Vertex *ev, Loop *loop){
    Edge* edgeToKill=getEdgeBy2Vertex(sv,ev);
    //GUI->print("find edge by two vertexs done",InfoType::info);
    kemr(edgeToKill,loop);
}

void BrepModel::kemr(Edge *e, Loop *loop){
    GUI->print("kemr edge:"+QString::number(e->half_l->sv->id)+"---"+QString::number(e->half_l->ev->id));
    Loop* newLoop=new Loop();
    HalfEdge* left=e->half_l;
    HalfEdge* right=e->half_r;
    if(loop->halfedge_list==left||loop->halfedge_list==right){
        GUI->print(" kill a begin edge",InfoType::error);
        return;
    }
    HalfEdge* it=left;
    while(it!=right){
        if(it==loop->halfedge_list){
            newLoop->halfedge_list=right->next;
            break;
        }
        it=it->next;
    }
    if(it==right){
        newLoop->halfedge_list=left->next;
    }
    //GUI->print("find new loop begin hfedge done",InfoType::info);
    HalfEdge* preNewLoop=newLoop->halfedge_list->pre->pair->next;
    HalfEdge* postNewLoop=newLoop->halfedge_list->pre->pre;
    //build new loop ds
    newLoop->halfedge_list->pre=preNewLoop->pre->pre;
    preNewLoop->pre->pre->next=newLoop->halfedge_list;

    //rebuild old loop ds
    preNewLoop->pre=postNewLoop;
    postNewLoop->next=preNewLoop;

    //add loop
    loop->face->addInerLoop(newLoop);

    //delete edge
    deleteEdgeInList(e);

    updateGUI();

}

void BrepModel::semv(Vertex *sv, Vertex *ev, QVector3D coordinate){
    Edge* edgeToSp=getEdgeBy2Vertex(sv,ev);
    //GUI->print("find edge by two vertexs done",InfoType::info);
    semv(edgeToSp,coordinate);
    updateGUI();
}

void BrepModel::kfmrh(Face *fa, Face *fb){
    GUI->print("kfmrh  kill face:"+QString::number(fa->id)+" Loop add to face:"+QString::number(fb->id));
    if(!fa->outLp){
        GUI->print("input face is empty",InfoType::error);
        return;
    }
    fb->addInerLoop(fa->outLp);
    fa->outLp->face=fb;
    deleteFaceInList(fa);
    updateGUI();
}

void BrepModel::semv(Edge *e, QVector3D coordinate){
    HalfEdge* left=e->half_l;
    HalfEdge* right=e->half_r;

    Vertex* v=new Vertex(coordinate);
    Edge* newEdge=new Edge();
    HalfEdge* newHfEdLeft=new HalfEdge();
    HalfEdge* newHfEdRight=new HalfEdge();


    //build ds
    newEdge->half_l=newHfEdLeft;
    newEdge->half_r=newHfEdRight;
    newHfEdLeft->pair=newHfEdRight;
    newHfEdRight->pair=newHfEdLeft;
    newHfEdLeft->edge=newEdge;
    newHfEdRight->edge=newEdge;

    //vertex pointer op
    newHfEdLeft->sv=v;
    newHfEdLeft->ev=left->ev;
    newHfEdRight->ev=v;
    newHfEdRight->sv=left->ev;
    left->ev=v;
    right->sv=v;

    //edge pointer op
    newHfEdLeft->next=left->next;
    newHfEdLeft->pre=left;
    newHfEdRight->pre=newHfEdLeft->next;
    newHfEdRight->next=right;
    right->pre=newHfEdRight;
    left->next=newHfEdLeft;
    addEdge(newEdge);
    GUI->print("semv edge:"+QString::number(e->half_l->sv->id)+"---"+QString::number(e->half_l->ev->id)+"coordinate: "+QString::number(coordinate.x())+" "+QString::number(coordinate.y())+" "+QString::number(coordinate.z()));
    updateGUI();
}

void BrepModel::sweep(QVector3D oritation, Loop *loop){
    HalfEdge* it=loop->halfedge_list;
    vector<Vertex*> vertexArray1;
    vector<Vertex*> vertexArray2;
    while(true){
        vertexArray1.push_back(it->sv);
        it=it->next;
        if(it==loop->halfedge_list)
            break;
    }
    for(unsigned int i=0;i<vertexArray1.size();i++){
        vertexArray2.push_back(mev(vertexArray1[i],vertexArray1[i]->coordinate+oritation,loop));
    }
    for(unsigned int i=0;i<vertexArray2.size();i++){
        mef(vertexArray2[i],vertexArray2[(i+1)%vertexArray2.size()],loop);
    }
    updateGUI();
}

void BrepModel::clearData(){
    while(Solid::GetEntityPtr()->edge_list){
        deleteEdgeInList(Solid::GetEntityPtr()->edge_list);
    }
    while(Solid::GetEntityPtr()->face_list){
        deleteFaceInList(Solid::GetEntityPtr()->face_list);
    }

}

void BrepModel::updateGUI(){
    GUI->updateGLWidget();
    GUI->updateTreeTable();
}

Edge *BrepModel::getEdgeBy2Vertex(const Vertex *v1, const Vertex *v2){
    Edge*  it=Solid::GetEntityPtr()->edge_list;
    if(!it){
        GUI->print("return a nullptr in getEdgeBy2Vertex;",InfoType::error);
        return nullptr;
    }
    while(true){
        if((it->half_l->ev==v1&&it->half_l->sv==v2)||(it->half_r->ev==v1&&it->half_r->sv==v2)){
            return it;
        }
        it=it->next;
        if(it==Solid::GetEntityPtr()->edge_list){
            GUI->print("return a nullptr in getEdgeBy2Vertex;",InfoType::error);
            return nullptr;
        }
    }
}

HalfEdge *BrepModel::gethfEdgeInLoopByVertex(const Loop *loop, const Vertex *v,int rank){
    if(!loop->halfedge_list){
        GUI->print("gethfEdgeInLoopByVertex, loop is nullptr",InfoType::error);
        return nullptr;
    }
    HalfEdge* it=loop->halfedge_list;
    int t=0;
    while(true){
        if(it->ev==v){
            t++;
            if(t==rank)
                return it;
        }
        it=it->next;
        if(it==loop->halfedge_list)
            return nullptr;
    }


}

void BrepModel::deleteEdgeInList(Edge *edgeToDelete){
    Edge* it=Solid::GetEntityPtr()->edge_list;
    while(true){//do not
        if(it==edgeToDelete){
            if(it->next==it){
                Solid::GetEntityPtr()->edge_list=nullptr;
            }
            else {
                if(Solid::GetEntityPtr()->edge_list==it){Solid::GetEntityPtr()->edge_list=it->next;}
                it->pre->next=it->next;
                it->next->pre=it->pre;
            }
            if(it->half_l)
            delete it->half_l;
            if(it->half_r)
            delete it->half_r;
            delete it;
            return;
        }
        it=it->next;
        if(it==Solid::GetEntityPtr()->edge_list)
            return ;
    }
}

void BrepModel::addEdge(Edge* edgeToAdd){
    Solid::GetEntityPtr()->edge_list->pre->next=edgeToAdd;
    edgeToAdd->pre=Solid::GetEntityPtr()->edge_list->pre;
    edgeToAdd->next=Solid::GetEntityPtr()->edge_list;
    Solid::GetEntityPtr()->edge_list->pre=edgeToAdd;
}

void BrepModel::deleteFaceInList(Face *faceToDelete){
    if(faceToDelete->next==faceToDelete){
        Solid::GetEntityPtr()->face_list=nullptr;
        delete faceToDelete;
        return;
    }
    if(faceToDelete==Solid::GetEntityPtr()->face_list){
        Solid::GetEntityPtr()->face_list=faceToDelete->next;
        deleteFaceInList(faceToDelete);
        return ;
    }
    faceToDelete->next->pre=faceToDelete->pre;
    faceToDelete->pre->next=faceToDelete->next;
    delete faceToDelete;
}

void BrepModel::addFace(Face *faceToAdd){
    Solid::GetEntityPtr()->face_list->pre->next=faceToAdd;
    faceToAdd->pre=Solid::GetEntityPtr()->face_list->pre;
    faceToAdd->next=Solid::GetEntityPtr()->face_list;
    Solid::GetEntityPtr()->face_list->pre=faceToAdd;
}

bool BrepModel::hfEdgeInLoop(HalfEdge *he, Loop *l){
    HalfEdge* it=l->halfedge_list;
    while(true){
        if(it==he)
            return true;
        it=it->next;
        if(it==l->halfedge_list)
            return false;
    }
}



int LoadSketch(string path){
    using namespace std;
    fstream infile(path);
    int LoopNum;
    int VertexNum;
    int CurrentNum = 0;
    vector<int> CurrentNumList;
    CurrentNumList.push_back(CurrentNum);
    infile>> LoopNum; infile >> VertexNum;
    double x,y,z;
    infile>>x; infile>>y; infile>>z;
    BrepModel::Instance()->mvfs(QVector3D(x,y,z));
    for(int VertexId = CurrentNum; VertexId<CurrentNum+VertexNum-1; VertexId++){
        infile>>x; infile>>y; infile>>z;
        BrepModel::Instance()->mev(Vertex::vertexArray[VertexId],QVector3D(x,y,z), Loop::loopArray[0]);
    }
    BrepModel::Instance()->mef(Vertex::vertexArray[CurrentNum+VertexNum-1],Vertex::vertexArray[0],Loop::loopArray[0]);
    CurrentNumList.push_back(CurrentNum+=VertexNum);
    if(LoopNum>1){
        for(int LoopId = 1; LoopId<LoopNum; LoopId++){
            infile>>VertexNum; 
            infile>>x; infile>>y; infile>>z;
            BrepModel::Instance()->mev(Vertex::vertexArray[0],QVector3D(x,y,z), Loop::loopArray[1]);
            for(int VertexId = CurrentNum; VertexId<CurrentNum+VertexNum-1; VertexId++){
                infile>>x; infile>>y; infile>>z;
                BrepModel::Instance()->mev(Vertex::vertexArray[VertexId],QVector3D(x,y,z), Loop::loopArray[1]);
            }
            BrepModel::Instance()->mef(Vertex::vertexArray[CurrentNum+VertexNum-1],Vertex::vertexArray[CurrentNum],Loop::loopArray[1]);
            CurrentNumList.push_back(CurrentNum+=VertexNum);
        }
        for(int LoopId = 1; LoopId<LoopNum; LoopId++){
            BrepModel::Instance()->kemr(Vertex::vertexArray[0],Vertex::vertexArray[CurrentNumList[LoopId]],Loop::loopArray[1]);
            BrepModel::Instance()->kfmrh(Loop::loopArray[LoopId+1]->face,Loop::loopArray[0]->face);
        }
    }
    // QVector3D orientation = QVector3D(0,0,7);
    // printf("%d\n",Loop::loopArray.size());
    // for(int i = 0; i<LoopNum; i++){
    //     if(i==1)BrepModel::Instance()->sweep(orientation, Loop::loopArray[i]);
    // }
    // // printf("%d\n",Loop::loopArray.size());
    // BrepModel::Instance()->sweep(QVector3D(0,0,7),Loop::loopArray[0]);
    // BrepModel::Instance()->sweep(QVector3D(0,0,7),Loop::loopArray[2]);
    // BrepModel::Instance()->sweep(QVector3D(0,0,7),Loop::loopArray[3]);
    return LoopNum;
}

void SweepSketch(QVector3D orientation,int LoopNum){
    for(int i=0; i<LoopNum+1; i++){
        if(i!=1){
            BrepModel::Instance()->sweep(orientation, Loop::loopArray[i]);
        }
    }
}
