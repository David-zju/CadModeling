/* 窗体设计 */
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QToolButton>
#include "BrepModel.h"

MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent),ui(new Ui::MainWindow){
    ui->setupUi(this);
    qpb=new QProgressBar();
    statusLabel=new QLabel("ready");
    ui->infoWidget->setMinimumWidth(300);
    ui->opWidget->setMinimumWidth(300);
    ui->infoWidget->setMinimumHeight(300);
    ui->opWidget->setMinimumHeight(300);
    glw=new GLWidget(ui->openGLWidget_2);
    this->setCorner(Qt::BottomLeftCorner,Qt::LeftDockWidgetArea);
    this->setCorner(Qt::BottomRightCorner,Qt::RightDockWidgetArea);
    this->setCorner(Qt::TopLeftCorner,Qt::LeftDockWidgetArea);
    this->setCorner(Qt::TopRightCorner,Qt::RightDockWidgetArea);
    this->setCentralWidget(ui->openGLWidget_2);
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString current_date = current_date_time.toString("yyyy-MM-dd hh:mm:ss ddd");
    messageBox=new QTextBrowser();
    intInputBox=new QSpinBox();
    messageBox->append("-------------------------\n"+current_date+"\n-------------------------\n");
    ui->MessageDockWidget->setWidget(messageBox);
    ui->infoWidget->setWidget(ui->treeWidget);

    infoWidgetShow=new QAction(tr("Info"),this);
    infoWidgetShow->setCheckable(true);
    infoWidgetShow->setChecked(true);

    opWidgetShow=new QAction(tr("Op"),this);
    opWidgetShow->setCheckable(true);
    opWidgetShow->setChecked(true);

    messaWidgetShow=new QAction(tr("Message"),this);
    messaWidgetShow->setCheckable(true);
    messaWidgetShow->setChecked(true);

    connect(ui->infoWidget,SIGNAL(visibilityChanged(bool)),infoWidgetShow,SLOT(setChecked(bool)));
    connect(infoWidgetShow,SIGNAL(triggered(bool)),ui->infoWidget,SLOT(setVisible(bool)));
    connect(ui->opWidget,SIGNAL(visibilityChanged(bool)),opWidgetShow,SLOT(setChecked(bool)));
    connect(opWidgetShow,SIGNAL(triggered(bool)),ui->opWidget,SLOT(setVisible(bool)));
    connect(ui->MessageDockWidget,SIGNAL(visibilityChanged(bool)),messaWidgetShow,SLOT(setChecked(bool)));
    connect(messaWidgetShow,SIGNAL(triggered(bool)),ui->MessageDockWidget,SLOT(setVisible(bool)));

    hideAll();
    ui->mvfsButton->setEnabled(true);
    ui->mefButton->setEnabled(false);
    ui->kemrButton->setEnabled(false);
    ui->kfmrhButton->setEnabled(false);
    ui->mevButton->setEnabled(false);
    ui->semvButton->setEnabled(false);
    ui->sweepButton->setEnabled(false);
    intInputBox->setMinimumWidth(60);
    ui->buttonBox->setEnabled(false);
    this->setWindowState(Qt::WindowMaximized);
    print("CAD B-rep model",InfoType::info);
    op=null;

}


void MainWindow::commandLineChangedTrigered(){
    QString com=commandLine->text();
    print(com,InfoType::info);
    commandLine->clear();
}


