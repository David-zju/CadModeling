/* main */
#include <iostream>
#include "BrepModel.h"
#include <QApplication>
#include "mainwindow.h"
#include <QTranslator>
using namespace std;

int main(int argc,char** argv)
{
    QApplication a(argc, argv);

    /*
    QTranslator translator;
    translator.load(a.applicationDirPath()+"/brepop.qm");
    a.installTranslator(&translator);
    QFile qss(a.applicationDirPath()+"/blue.qss");
    qss.open(QFile::ReadOnly);
    qApp->setStyleSheet(qss.readAll());
    qss.close(); */

    MainWindow w;
    w.show();
    int LoopNum = LoadSketch("C:/Users/David/Desktop/sketch.txt");
    SweepSketch(QVector3D(-3,6,7),LoopNum);
    // BrepModel::Instance()->mvfs(QVector3D(-3.0,-3.0,0));
    // BrepModel::Instance()->mev(Vertex::vertexArray[0],QVector3D(7.0,-3.0,0),Loop::loopArray[0]);
    // BrepModel::Instance()->mev(Vertex::vertexArray[1],QVector3D(7.0,7.0,0),Loop::loopArray[0]);
    // BrepModel::Instance()->mev(Vertex::vertexArray[2],QVector3D(-3.0,7.0,0),Loop::loopArray[0]);
    // BrepModel::Instance()->mef(Vertex::vertexArray[3],Vertex::vertexArray[0],Loop::loopArray[0]);
    // BrepModel::Instance()->mev(Vertex::vertexArray[0],QVector3D(2,2,0),Loop::loopArray[1]);
    // BrepModel::Instance()->mev(Vertex::vertexArray[4],QVector3D(4,2,0),Loop::loopArray[1]);
    // BrepModel::Instance()->mev(Vertex::vertexArray[5],QVector3D(4,4,0),Loop::loopArray[1]);
    // BrepModel::Instance()->mev(Vertex::vertexArray[6],QVector3D(2,4,0),Loop::loopArray[1]);
    // BrepModel::Instance()->mef(Vertex::vertexArray[7],Vertex::vertexArray[4],Loop::loopArray[1]);

    // BrepModel::Instance()->mev(Vertex::vertexArray[0],QVector3D(-2,-2,0),Loop::loopArray[1]);
    // BrepModel::Instance()->mev(Vertex::vertexArray[8],QVector3D(0,-2,0),Loop::loopArray[1]);
    // BrepModel::Instance()->mev(Vertex::vertexArray[9],QVector3D(0,0,0),Loop::loopArray[1]);
    // BrepModel::Instance()->mev(Vertex::vertexArray[10],QVector3D(-2,0,0),Loop::loopArray[1]);
    // BrepModel::Instance()->mef(Vertex::vertexArray[11],Vertex::vertexArray[8],Loop::loopArray[1]);
    // //    BrepModel::Instance()->kemr(Vertex::vertexArray[0],Vertex::vertexArray[4],Loop::loopArray[1]);
    // BrepModel::Instance()->kemr(Vertex::vertexArray[0],Vertex::vertexArray[8],Loop::loopArray[1]);

    // BrepModel::Instance()->kemr(Vertex::vertexArray[0],Vertex::vertexArray[4],Loop::loopArray[1]);
    // BrepModel::Instance()->kfmrh(Loop::loopArray[3]->face,Loop::loopArray[0]->face);
    // BrepModel::Instance()->kfmrh(Loop::loopArray[2]->face,Loop::loopArray[0]->face);
    // BrepModel::Instance()->sweep(QVector3D(0,0,7),Loop::loopArray[0]);
    // BrepModel::Instance()->sweep(QVector3D(0,0,7),Loop::loopArray[2]);
    // BrepModel::Instance()->sweep(QVector3D(0,0,7),Loop::loopArray[3]);
    // printf("%d\n",Loop::loopArray.size());
    return a.exec();





}
