/* opengl操作 */
#include "glwidget.h"
#include "mainwindow.h"
#include "windows.h"
#include <QOpenGLFunctions>
void CALLBACK vertexCallback(GLvoid* vertex)
{
    GLdouble* pt;

    int numb;
    pt = (GLdouble*)vertex;

    glVertex3d(pt[0],pt[1],pt[2]);
}

void CALLBACK beginCallback(GLenum type)
{
    glBegin(type);
}

void CALLBACK endCallback()
{
    glEnd();
}

void CALLBACK errorCallback(GLenum errorCode)
{
    const GLubyte * estring;
    estring = gluErrorString(errorCode);
    fprintf(stderr, "Tessellation Error: %s/n", estring);
    exit(0);
}


GLWidget::GLWidget(QWidget *parent) :
    QOpenGLWidget(parent)
{
    sizeBefore=QSize(0,0);
    lightOn=false;
    lightMode=false;
    drawLine=true;
    updateMatrix=true;
    father=parent;
    zscale=500;
    isFirstCall=0;
    lists=1;
    _wheelScale=1.0;
    currentSection=-1;
    currentFaceId=0;
    verticalAngle = 0.0f;
    horizonAngle = 0.0f;
    center.setX(0);
    center.setZ(0);
    center.setY(0);
    /*the coordinate blew need to coordinated with the input flight*/
    eye.setX(1.f);
    eye.setY(1.f);
    eye.setZ(1.f);
    eye.normalize();

    upside.setX(1.f);
    upside.setY(0.f);
    upside.setZ(0.f);
    upside.normalize();

    TempTranslateVec.setX(0.0);
    TempTranslateVec.setY(0.0);
    TempTranslateVec.setZ(0.0);


    viewport.setX(0.f);
    viewport.setY(0.f);
    viewport.setWidth(911);
    viewport.setHeight(641);
    currentLoopId=-1;
    connect(father,SIGNAL(resized()),this,SLOT(update()));
}
void GLWidget::initializeGL()
{
    setGeometry(0, 0, father->width(),father->height());
    glShadeModel( GL_SMOOTH );
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_AUTO_NORMAL);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    //    glEnable(GL_POLYGON_SMOOTH);
    //    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset (1.0f, 1.0f);
    glEnable( GL_COLOR_MATERIAL );
    glLineWidth(0.5);
    glEnable( GL_DEPTH_TEST );                      //启用深度测试
    glEnable(GL_CULL_FACE);
    glDepthFunc( GL_LEQUAL );
    glClearColor( 82.0/255, 87.0/255, 110.0/255,0.0);                //the same color as paraview's background
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_FRONT); //启用剔除操作效果
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 10.0 };
    GLfloat light_position0[] = { -1.0, 10000.0, 1.0, -1.0 };
    GLfloat light_position1[] = { -1.0, -10000.0, 1.0, -1.0 };
    GLfloat light_position2[] = { -1.0, -1.0, 10000.0, -1.0 };
    GLfloat light_position3[] = { -1.0, -1.0, -10000.0, -1.0 };
    GLfloat light_position4[] = { -10000.0, -1.0, 1.0, -1.0 };
    GLfloat light_position5[] = { 10000.0, -1.0, 1.0, -1.0 };
    GLfloat white_light[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat sp_light[] = { 0.4, 0.4, 0.4, 1.0 };
    GLfloat Light_Model_Ambient[] = { 0.8, 0.8, 0.8, 1.0 };

    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);


    glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, sp_light);

    glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
    glLightfv(GL_LIGHT1, GL_AMBIENT_AND_DIFFUSE , white_light);
    glLightfv(GL_LIGHT1, GL_SPECULAR, sp_light);

    glLightfv(GL_LIGHT2, GL_POSITION, light_position2);
    glLightfv(GL_LIGHT2, GL_AMBIENT_AND_DIFFUSE , white_light);
    glLightfv(GL_LIGHT2, GL_SPECULAR, sp_light);

    glLightfv(GL_LIGHT3, GL_POSITION, light_position3);
    glLightfv(GL_LIGHT3, GL_AMBIENT_AND_DIFFUSE , white_light);
    glLightfv(GL_LIGHT3, GL_SPECULAR, sp_light);

    glLightfv(GL_LIGHT4, GL_POSITION, light_position4);
    glLightfv(GL_LIGHT4, GL_AMBIENT_AND_DIFFUSE , white_light);
    glLightfv(GL_LIGHT4, GL_SPECULAR, sp_light);

    glLightfv(GL_LIGHT5, GL_POSITION, light_position5);
    glLightfv(GL_LIGHT5, GL_AMBIENT_AND_DIFFUSE , white_light);
    glLightfv(GL_LIGHT5, GL_SPECULAR, sp_light);


    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, Light_Model_Ambient);

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
    glEnable(GL_LIGHT3);
    glEnable(GL_LIGHT4);
    glEnable(GL_LIGHT5);

}
void GLWidget::resizeGL(int w, int h)
{
    setGeometry(0, 0, w,h);
    glViewport(0, 0, w,h);
    glMatrixMode(GL_PROJECTION);                      //选择投影矩阵
    glLoadIdentity();                                 //重置投影矩阵
    gluPerspective(45, (double) w / h, 1, 1000000);
    glMatrixMode(GL_MODELVIEW);                       //选择模型观察矩阵
    update();
}
void GLWidget::drawString(std::string str){//draw ascll
    if( isFirstCall ) {
        isFirstCall = 0;
        lists = glGenLists(128);
    }
    wglUseFontBitmaps(wglGetCurrentDC(), 0, 128, lists);//need windows.h
    for(int i=0;i<str.size();i++)
        glCallList(lists +str[i] );
}
void GLWidget::turnOnOffLight(){
    lightOn=!lightOn;
    update();
}
void GLWidget::changeLightMode(){
    lightMode=!lightMode;
    update();
}

