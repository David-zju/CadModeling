/* 基于opengl的操作 */
# pragma once
#include <stdlib.h>
#include <QOpenGLWidget>
#include <gl/GLU.h>
#include <gl/GL.h>
#pragma comment (lib, "opengl32.lib")  /* link with Microsoft OpenGL lib */
#pragma comment (lib, "glu32.lib")     /* link with OpenGL Utility lib */
#include <iostream>
#include <vector>
#include <QVector3D>
#include <QMouseEvent>
#include <cmath>
#include <matrix4d.h>
#include <string>
#include "BrepModel.h"

class GLWidget : public QOpenGLWidget
{
    Q_OBJECT

public :
    GLWidget(QWidget *parent = nullptr);
    ~GLWidget();
    void drawString(std::string str);
    void drawAxis();
    void resizeGL(int w, int h);
    void initializeGL();
    void paintGL();
    void mousePressEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);

    void wheelEvent(QWheelEvent *event);

    void xyTranslate(int dx, int dy);

    void xyRotate(int dx, int dy);

    void xyScale(int dx, int dy);
        /*handle the wheel event*/
    void zScale(float dz);

    void drawHalfEdge(HalfEdge *he, bool selected, QVector3D normal);
    void drawFace(Face *fa,bool selected=false);
    void drawLoop(Loop *lp,bool selected=false);
    GLfloat horizonAngle;
    GLfloat verticalAngle;
    GLfloat zscale;
    GLfloat _wheelScale;
    GLfloat scaleForDraw;
    QPoint lastPos,currPos;
    QVector3D centermoved;
    QVector3D eye, upside;
    QVector3D center;
    QVector3D TempTranslateVec;
    QWidget *father;
    QSize sizeBefore;
    QColor qc;
public:
    QRectF viewport;
    int currentSection;
    int currentLoopId;
    int currentFaceId;
public slots:
    void turnOnOffLight();
    void changeLightMode();
    void changeDrawLine();
    void changeDrawFace();
private:
    GLuint eboID;
    matrix4d transforMatrix1;
    matrix4d transforMatrix2;
    bool lightOn;
    bool lightMode;
    bool drawLine;
    bool updateMatrix;
    int  isFirstCall;
    GLuint lists;
};