void MainWindow::updateTreeTable()
{
    //update vertex array
    while(ui->treeWidget->topLevelItem(2)->childCount()){
        QTreeWidgetItem* t=ui->treeWidget->topLevelItem(2)->child(0);
        ui->treeWidget->topLevelItem(2)->takeChild(0);
        delete t;
    }
    QList<QTreeWidgetItem*> qwil;
    for(unsigned int i=0;i< Vertex::vertexArray.size();i++){
        QTreeWidgetItem *qtw;
        QStringList qsl;
        qsl.append("VertexId:"+QString::number(Vertex::vertexArray[i]->id));
        qtw=new QTreeWidgetItem(ui->treeWidget->topLevelItem(2),qsl);
        qwil.append(qtw);
    }
    ui->treeWidget->addTopLevelItems(qwil);

    //update loop array
    while(ui->treeWidget->topLevelItem(1)->childCount()){
        QTreeWidgetItem* t=ui->treeWidget->topLevelItem(1)->child(0);
        ui->treeWidget->topLevelItem(1)->takeChild(0);
        delete t;
    }
    qwil.clear();
    for(unsigned int i=0;i< Loop::loopArray.size();i++){
        QTreeWidgetItem *qtw;
        QStringList qsl;
        qsl.append("LoopId:"+QString::number(Loop::loopArray[i]->id));
        qtw=new QTreeWidgetItem(ui->treeWidget->topLevelItem(1),qsl);
        qwil.append(qtw);
    }
    ui->treeWidget->addTopLevelItems(qwil);

    while(ui->treeWidget->topLevelItem(0)->childCount()){
        QTreeWidgetItem* t=ui->treeWidget->topLevelItem(0)->child(0);
        ui->treeWidget->topLevelItem(0)->takeChild(0);
        delete t;
    }
    qwil.clear();

    Face* fit=Solid::GetEntityPtr()->face_list;
    while(true){
        if(fit==nullptr)
            break;
        QTreeWidgetItem *qtw;
        QStringList qsl;
        qsl.append("FaceId:"+QString::number(fit->id));
        qtw=new QTreeWidgetItem(ui->treeWidget->topLevelItem(0),qsl);
        qwil.append(qtw);
        fit=fit->next;
        if(fit==Solid::GetEntityPtr()->face_list)
            break;
    }

    ui->treeWidget->addTopLevelItems(qwil);
}

void MainWindow::updateGLWidget()
{
    //update last loop's normal vector
    if(!Loop::loopArray.empty())
    Loop::loopArray[Loop::loopArray.size()-1]->normal=Loop::loopArray[Loop::loopArray.size()-1]->getOritation();
    glw->update();
}




void MainWindow::reDo(){

}
void MainWindow::unDo(){

}
void MainWindow::reposX(){
    //glw->reposX();
}
void MainWindow::fitView(){
    //glw->fitView();
}
void MainWindow::reposY(){
    //glw->reposY();
}
void MainWindow::reposZ(){
    //glw->reposZ();
}

void MainWindow::changeViewTypeTriggered()
{
    //glw->changeViewType();
}







void MainWindow::clearLayout(QLayout *layout)
{
    QLayoutItem *item;
    while((item = layout->takeAt(0)) != nullptr){
        if(item->widget()){
            layout->removeWidget(item->widget());
            item->widget()->setParent(nullptr);
            item->widget()->deleteLater();
        }
        QLayout *childLayout = item->layout();
        if(childLayout){
            clearLayout(childLayout);
        }
        delete item;
    }
}


void MainWindow::addActionToToolBar(QAction *qa, QToolBar *qtb, bool checkable){
    QToolButton *qtbu=new QToolButton();
    qtbu->setDefaultAction(qa);
    qtbu->setCheckable(checkable);
    qtb->addWidget(qtbu);
}



void MainWindow::hideAll()
{
    ui->label->hide();
    ui->label_2->hide();
    ui->label_3->hide();
    ui->label_4->hide();
    ui->label_5->hide();
    ui->doubleSpinBox->hide();
    ui->doubleSpinBox_2->hide();
    ui->doubleSpinBox_3->hide();
    ui->LoopIDspinBox->hide();
    ui->FaceIDspinBox->hide();
    ui->FaceIDspinBox_2->hide();
    ui->VertexIDspinBox->hide();
    ui->VertexIDspinBox_2->hide();
}



void MainWindow::resizeEvent(QResizeEvent *q){
    q->accept();
    glw->update();
}


MainWindow::~MainWindow()
{
    delete glw;
    delete ui;
}


void MainWindow::print(QString message,InfoType it){
    if(it==InfoType::warning){
        messageBox->append("");
        messageBox->insertHtml("<font color=\"orange\"><b>warning:</b></font><font color=\"black\"> </font>");
    }
    else if(it==InfoType::error){
        messageBox->append("");
        messageBox->insertHtml("<font color=\"red\"><b>error:</b></font><font color=\"black\"> </font>");
    }
    else if(it==InfoType::info){
        messageBox->append("");
        messageBox->insertHtml("<font color=\"green\"><b>info:</b></font><font color=\"black\"> </font>");
    }
    messageBox->insertPlainText(message);



}