void GLWidget::changeDrawLine()
{
    drawLine=!drawLine;
    update();
}

void GLWidget::changeDrawFace()
{
    update();
}
void GLWidget::drawAxis()
{
    //    glEnable(GL_BLEND);
    //        //glEnable(GL_LINE_SMOOTH);
    //        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    QVector3D iniPos(center.x(),center.y(),center.z());//the center pos of axis

    glColor3f(0.9f, 0.9f, 0.9f);
    glRasterPos3f(center.x()+450,center.y(),center.z());
    drawString("x");
    glRasterPos3f(center.x(),center.y()+450,center.z());
    drawString("y");
    glRasterPos3f(center.x(),center.y(),center.z()+450);
    drawString("z");

    glLineWidth(1);
    glBegin(GL_LINES);
    glColor3f(0.0f, 0.0f, 0.8f);
    glVertex3f((iniPos).x(),(iniPos).y()+450.0,(iniPos).z());
    glVertex3f(iniPos.x(),iniPos.y(),iniPos.z());
    glColor3f(0.0f, 0.8f, 0.0f);
    glVertex3f((iniPos).x(),(iniPos).y(),(iniPos).z()+450.0);
    glVertex3f(iniPos.x(),iniPos.y(),iniPos.z());
    glColor3f(0.8f, 0.0f, 0.0f);
    glVertex3f((iniPos).x()+450.0,(iniPos).y(),(iniPos).z());
    glVertex3f(iniPos.x(),iniPos.y(),iniPos.z());
    glColor4f(1.0,1.0,1.0,0.5);
    glLineWidth(0.4);
    glVertex3f((iniPos).x()+150.0,(iniPos).y(),(iniPos).z());
    glVertex3f(iniPos.x()+150.0,iniPos.y()+150.0,iniPos.z());
    glVertex3f((iniPos).x()+150.0,(iniPos).y()+150,(iniPos).z());
    glVertex3f(iniPos.x(),iniPos.y()+150.0,iniPos.z());
    glVertex3f((iniPos).x()+150.0,(iniPos).y(),(iniPos).z()+150.0);
    glVertex3f(iniPos.x()+150.0,iniPos.y(),iniPos.z());
    glVertex3f((iniPos).x()+150.0,(iniPos).y(),(iniPos).z()+150.0);
    glVertex3f(iniPos.x(),iniPos.y(),iniPos.z()+150.0);
    glVertex3f((iniPos).x(),(iniPos).y()+150.0,(iniPos).z()+150.0);
    glVertex3f(iniPos.x(),iniPos.y()+150.0,iniPos.z());
    glVertex3f((iniPos).x(),(iniPos).y()+150.0,(iniPos).z()+150.0);
    glVertex3f(iniPos.x(),iniPos.y(),iniPos.z()+150.0);
    glVertex3f((iniPos).x()+150.0,(iniPos).y()+150.0,(iniPos).z()+150.0);
    glVertex3f(iniPos.x()+150.0,iniPos.y()+150.0,iniPos.z());
    glVertex3f((iniPos).x()+150.0,(iniPos).y()+150.0,(iniPos).z()+150.0);
    glVertex3f(iniPos.x()+150.0,iniPos.y(),iniPos.z()+150.0);
    glVertex3f((iniPos).x()+150.0,(iniPos).y()+150.0,(iniPos).z()+150.0);
    glVertex3f(iniPos.x(),iniPos.y()+150.0,iniPos.z()+150.0);
    glEnd();

    glBegin(GL_POINTS);
    glPointSize(70);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(iniPos.x(),iniPos.y(),iniPos.z());
    glEnd();
    glLineWidth(0.8);
    glPointSize(3);
}
void GLWidget::paintGL()
{

    if(father->size()!=sizeBefore){
        sizeBefore=father->size();
        resizeGL(sizeBefore.width(),sizeBefore.height());
    }
    glDisable( GL_DEPTH_TEST );
    glPolygonMode(GL_FRONT_AND_BACK ,GL_FILL );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(-10,10,-10,10);
    glBegin(GL_POLYGON);//多边形
    glColor3f(255/255, 255/255, 255/255);//r
    glVertex2f(-10.0f, -10.0f);
    glColor3f(7.0/255, 115.0/255, 151.0/255);//r
    glVertex2f(10.0f, -10.0f);
    glColor3f(36.0/255, 56.0/255, 76.0/255);//r
    glVertex2f(10.0f, 10.0f);
    glColor3f(36.0/255, 56.0/255, 76.0/255);//r
    glVertex2f(-10.0f, 10.0f);
    glEnd();
    glPopMatrix();
    glEnable( GL_DEPTH_TEST );
    glMatrixMode(GL_MODELVIEW);
    const float pi=3.14159265358;
    glLoadIdentity();
    //if(updateMatrix){
        transforMatrix1=matrix4d(upside,pi*horizonAngle/180.0);
        transforMatrix2=matrix4d(QVector3D::crossProduct(eye-center,upside).normalized(),-1*pi*verticalAngle/180.0);
        //updateMatrix=false;
    //}
    eye=transforMatrix1.dotProduct(eye);
    eye=transforMatrix2.dotProduct(eye);
    upside=transforMatrix2.dotProduct(upside);
    glTranslatef(TempTranslateVec.x(),TempTranslateVec.y(),TempTranslateVec.z());
    gluLookAt(zscale*eye.x(),zscale*eye.y(),zscale*eye.z(),
              center.x(),center.y(),center.z(),
              upside.x(),upside.y(),upside.z());

    glBegin(GL_POINTS);
    glColor3f(1,1,1);
    glPointSize(10.0);
    for(GLuint i=0;i<Vertex::vertexArray.size();i++){
        glVertex3f(10*Vertex::vertexArray[i]->coordinate.x(),10*Vertex::vertexArray[i]->coordinate.y(),10*Vertex::vertexArray[i]->coordinate.z());

    }
    glEnd();

    bool selected=false;
    for(GLuint i=0;i<Loop::loopArray.size();i++){
        if(Loop::loopArray[i]->halfedge_list==nullptr)
            continue;
        if(i==currentLoopId){
            selected=true;
        }
        else{
            selected=false;
        }
        drawLoop(Loop::loopArray[i],selected);
    }
    Face* fit=Solid::GetEntityPtr()->face_list;
    while(true){
        if(fit==nullptr)
            break;
        if(fit->outLp==nullptr)
            break;
        if(fit->outLp->halfedge_list==nullptr)
            break;
        if(fit->id==currentFaceId)
            drawFace(fit,true);
        else
            drawFace(fit,false);
        fit=fit->next;
        if(fit==Solid::GetEntityPtr()->face_list)
            break;
    }
    glColor3f(0,0,0);
    for(GLuint i=0;i<Vertex::vertexArray.size();i++){
        glRasterPos3f(10*Vertex::vertexArray[i]->coordinate.x(),10*Vertex::vertexArray[i]->coordinate.y(),10*Vertex::vertexArray[i]->coordinate.z());
        drawString(QString::number(Vertex::vertexArray[i]->id).toStdString());
    }
    glLoadIdentity();//initial identity
    QVector3D newEye=(eye-center).normalized()*7000+center;//keep the distence between eye and center=7000(or better value)
    glTranslatef(0,0,0);
    gluLookAt(newEye.x(), newEye.y(), newEye.z(),
              center.x(), center.y(), center.z(),
              upside.x(),upside.y(),upside.z());
    glViewport(this->width()*-0.41,-0.35*this->height(),
               this->width(), this->height());
    glDisable( GL_DEPTH_TEST );
    /*draw coordinate axis*/
    drawAxis();

    glEnable( GL_DEPTH_TEST );

    horizonAngle=0;
    verticalAngle=0;


}
void GLWidget::xyTranslate(int dx, int dy)
{
    currPos.setX(dx);
    currPos.setY(dy);
    QVector3D vec(-lastPos.x()+currPos.x(),lastPos.y()-currPos.y(),0);
    TempTranslateVec=TempTranslateVec+vec*(zscale/1000+0.01)/1;
    lastPos=currPos;
}

void GLWidget::xyRotate(int dx, int dy)
{
    currPos.setX(dx);
    currPos.setY(dy);
    dx-=lastPos.x();
    dy-=lastPos.y();
    horizonAngle=dx/6.0f;
    //if(_verticalAngle+dy <89 && _verticalAngle+dy >-89)
    verticalAngle=dy/6.0f;

    lastPos=currPos;//Update the mouse coordinate
}

void GLWidget::xyScale(int dx, int dy)
{
    currPos.setX(dx);
    currPos.setY(dy);
    dx-=lastPos.x();
    dy-=lastPos.y();
    if(dx < 4.0&&dx>-4 && dy < 4.0&&dy>-4)
        return;
    zScale(3*(dy+dx));
}

void GLWidget::zScale(float dz)
{
    if(_wheelScale-dz*0.0003f>-20)
        _wheelScale-=dz*0.0003f;
    zscale=pow(2.0f,_wheelScale)*250.0f;
}

void GLWidget::drawHalfEdge(HalfEdge* he,bool selected,QVector3D normal)
{
    //QVector3D
     QVector3D temp,temp1,temp2;
    if(selected){
        glLineWidth(3);
        glColor3f(1,0,0);
        temp=QVector3D::crossProduct(normal,he->sv->coordinate-he->ev->coordinate).normalized();
        temp1=2*temp+10*he->ev->coordinate+(he->sv->coordinate-he->ev->coordinate).normalized()*2;
        temp2=-2*temp+10*he->ev->coordinate+(he->sv->coordinate-he->ev->coordinate).normalized()*2;
    }
    else{
        glLineWidth(0.5);
        glColor3f(0.3f,0.8f,0.2f);
    }

    glBegin(GL_LINES);
    glVertex3f(10*he->sv->coordinate.x(),10*he->sv->coordinate.y(),10*he->sv->coordinate.z());
    glVertex3f(10*he->ev->coordinate.x(),10*he->ev->coordinate.y(),10*he->ev->coordinate.z());
    if(selected){
        glVertex3f(10*he->ev->coordinate.x(),10*he->ev->coordinate.y(),10*he->ev->coordinate.z());
        glVertex3f(temp1.x(),temp1.y(),temp1.z());
        glVertex3f(10*he->ev->coordinate.x(),10*he->ev->coordinate.y(),10*he->ev->coordinate.z());
        glVertex3f(temp2.x(),temp2.y(),temp2.z());
    }
    glEnd();
}