void MainWindow::on_actionAbout_Qt_triggered()
{
    QMessageBox::aboutQt(this,"About qt");
}

void MainWindow::on_actionAbout_2_triggered()
{
    QMessageBox::about(this,"About Author",tr("DaiZiheng")+"\n" + tr("StudentID")+" :3190100770\n"+tr("3D_CAD_Modeling")+"\n");
}



void MainWindow::on_treeWidget_itemPressed(QTreeWidgetItem *item, int column)
{

    if(item->text(column).left(7)=="LoopId:")
        glw->currentLoopId=item->text(column).right(item->text(column).size()-item->text(column).lastIndexOf(':')-1).toInt();

    else if(item->text(column).left(7)=="FaceId:")
    {
        glw->currentFaceId=item->text(column).right(item->text(column).size()-item->text(column).lastIndexOf(':')-1).toInt();
    }

    else {
        glw->currentLoopId=-1;
        glw->currentFaceId=-1;
    }

    if(glw->currentLoopId!=-1||glw->currentFaceId!=-1)
    glw->update();
}

void MainWindow::on_mvfsButton_released()
{
    op=mvfs;
    hideAll();
    ui->label->show();
    ui->doubleSpinBox->show();
    ui->doubleSpinBox_2->show();
    ui->doubleSpinBox_3->show();
    ui->buttonBox->setEnabled(true);
}

void MainWindow::on_mevButton_released()
{
    op=mev;
    hideAll();
    ui->label->show();
    ui->doubleSpinBox->show();
    ui->doubleSpinBox_2->show();
    ui->doubleSpinBox_3->show();
    ui->label_2->show();
    ui->VertexIDspinBox->show();
    ui->label_3->show();
    ui->LoopIDspinBox->show();

}

void MainWindow::on_mefButton_released()
{
    op=mef;
    hideAll();
    ui->label_2->show();
    ui->VertexIDspinBox->show();
    ui->VertexIDspinBox_2->show();
    ui->label_3->show();
    ui->LoopIDspinBox->show();
}

void MainWindow::on_kemrButton_released()
{
    op=kemr;
    hideAll();
    ui->label_2->show();
    ui->VertexIDspinBox->show();
    ui->VertexIDspinBox_2->show();
    ui->label_3->show();
    ui->LoopIDspinBox->show();
}

void MainWindow::on_kfmrhButton_released()
{
    op=kfmrh;
    hideAll();
    ui->label_4->show();
    ui->FaceIDspinBox->show();
    ui->FaceIDspinBox_2->show();
}

void MainWindow::on_semvButton_released()
{
    op=semv;
    hideAll();
    ui->label->show();
    ui->doubleSpinBox->show();
    ui->doubleSpinBox_2->show();
    ui->doubleSpinBox_3->show();
    ui->label_2->show();
    ui->VertexIDspinBox->show();
    ui->VertexIDspinBox_2->show();
}