void GLWidget::drawFace(Face *fa, bool selected)
{
    Loop* outLoop=fa->outLp;
    vector<Loop*> innerLoop;
    Loop* it=fa->innerLp;
    while(true){
        if(it==nullptr)
            break;
        innerLoop.push_back(it);
        it=it->next;
        if(it==fa->innerLp){
            break;
        }
    }
    GLUtesselator       *tobj=gluNewTess();
    gluTessCallback(tobj, GLU_TESS_VERTEX, (void (CALLBACK *)())vertexCallback);
    gluTessCallback(tobj, GLU_TESS_BEGIN, (void (CALLBACK *)())beginCallback);
    gluTessCallback(tobj, GLU_TESS_END, (void (CALLBACK *)())endCallback);
    gluTessCallback(tobj, GLU_TESS_ERROR, (void (CALLBACK *)())errorCallback);

    double data[1000][3];
    int count=0;
    gluTessBeginPolygon(tobj, nullptr);
    gluTessBeginContour(tobj);
    HalfEdge* he=outLoop->halfedge_list;
    drawLoop(outLoop,selected);
    if(selected)
        glColor3f(0.3,0.3,0.9);
    else
        glColor3f(1,1,1);
    while(true){
        he=he->next;
        data[count][0]=10.0*he->sv->coordinate.x();
        data[count][1]=10.0*he->sv->coordinate.y();
        data[count][2]=10.0*he->sv->coordinate.z();
        //GUI->print(QString::number(data[count][0])+" "+QString::number(data[count][1])+" "+QString::number(data[count][2]));
        gluTessVertex(tobj, data[count], data[count]);
        count++;
        if(he==outLoop->halfedge_list)
            break;
    }
    gluTessEndContour(tobj);
    for(unsigned int i=0;i<innerLoop.size();i++){
        drawLoop(innerLoop[i],selected);
        if(selected)
            glColor3f(0.8,0.8,0.8);
        else
            glColor3f(0.8,0.8,0.7);
        gluTessBeginContour(tobj);
        he=innerLoop[i]->halfedge_list;
        while(true){
            he=he->next;
            data[count][0]=10*he->sv->coordinate.x();
            data[count][1]=10*he->sv->coordinate.y();
            data[count][2]=10*he->sv->coordinate.z();
            gluTessVertex(tobj, data[count], data[count]);
            count++;
            if(he==innerLoop[i]->halfedge_list)
                break;
        }
        gluTessEndContour(tobj);
    }
    gluTessEndPolygon(tobj);

}