void MainWindow::on_buttonBox_accepted()
{
    if(op==mvfs){
        BrepModel::Instance()->mvfs(QVector3D(ui->doubleSpinBox->value(),ui->doubleSpinBox_2->value(),ui->doubleSpinBox_3->value()));
        ui->mvfsButton->setEnabled(false);
        ui->mevButton->setEnabled(true);
        on_mevButton_released();
    }
    else if(op==mev){
        Loop* lp;Vertex* vp;
        QVector3D newP=QVector3D(ui->doubleSpinBox->value(),ui->doubleSpinBox_2->value(),ui->doubleSpinBox_3->value());
        for(unsigned int i=0;i<Loop::loopArray.size();i++){
            if(Loop::loopArray[i]->id==ui->LoopIDspinBox->value()){
                lp=Loop::loopArray[i];
                break;
            }
        }
        for(unsigned int i=0;i<Vertex::vertexArray.size();i++){
            if(Vertex::vertexArray[i]->id==ui->VertexIDspinBox->value()){
                vp=Vertex::vertexArray[i];
                break;
            }
        }
        ui->mefButton->setEnabled(true);
        ui->kemrButton->setEnabled(true);

        BrepModel::Instance()->mev(vp,newP,lp);
        ui->VertexIDspinBox->setValue(ui->VertexIDspinBox->value()+1);
    }
    else if(op==mef){
        Vertex *sv;Vertex* ev;Loop* lp;
        for(unsigned int i=0;i<Loop::loopArray.size();i++){
            if(Loop::loopArray[i]->id==ui->LoopIDspinBox->value()){
                lp=Loop::loopArray[i];
                break;
            }
        }
        for(unsigned int i=0;i<Vertex::vertexArray.size();i++){
            if(Vertex::vertexArray[i]->id==ui->VertexIDspinBox->value()){
                sv=Vertex::vertexArray[i];
                break;
            }
        }
        for(unsigned int i=0;i<Vertex::vertexArray.size();i++){
            if(Vertex::vertexArray[i]->id==ui->VertexIDspinBox_2->value()){
                ev=Vertex::vertexArray[i];
                break;
            }
        }
        ui->sweepButton->setEnabled(true);
        ui->kfmrhButton->setEnabled(true);
        BrepModel::Instance()->mef(sv,ev,lp);
    }
    else if(op==kemr){
        Vertex *sv;
        Vertex* ev;
        Loop* lp;
        for(unsigned int i=0;i<Loop::loopArray.size();i++){
            if(Loop::loopArray[i]->id==ui->LoopIDspinBox->value()){
                lp=Loop::loopArray[i];
                break;
            }
        }
        for(unsigned int i=0;i<Vertex::vertexArray.size();i++){
            if(Vertex::vertexArray[i]->id==ui->VertexIDspinBox->value()){
                sv=Vertex::vertexArray[i];
                break;
            }
        }
        for(unsigned int i=0;i<Vertex::vertexArray.size();i++){
            if(Vertex::vertexArray[i]->id==ui->VertexIDspinBox_2->value()){
                ev=Vertex::vertexArray[i];
                break;
            }
        }
        BrepModel::Instance()->kemr(sv,ev,lp);
    }
    else if(op==kfmrh){
        Face *fa;Face* fb;

        Face* fit=Solid::GetEntityPtr()->face_list;
        while(true){
            if(fit->id==ui->FaceIDspinBox->value()){
                fa=fit;
                break;
            }
            fit=fit->next;
            if(fit==Solid::GetEntityPtr()->face_list)
                break;
        }


        fit=Solid::GetEntityPtr()->face_list;
        while(true){
            if(fit->id==ui->FaceIDspinBox_2->value()){
                fb=fit;
                break;
            }
            fit=fit->next;
            if(fit==Solid::GetEntityPtr()->face_list)
                break;
        }

        BrepModel::Instance()->kfmrh(fa,fb);
    }
    else if(op==sweep){
        Loop* lp;
        for(unsigned int i=0;i<Loop::loopArray.size();i++){
            if(Loop::loopArray[i]->id==ui->LoopIDspinBox->value()){
                lp=Loop::loopArray[i];
                break;
            }
        }
        BrepModel::Instance()->sweep(QVector3D(ui->doubleSpinBox->value(),ui->doubleSpinBox_2->value(),ui->doubleSpinBox_3->value()),lp);
    }
    else if(op==semv){
        Vertex *sv;
        Vertex* ev;
        for(unsigned int i=0;i<Vertex::vertexArray.size();i++){
            if(Vertex::vertexArray[i]->id==ui->VertexIDspinBox->value()){
                sv=Vertex::vertexArray[i];
                break;
            }
        }
        for(unsigned int i=0;i<Vertex::vertexArray.size();i++){
            if(Vertex::vertexArray[i]->id==ui->VertexIDspinBox_2->value()){
                ev=Vertex::vertexArray[i];
                break;
            }
        }
        BrepModel::Instance()->semv(sv,ev,QVector3D(ui->doubleSpinBox->value(),ui->doubleSpinBox_2->value(),ui->doubleSpinBox_3->value()));
    }
    GUI->update();
}


void MainWindow::on_sweepButton_released()
{
    op=sweep;
    hideAll();
    ui->label_5->show();
    ui->doubleSpinBox->show();
    ui->doubleSpinBox_2->show();
    ui->doubleSpinBox_3->show();
    ui->label_2->show();
    ui->LoopIDspinBox->show();
}