void GLWidget::drawLoop(Loop *lp, bool selected)
{
    HalfEdge* it=lp->halfedge_list;
    lp->normal=lp->getOritation();
    while(true){
        drawHalfEdge(it,selected,lp->normal);
        it=it->next;
        if(it==lp->halfedge_list)
            break;
    }
}
void GLWidget::mousePressEvent(QMouseEvent *event)
{
    setFocus();
    updateMatrix=true;
    if(event->buttons() & Qt::LeftButton)
    {
        //if(VM==SELECTMODE)
        //selectObjects();
    }
    else if(event->buttons() & Qt::RightButton)
    {
    }
    else if(event->buttons() & Qt::MidButton){

    }
    lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    QPoint qPoint = event->pos();// - lastPos; // mouse move right x+, down y+
    if(event->buttons() & Qt::LeftButton)
    {
        xyRotate(qPoint.x(), qPoint.y());
    }
    else if(event->buttons() & Qt::MidButton)
    {
        xyTranslate(qPoint.x(), qPoint.y());
    }
    else{
        xyScale(qPoint.x(), qPoint.y());
    }
    lastPos = event->pos();//Update the mouse coordinate
    update();
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
    int qwheel = event->delta();  // positive when wheel up, delta +/- 120 when wheel +/- 1
    zScale(qwheel*5.0);
    update();
}

GLWidget::~GLWidget(){

}