void MainWindow::on_genExampleButton_released()
{
    ui->mvfsButton->setEnabled(false);
    ui->mefButton->setEnabled(true);
    ui->kemrButton->setEnabled(true);
    ui->kfmrhButton->setEnabled(true);
    ui->mevButton->setEnabled(true);
    ui->semvButton->setEnabled(true);
    ui->sweepButton->setEnabled(true);
    ui->genExampleButton->setEnabled(false);
    ui->buttonBox->setEnabled(true);


    BrepModel::Instance()->mvfs(QVector3D(-3.0,-3.0,0));
    BrepModel::Instance()->mev(Vertex::vertexArray[0],QVector3D(10.0,-3.0,0),Loop::loopArray[0]);
    BrepModel::Instance()->mev(Vertex::vertexArray[1],QVector3D(7.0,10,0),Loop::loopArray[0]);
    BrepModel::Instance()->mev(Vertex::vertexArray[2],QVector3D(-5.0,7.0,0),Loop::loopArray[0]);
    BrepModel::Instance()->mef(Vertex::vertexArray[3],Vertex::vertexArray[0],Loop::loopArray[0]);

    BrepModel::Instance()->mev(Vertex::vertexArray[0],QVector3D(2,2,0),Loop::loopArray[1]);
    BrepModel::Instance()->mev(Vertex::vertexArray[4],QVector3D(4,4,0),Loop::loopArray[1]);
    BrepModel::Instance()->mev(Vertex::vertexArray[5],QVector3D(4,4,0),Loop::loopArray[1]);
    BrepModel::Instance()->mev(Vertex::vertexArray[6],QVector3D(2,7,0),Loop::loopArray[1]);
    BrepModel::Instance()->mef(Vertex::vertexArray[7],Vertex::vertexArray[4],Loop::loopArray[1]);

    BrepModel::Instance()->mev(Vertex::vertexArray[0],QVector3D(-2,-2,0),Loop::loopArray[1]);
    BrepModel::Instance()->mev(Vertex::vertexArray[8],QVector3D(0,-2,0),Loop::loopArray[1]);
    BrepModel::Instance()->mev(Vertex::vertexArray[9],QVector3D(0,2,0),Loop::loopArray[1]);
    BrepModel::Instance()->mev(Vertex::vertexArray[10],QVector3D(-2,0,0),Loop::loopArray[1]);
    BrepModel::Instance()->mef(Vertex::vertexArray[11],Vertex::vertexArray[8],Loop::loopArray[1]);

    BrepModel::Instance()->kemr(Vertex::vertexArray[0],Vertex::vertexArray[8],Loop::loopArray[1]);

    BrepModel::Instance()->kemr(Vertex::vertexArray[0],Vertex::vertexArray[4],Loop::loopArray[1]);

    BrepModel::Instance()->kfmrh(Loop::loopArray[3]->face,Loop::loopArray[0]->face);
    BrepModel::Instance()->kfmrh(Loop::loopArray[2]->face,Loop::loopArray[0]->face);

    // BrepModel::Instance()->sweep(QVector3D(0,0,7),Loop::loopArray[0]);
    // BrepModel::Instance()->sweep(QVector3D(0,0,7),Loop::loopArray[2]);
    // BrepModel::Instance()->sweep(QVector3D(0,0,7),Loop::loopArray[3]);
    // BrepModel::Instance()->mev(Vertex::vertexArray[0],QVector3D(-3,-3.0,7),Loop::loopArray[1]);
    // BrepModel::Instance()->mev(Vertex::vertexArray[12],QVector3D(-5,7,7),Loop::loopArray[1]);
    // BrepModel::Instance()->mef(Vertex::vertexArray[7],Vertex::vertexArray[4],Loop::loopArray[1]);
}

void MainWindow::on_deleteAllButton_released()
{
    Face::numFace=0;
    Edge::numEdge=0;
    Loop::loopArray.clear();
    Loop::numLoop=0;
    Vertex::vertexArray.clear();
    Vertex::numVertex=0;
    BrepModel::Instance()->clearData();
    Solid::GetEntityPtr()->face_list=nullptr;
    GUI->updateGLWidget();
    GUI->updateTreeTable();
    hideAll();
    ui->mvfsButton->setEnabled(true);
    ui->mefButton->setEnabled(false);
    ui->kemrButton->setEnabled(false);
    ui->kfmrhButton->setEnabled(false);
    ui->mevButton->setEnabled(false);
    ui->semvButton->setEnabled(false);
    ui->sweepButton->setEnabled(false);
    ui->genExampleButton->setEnabled(true);
}
